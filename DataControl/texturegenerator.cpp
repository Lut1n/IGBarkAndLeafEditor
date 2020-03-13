#include "DataControl/texturegenerator.h"
#include "GuiContext/loadinginfo.h"

#include <Graphics/ImageOperation.h>
#include <Descriptors/JsonImageOp.h>

#include <QUrl>
#include <QFile>
#include <QDebug>

using namespace imp;

// -----------------------------------------------------------------------------------------------------------------------
TextureGenerator::TextureGenerator()
{

}

using OperationCache = std::vector<ImgOperation::Ptr>;

void generateImageFromJsonOverride(const std::string data)
{
    std::cout << "loading json data..." << std::endl;
    OperationCache opCache;
    std::string json = data;
    std::string jsonWithoutSpace = removeSpace(json);

    JsonObject* root = dynamic_cast<JsonObject*>( parse(jsonWithoutSpace) );
    JsonArray* array = root->getArray("operations");

    JsonObject* prop = root->getObject("properties");
    ImgOperation::s_width = prop->getNumeric("width")->value;
    ImgOperation::s_height = prop->getNumeric("height")->value;

    int s = array->value.size();
    for(int i=0; i<s; ++i)
    {
        JsonObject* jo = array->getObject(i);
        ImgOperation::Ptr op = createFromJsonObj(jo);
        opCache.push_back(op);
    }
    std::cout << "done" << std::endl;


    std::cout << "start generations... (0/" << opCache.size() << ")" << std::endl;
    int notReady = opCache.size();
    while(notReady)
    {
        for(auto op : opCache) if( !op->_done )
        {
            if( op->isReady() )
            {
                std::cout << "applying " << op->_opName << "..." << std::endl;
                op->apply();
                notReady--;
                std::cout << "done (" << (opCache.size()-notReady) << "/" << opCache.size() << ")" << std::endl;
            }
        }
    }
}


void decode_packet_header(unsigned char packet_header, bool& rl_type, int& len)
{
    unsigned char type_filter = 0b10000000;
    unsigned char size_filter = 0b01111111;

    rl_type = (packet_header & type_filter) == type_filter;
    len = 1 + int(packet_header & size_filter);
}


void readU8(QFile& is, unsigned char* buf, int size)
{
    is.read( reinterpret_cast<char*>(buf), size*sizeof(unsigned char) );
}


void readU8(QFile& is, std::vector<unsigned char>& buf, int size)
{
    buf.resize(size);
    readU8(is,buf.data(),size);
}


void readU8_rle(QFile& is, unsigned char* dst, int size, int chnl)
{
    bool to_repeat; int run_len;
    unsigned char packet_header;
    std::vector<unsigned char> buf;

    int progress = 0;
    while(progress < size)
    {
        is.read(reinterpret_cast<char*>(&packet_header),sizeof(unsigned char));
        decode_packet_header(packet_header, to_repeat, run_len);

        if(to_repeat)
        {
            readU8(is, buf, chnl);
            for(int l=0;l<run_len;++l)
            {
                for(int c=0;c<chnl;++c) dst[progress+c]= buf[c];
                progress += chnl;
            }
        }
        else
        {
            readU8(is, &dst[progress], run_len*chnl);
            progress += run_len*chnl;
        }
    }
}


Image::Ptr TextureGenerator::loadTGA(const std::string& filename, bool flipY)
{
    unsigned short w, h;
    unsigned char pixel_depth;

    QFile tgaFile(filename.c_str());
    if ( !tgaFile.open(QIODevice::ReadOnly) ) return Image::create(8,8,3);

    unsigned char id_len = 0;
    unsigned char colormap;
    unsigned char image_type = 0;

    tgaFile.read(reinterpret_cast<char*>(&id_len), sizeof id_len); //ID length (1o)
    tgaFile.read(reinterpret_cast<char*>(&colormap), sizeof colormap); // Color map type (1o)
    tgaFile.read(reinterpret_cast<char*>(&image_type), sizeof image_type); // Image type (1o)

    static const int NO_IMAGE = 0;
    static const int INDEXED = 1;
    // static const int TRUE_COLOR = 2;
    // static const int GRAYSCALE = 3;
    static const int INDEXED_RLE = 9;
    static const int TRUE_COLOR_RLE = 10;
    static const int GRAYSCALE_RLE = 11;
    if(image_type == INDEXED || image_type == INDEXED_RLE)
        std::cout << "TGA loader : warning - indexed color (not supported)" << std::endl;
    if(image_type == NO_IMAGE)
        std::cout << "TGA loader : warning - no image" << std::endl;

    bool rle = image_type==INDEXED_RLE || image_type==TRUE_COLOR_RLE || image_type==GRAYSCALE_RLE;

    // if(image_type == 3) chnl = 1; // black and white
    // if(image_type == 2) chnl = 3; // TrueColor

    std::vector<unsigned char> ignored;
    readU8(tgaFile, ignored, 5); // colormap info (ignored)

    unsigned short x_minus_o = 0;
    unsigned short y_minus_o = 0;
    tgaFile.read(reinterpret_cast<char*>(&x_minus_o), sizeof x_minus_o); // X-origin (2o)
    tgaFile.read(reinterpret_cast<char*>(&y_minus_o), sizeof y_minus_o); // Y-origin (2o)

    tgaFile.read(reinterpret_cast<char*>(&w), sizeof w); // width (2o)
    tgaFile.read(reinterpret_cast<char*>(&h), sizeof h); // height (2o)

    tgaFile.read(reinterpret_cast<char*>(&pixel_depth), sizeof pixel_depth); // px depth (1o)
    int chnl = pixel_depth / 8;	// 24 for RGB; 32 for RGBA


    // Image description
    // bits 0,1,2,3 : alpha channel depth (ignored)
    // bits 4,5 : flip X and Y
    // bits 6,7 : unused (ignored)
    char id_info;
    tgaFile.read(&id_info, sizeof(char));
    char flip_y_bit = 0b00100000;
    bool need_flipY = (id_info & flip_y_bit) == flip_y_bit;
    if(need_flipY) std::cout << "need flip Y" << std::endl;
    flipY ^= need_flipY;

    Image::Ptr result = Image::create(w,h,chnl);
    Image::Ptr tmp_img = Image::create(w,h,chnl);
    int totalSize = result->width() * result->height() * result->channels();

    if(rle) // compression run-length encoding
    {
        readU8_rle(tgaFile, tmp_img->data(), totalSize, chnl);

    }
    else // no compression - just read
    {
        readU8(tgaFile, tmp_img->data(), totalSize);
    }

    bool effective_flip_y = !flipY; // imp::Image seems to be flipped on screen (has to be confirmed)
    if(chnl == 1) result->copy(tmp_img, {0,1,2,3}, effective_flip_y);
    else if(chnl == 2) result->copy(tmp_img, {1,0}, effective_flip_y);
    else if(chnl == 3) result->copy(tmp_img, {2,1,0}, effective_flip_y);
    else if(chnl == 4) result->copy(tmp_img, {2,1,0,3}, effective_flip_y);

    return result;
}


// -----------------------------------------------------------------------------------------------------------------------
Image::Ptr TextureGenerator::loadJSON(const std::string& jsonFilename)
{
    Image::Ptr res = Image::create(8,8,3); res->fill(Vec4(0.0,0.0,1.0,1.0));
    QFile file(jsonFilename.c_str());
    if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QString jsonData = file.readAll();
        qDebug() << "jsonData = " << jsonData;
        generateImageFromJsonData( jsonData.toStdString() );

        if( !JsonFakeSaveOperation::s_fakeFiles.empty() )
        {
            res = JsonFakeSaveOperation::s_fakeFiles[jsonFilename];
        }
    }
    return res;
}
