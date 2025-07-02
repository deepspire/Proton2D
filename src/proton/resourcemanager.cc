#include "resourcemanager.hh"
#include "logman.hh"
#include <SDL3_gfx/SDL3_gfxPrimitives.h>

namespace Proton
{
    ResourceManager &ResourceManager::getInstance()
    {
        static ResourceManager instance;
        return instance;
    }

    ResourceManager::~ResourceManager()
    {
        clearCache();
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

        std::string fullPath = "assets/" + path;

        SDL_Surface *surface = IMG_Load(fullPath.c_str());
        if (!surface)
        {
            Proton::Log("Failed to load image: ", fullPath, ". error: ", SDL_GetError());
            return nullptr;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surface);
        SDL_DestroySurface(surface);

        if (!texture)
        {
            Proton::Log("Failed to create texture from surface: ", fullPath, ". error: ", SDL_GetError());
            return nullptr;
        }

        this->textureCache[path] = texture;
        return texture;
    }

    SDL_Surface *ResourceManager::getIcon(const std::string &path)
    {
        std::string fullPath = "assets/" + path;

        if (this->currentIcon)
        {
            SDL_DestroySurface(currentIcon);
        }

        currentIcon = IMG_Load(fullPath.c_str());
        if (!currentIcon)
        {
            Proton::Log("Failed to load icon: ", fullPath);
            return nullptr;
        }

        return currentIcon;
    }

    TTF_Font *ResourceManager::getFont(const std::string &path, int fontSize)
    {
        std::string key = path + ":" + std::to_string(fontSize);
        auto it = fontCache.find(key);
        if (it != fontCache.end())
        {
            return it->second;
        }

        std::string fullPath = "assets/" + path;

        TTF_Font *font = TTF_OpenFont(fullPath.c_str(), fontSize);
        if (!font)
        {
            Proton::Log("Failed to load font: ", SDL_GetError());
            return nullptr;
        }

        this->fontCache[key] = font;
        return font;
    }

    SDL_Texture *ResourceManager::getRoundedRectTexture(SDL_Renderer *render, int width, int height, int roundness, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        std::string key = "rounded_rect:" + std::to_string(width) + ":" + std::to_string(height) + ":" +
                          std::to_string(roundness);

        auto it = roundedRectCache.find(key);
        if (it != roundedRectCache.end())
        {
            return it->second;
        }

        SDL_Texture *texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888,
                                                 SDL_TEXTUREACCESS_TARGET, width, height);
        if (!texture)
        {
            Proton::Log("Failed to create rounded rectangle texture: ", SDL_GetError());
            return nullptr;
        }

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);

        SDL_SetRenderTarget(render, texture);
        SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
        SDL_RenderClear(render);

        roundedBoxRGBA(render, 0, 0, width - 1, height - 1, roundness * 8, r, g, b, a);

        SDL_SetRenderTarget(render, nullptr);

        roundedRectCache[key] = texture;
        return texture;
    }

    void ResourceManager::clearCache()
    {
        for (auto const &[path, texture] : textureCache)
        {
            SDL_DestroyTexture(texture);
        }
        textureCache.clear();

        for (auto const &[key, font] : fontCache)
        {
            TTF_CloseFont(font);
        }
        fontCache.clear();

        for (auto const &[key, texture] : roundedRectCache)
        {
            SDL_DestroyTexture(texture);
        }
        roundedRectCache.clear();

        if (currentIcon)
        {
            SDL_DestroySurface(currentIcon);
            currentIcon = nullptr;
        }
    }
}