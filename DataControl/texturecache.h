#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

#include <QObject>
#include <map>

#include <Graphics/Image.h>

class TextureCache : public QObject
{
    Q_OBJECT
public:
    TextureCache();

    bool isLoaded(const std::string& url) const;
    imp::Image::Ptr load(const std::string& url);

    static TextureCache* s_instance;
    static TextureCache* instance();

protected:

    std::map<std::string,imp::Image::Ptr> _cache;
};

#endif // TEXTURECACHE_H
