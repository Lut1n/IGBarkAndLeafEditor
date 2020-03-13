#include "texturecache.h"
#include "texturegenerator.h"

TextureCache* TextureCache::s_instance = nullptr;

TextureCache::TextureCache()
{
    s_instance = this;
}


TextureCache* TextureCache::instance()
{
    return s_instance;
}

bool TextureCache::isLoaded(const std::string& url) const
{
    return _cache.find(url) != _cache.end();
}

imp::Image::Ptr TextureCache::load(const std::string& url)
{
    if( !isLoaded(url) )
    {
        std::string ext = ".tga";
        if(url.size() > 5) ext = url.substr(url.size()-5);

        if(ext == ".json")
            _cache[url] = TextureGenerator::loadJSON(url);
        else
            _cache[url] = TextureGenerator::loadTGA(url);
    }
    return _cache[url];
}
