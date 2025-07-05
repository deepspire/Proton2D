#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <SDL3_image/SDL_image.h>
#include <unordered_map>
#if defined(__ANDROID__)
#include <android/asset_manager.h>
#endif
#include "miniaudio.h"
#include "logman.hh"

namespace Proton
{
    class ResourceManager
    {
    public:
        static ResourceManager &getInstance();

        ResourceManager(const ResourceManager &) = delete;
        ResourceManager &operator=(const ResourceManager &) = delete;

        ma_engine *getAudioEngine();
        void initAudioEngine();
        SDL_Texture *getTexture(SDL_Renderer *render, const std::string &path);
        TTF_Font *getFont(const std::string &path, int fontSize);
        SDL_Surface *getIcon(const std::string &path);

#if defined(__ANDROID__)
        void setAssetManager(AAssetManager *manager);
        AAssetManager *getAssetManager();
#endif

        void clearCache();

    private:
        ResourceManager() = default;
        ~ResourceManager();

        ma_engine currentAudioEngine{};
        bool audioEngineInitialized{};

        std::unordered_map<std::string, SDL_Texture *> textureCache;
        std::unordered_map<std::string, TTF_Font *> fontCache;
        SDL_Surface *currentIcon{};

#if defined(__ANDROID__)
        AAssetManager *assetManager = nullptr;
#endif
    };
};