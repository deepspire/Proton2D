#include "resourcemanager.hh"
#include "logman.hh"

namespace Proton
{
    ResourceManager &ResourceManager::getInstance()
    {
        static ResourceManager instance;
        return instance;
    }

    ResourceManager::~ResourceManager()
    {
        this->clearCache();
        Proton::Log("ResourceManager destroyed, all textures unloaded");
    }

    SDL_Texture *ResourceManager::getTexture(SDL_Renderer *render, const std::string &path)
    {
        if (!render)
        {
            Proton::Log("attempt to get texture without renderer..");
            return nullptr;
        }

        auto it = this->textureCache.find(path);
        if (it != this->textureCache.end())
        {
            return it->second;
        }

        Proton::Log("loading new texture: ", path);
        SDL_Surface *surface = IMG_Load(path.c_str());
        if (!surface)
        {
            Proton::Log("failed to load image: ", path, ". error: ", SDL_GetError());
            return nullptr;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surface);
        SDL_DestroySurface(surface);

        if (!texture)
        {
            Proton::Log("failed to create texture from surface: ", path, ". error: ", SDL_GetError());
            return nullptr;
        }

        this->textureCache[path] = texture;
        return texture;
    }

    TTF_Font* ResourceManager::getFont(const std::string& path, int fontSize)
    {
        std::string key = path + ":" + std::to_string(fontSize);
        auto it = fontCache.find(key);
        if (it != fontCache.end())
        {
            return it->second;
        }

        Proton::Log("loading new font: ", path, " with size ", std::to_string(fontSize));
        TTF_Font* font = TTF_OpenFont(path.c_str(), fontSize);
        if (!font)
        {
            Proton::Log("failed to load font: ", SDL_GetError());
            return nullptr;
        }
        
        fontCache[key] = font;
        return font;
    }

    void ResourceManager::clearCache()
    {
        for (auto const& [path, texture] : textureCache) {
            SDL_DestroyTexture(texture);
        }
        textureCache.clear();
        
        for (auto const& [key, font] : fontCache) {
            TTF_CloseFont(font);
        }
        fontCache.clear();
    }
}