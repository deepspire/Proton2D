#include "proton/resourcemanager.hh"
#include "proton/logman.hh"
#include "proton/physics.hh"

namespace Proton
{
ResourceManager &ResourceManager::getInstance()
{
    static ResourceManager instance;
    return instance;
}

ResourceManager::~ResourceManager() { this->clearCache(); }

void ResourceManager::initAudioEngine()
{
    if (!this->audioEngineInitialized)
    {
        if (ma_engine_init(nullptr, &this->currentAudioEngine) != MA_SUCCESS)
        {
            Proton::Log("Unable to init sound engine");
            return;
        }

        this->audioEngineInitialized = true;

        return;
    }

    Log("Audio engine is defined already!!");
}

ma_engine *ResourceManager::getAudioEngine()
{
    if (!this->audioEngineInitialized)
    {
        this->initAudioEngine();
    }
    return &this->currentAudioEngine;
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

#ifndef __ANDROID__
    std::string fullPath = "assets/" + path;
#else
    std::string fullPath = path;
#endif

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
#ifndef __ANDROID__
    std::string fullPath = "assets/" + path;
#else
    std::string fullPath = path;
#endif

    if (this->currentIcon)
    {
        SDL_DestroySurface(currentIcon);
    }

    currentIcon = IMG_Load(fullPath.c_str());
    if (!currentIcon)
    {
        Log("Failed to load icon: ", fullPath);
        return nullptr;
    }

    return currentIcon;
}

TTF_Font *ResourceManager::getFont(const std::string &path, int fontSize)
{
    const std::string key = path + ":" + std::to_string(fontSize);
    if (const auto it = fontCache.find(key); it != fontCache.end())
    {
        return it->second;
    }

#ifndef __ANDROID__
    std::string fullPath = "assets/" + path;
#else
    std::string fullPath = path;
#endif

    TTF_Font *font = TTF_OpenFont(fullPath.c_str(), static_cast<float>(fontSize)); // TODO font size float too?
    if (!font)
    {
        Log("Failed to load font: ", SDL_GetError());
        return nullptr;
    }

    this->fontCache[key] = font;
    return font;
}

#if defined(__ANDROID__)
void ResourceManager::setAssetManager(AAssetManager *manager) { this->assetManager = manager; }

AAssetManager *ResourceManager::getAssetManager() { return this->assetManager; }
#endif

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

    if (currentIcon)
    {
        SDL_DestroySurface(currentIcon);
        currentIcon = nullptr;
    }

    if (this->audioEngineInitialized)
    {
        ma_engine_uninit(&this->currentAudioEngine);
        this->audioEngineInitialized = false;
    }
}
} // namespace Proton
