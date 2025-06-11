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
        /*this->clearCache(); // чистим когда я захочу
        Proton::Log("ResourceManager destroyed, all textures unloaded");*/
    }

    SDL_Texture *ResourceManager::getTexture(SDL_Renderer *render, const std::string &path)
    {
        if (!render)
        {
            Proton::Log("Attempt to get texture without renderer..");
            return nullptr;
        }

        auto it = this->textureCache.find(path);
        if (it != this->textureCache.end())
        {
            return it->second;
        }

        Proton::Log("Loading new texture: ", path);
        SDL_Surface *surface = IMG_Load(path.c_str());
        if (!surface)
        {
            Proton::Log("Failed to load image: ", path, ". error: ", SDL_GetError());
            return nullptr;
        }
        Proton::Log("Texture ", path, " loaded successfully");
        SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surface);
        SDL_DestroySurface(surface);


        if (!texture)
        {
            Proton::Log("Failed to create texture from surface: ", path, ". error: ", SDL_GetError());
            return nullptr;
        }

        this->textureCache[path] = texture;
        return texture;
    }

    SDL_Surface* ResourceManager::getIcon(const std::string& path)
    {
        Proton::Log("Loading new icon: ", path);
        if(this->currentIcon)
        {
            delete currentIcon;
        }
        currentIcon = IMG_Load(path.c_str());
        if(!currentIcon)
        {
            Proton::Log("Failed to load icon: ", path);
            return nullptr;
        }
        Proton::Log("Icon ", path, " loaded successfully");

        return currentIcon;
    }

    TTF_Font* ResourceManager::getFont(const std::string& path, int fontSize)
    {
        std::string key = path + ":" + std::to_string(fontSize);
        auto it = fontCache.find(key);
        if (it != fontCache.end())
        {
            return it->second;
        }

        Proton::Log("Loading new font: ", path, " with size ", std::to_string(fontSize));
        TTF_Font* font = TTF_OpenFont(path.c_str(), fontSize);
        if (!font)
        {
            Proton::Log("Failed to load font: ", SDL_GetError());
            return nullptr;
        }
        
        this->fontCache[key] = font;
        return font;
    }

    void ResourceManager::clearCache()
    {
        for (auto const& [path, texture] : textureCache) {
            SDL_DestroyTexture(texture);
        }
        textureCache.clear();
        
        for (auto const& [key, font] : fontCache) {
            Log("очищаю шрифт");
            TTF_CloseFont(font);
        }
        Log("Доне!!!");
        fontCache.clear();

        if (currentIcon)
        {
            Log("pizdec?");
            delete currentIcon;
        }
    }
}