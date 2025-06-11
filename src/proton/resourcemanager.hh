#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <unordered_map>

namespace Proton
{
    class ResourceManager
    {
    public:
        static ResourceManager &getInstance();

        ResourceManager(const ResourceManager &) = delete;
        ResourceManager &operator=(const ResourceManager &) = delete;

        SDL_Texture* getTexture(SDL_Renderer *render, const std::string& path);
        TTF_Font* getFont(const std::string& path, int fontSize);
        SDL_Surface* getIcon(const std::string& path);

        void clearCache();

    private:
        ResourceManager() = default;
        ~ResourceManager();

        std::unordered_map<std::string, SDL_Texture *> textureCache;
        std::unordered_map<std::string, TTF_Font *> fontCache;
        SDL_Surface* currentIcon;
    };
};