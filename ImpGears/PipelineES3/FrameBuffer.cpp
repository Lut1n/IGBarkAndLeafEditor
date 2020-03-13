
#include <SceneGraph/Camera.h>

#include <PipelineES3/FrameBuffer.h>
#include <PipelineES3/GlError.h>

#include <cstdio>
#include <iostream>

IMPGEARS_BEGIN

std::uint32_t FrameBuffer::_s_count = 0;

FrameBuffer* FrameBuffer::s_bound = nullptr;

//--------------------------------------------------------------
FrameBuffer::FrameBuffer()
    : _id(0)
    , _textureCount(0)
    , _hasDepthBuffer(false)
{
}

//--------------------------------------------------------------
FrameBuffer::~FrameBuffer()
{
    destroy();
}

//--------------------------------------------------------------
void FrameBuffer::build(const std::vector<Texture::Ptr>& textures, bool depthBuffer)
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    GLint maxAttach = 0, maxDrawBuf = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttach);
    GL_CHECKERROR("get max attachments");
    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuf);
    GL_CHECKERROR("get max draw buffers");

    destroy();
    _hasDepthBuffer = depthBuffer;
    // TODO : test  GL_MAX_COLOR_ATTACHMENTS & GL_MAX_DRAW_BUFFERS
    _textureCount = textures.size();
    _colorTextures = textures;

    // if(_textureCount > maxAttach) qDebug() << "texture count > maxAttach";
    // if(_textureCount > maxDrawBuf) qDebug() << "texture count > maxDrawBuf";

    // qDebug() << "build frame buffer (" << _textureCount << "):";
    // qDebug() << "   maxAttach = " << maxAttach;
    // qDebug() << "   maxDrawBuf = " << maxDrawBuf;


    if(_textureCount==0) return;

    GLuint id;
    glGenFramebuffers(1, &id);
    _s_count++;
    _id = (int)id;
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    GL_CHECKERROR("bind FBO");

    _drawBuffers.clear();
    // GLenum drawBuffers[_textureCount];
    for(int i=0; i<_textureCount; ++i)
    {
        _colorTextures[i]->bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, _colorTextures[i]->getVideoID(), 0);
        // drawBuffers[i] = GL_COLOR_ATTACHMENT0+i;
        _drawBuffers.push_back( (unsigned int)(GL_COLOR_ATTACHMENT0+i) );
        GL_CHECKERROR("set color buffer");
    }
    f->glDrawBuffers(_drawBuffers.size(), static_cast<GLenum*>(_drawBuffers.data()) );
    GL_CHECKERROR("set draw buffers");

    if(_hasDepthBuffer)
    {
        int width = _colorTextures[0]->width();
        int height = _colorTextures[0]->height();

        // render buffer
        GLuint rbo = _rbo;
        glGenRenderbuffers(1, &rbo); _rbo = rbo;
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,rbo);
        GL_CHECKERROR("set depth buffer");
    }

    GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(error != GL_FRAMEBUFFER_COMPLETE)
    {
        std::string string;
        ENUM_TO_STR(error, GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT, string)
        // ENUM_TO_STR(error, GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT, string)
        ENUM_TO_STR(error, GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT, string)
        ENUM_TO_STR(error, GL_FRAMEBUFFER_UNSUPPORTED, string)
        // std::cout << "[impError] FrameBuffer creation failed : " << string << std::endl;
        qDebug() << "[impError] FrameBuffer creation failed : " << string.c_str();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//--------------------------------------------------------------
void FrameBuffer::build(int width, int height, int textureCount, bool depthBuffer)
{
    std::vector<Texture::Ptr> textures;
    textures.resize(textureCount);

    for(int i=0; i<textureCount; ++i)
    {
        textures[i] = Texture::create();
        textures[i]->loadFromMemory(NULL, width, height, 4);
        textures[i]->update();
        textures[i]->bind();
    }

    build(textures, depthBuffer);
}

//--------------------------------------------------------------
void FrameBuffer::build(const CubeMap::Ptr& cubemap, int faceID, bool depthBuffer)
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    GLint maxAttach = 0, maxDrawBuf = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttach);
    GL_CHECKERROR("get max attachments");
    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuf);
    GL_CHECKERROR("get max draw buffers");


    destroy();
    _hasDepthBuffer = depthBuffer;
    // TODO : test  GL_MAX_COLOR_ATTACHMENTS & GL_MAX_DRAW_BUFFERS
    _textureCount = 1;
    _colorTextures.clear();
    _cubemap = cubemap;
    _faceID = faceID;
    _useCubeMap = true;

    if(!_cubemap) return;

    GLuint id;
    glGenFramebuffers(1, &id);
    _s_count++;
    _id = (int)id;
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    GL_CHECKERROR("bind FBO");

    _drawBuffers.clear();
    // GLenum drawBuffers[_textureCount];
    for(int i=0; i<_textureCount; ++i)
    {
        _cubemap->bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_CUBE_MAP_POSITIVE_X+faceID, _cubemap->getVideoID(), 0);
        // drawBuffers[i] = GL_COLOR_ATTACHMENT0+i;
        _drawBuffers.push_back( (unsigned int)(GL_COLOR_ATTACHMENT0+i) );
        GL_CHECKERROR("set color buffer");
    }
    // glDrawBuffers(_textureCount, drawBuffers);
    f->glDrawBuffers(_drawBuffers.size(), static_cast<GLenum*>(_drawBuffers.data()) );
    GL_CHECKERROR("set draw buffers");

    if(_hasDepthBuffer)
    {
        int width = 1024.0; // _colorTextures[0]->width();
        int height = 1024.0; // _colorTextures[0]->height();

        // render buffer
        GLuint rbo = _rbo;
        glGenRenderbuffers(1, &rbo); _rbo = rbo;
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,rbo);
        GL_CHECKERROR("set depth buffer");
    }

    GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(error != GL_FRAMEBUFFER_COMPLETE)
    {
        std::string string;
        ENUM_TO_STR(error, GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT, string)
        // ENUM_TO_STR(error, GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT, string)
        ENUM_TO_STR(error, GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT, string)
        ENUM_TO_STR(error, GL_FRAMEBUFFER_UNSUPPORTED, string)
        qDebug() << "[impError] FrameBuffer creation failed : " << string.c_str();
        // std::cout << "[impError] FrameBuffer creation failed : " << string << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//--------------------------------------------------------------
void FrameBuffer::destroy()
{
    GLuint id = _id;
    glDeleteFramebuffers(1, &id);
    if(_id > 0) _s_count--;

    _colorTextures.clear();
    if(_hasDepthBuffer) {GLuint rbo=_rbo; glDeleteRenderbuffers(1,&rbo);_rbo=rbo;}

    _textureCount = 0;
    _hasDepthBuffer = false;
}

//--------------------------------------------------------------
Texture::Ptr FrameBuffer::getTexture(int n)
{
    return _colorTextures[n];
}

//--------------------------------------------------------------
CubeMap::Ptr FrameBuffer::getCubeMap()
{
    return _cubemap;
}

//--------------------------------------------------------------
int FrameBuffer::getFaceID()
{
    return _faceID;
}

//--------------------------------------------------------------
void FrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    GL_CHECKERROR("FrameBuffer bind");
    s_bound = this;
}

//--------------------------------------------------------------
void FrameBuffer::unbind()
{
    s_bound = nullptr;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GL_CHECKERROR("FrameBuffer unbind");
}

//--------------------------------------------------------------
void FrameBuffer::setClearColors(const std::vector<Vec4>& clearColors)
{
    _clearColors = clearColors;
}

//--------------------------------------------------------------
void FrameBuffer::s_clearBuffers()
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    if(s_bound == nullptr) return;

    std::vector<unsigned int>& drawBuffers = s_bound->_drawBuffers;
    std::vector<Vec4>& clearColors = s_bound->_clearColors;

    unsigned int size = drawBuffers.size();
    if(clearColors.size() < size)
    {
        // std::cout << "FrameBuffer : warning clearColor buffer size is smaller than drawBuffers size" << std::endl;
        qDebug() <<  "FrameBuffer : warning clearColor buffer size is smaller than drawBuffers size";
        return;
    }

    GL_CHECKERROR("FrameBuffer : before framebuffer clear color");
    // qDebug() << "clearcolor for frameBuffer with " << size << " draw buffers";

    GLint currFbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currFbo);
    GL_CHECKERROR("FrameBuffer : get id");
    // qDebug() << "curr fbo = " << currFbo;

    for(unsigned int i=0; i<size; ++i)
    {
        GLenum e = static_cast<GLenum>(drawBuffers[i]);
        // qDebug() << "GLenum = " << (e-GL_COLOR_ATTACHMENT0);

        // f->glDrawBuffers( 1, &e );
        // GL_CHECKERROR("FrameBuffer : select buffer for clear color");
        Vec4 color = clearColors[i];
        // glClearColor( color[0], color[1], color[2], color[3] );
        // GL_CHECKERROR("FrameBuffer : set clear color");
        // glClear( GL_COLOR_BUFFER_BIT );
        // GL_CHECKERROR("FrameBuffer : apply clear color");

        // const float colorfv[] = { color[], 0, 0, 1 };
        f->glClearBufferfv(GL_COLOR, i, color.data());
        GL_CHECKERROR("FrameBuffer : clearBufferfv");
    }

    // f->glDrawBuffers(drawBuffers.size(), static_cast<GLenum*>(drawBuffers.data()) );
    // GL_CHECKERROR("FrameBuffer : reset draw buffers selection");
}

IMPGEARS_END
