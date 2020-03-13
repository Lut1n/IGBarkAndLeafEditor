#ifndef TEXTUREGENERATOR_H
#define TEXTUREGENERATOR_H

#include <Graphics/Image.h>

class TextureGenerator
{
public:
    TextureGenerator();

    static imp::Image::Ptr loadTGA(const std::string& filename, bool flipY = false);
    static imp::Image::Ptr loadJSON(const std::string& jsonFilename);
};

#endif // TEXTUREGENERATOR_H
