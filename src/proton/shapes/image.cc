#include "proton/shapes/image.hh"
#include "SDL3/SDL_render.h"
#include "proton/logman.hh"

namespace Proton
{
Image::Image(SDL_Texture *texture, const float x, const float y, const float width, const float height)
{
    if (texture == nullptr)
    {
        LogNew(Error, "Invalid texture or didn't loaded successfully");
        return;
    }
    this->imageTexture = texture;
    SDL_SetTextureBlendMode(this->imageTexture, SDL_BLENDMODE_BLEND);

    this->width = width;
    this->height = height;
    this->position.x = x;
    this->position.y = y;

    if (width == 0)
        this->width = static_cast<float>(this->imageTexture->w);
    if (height == 0)
        this->height = static_cast<float>(this->imageTexture->h);
}

void Image::resize(const float width, const float height)
{
    this->width = width;
    this->height = height;
}

void Image::paint(SDL_Renderer *render, const float rX, const float rY)
{
    const float drawX = rX + this->position.x;
    const float drawY = rY + this->position.y;

    const SDL_FRect rectToRender = {drawX, drawY, this->width, this->height};

    SDL_SetTextureAlphaModFloat(this->imageTexture, this->alpha);
    SDL_RenderTextureRotated(render, imageTexture, nullptr, &rectToRender, rotation, nullptr, SDL_FLIP_NONE);
    SDL_SetTextureAlphaModFloat(this->imageTexture, 1);
}

void Image::setTexture(SDL_Texture *texture)
{
    this->imageTexture = texture;
    this->width = static_cast<float>(this->imageTexture->w);
    this->height = static_cast<float>(this->imageTexture->h);
    SDL_SetTextureBlendMode(this->imageTexture, SDL_BLENDMODE_BLEND);
}

void Image::setAlpha(float alpha)
{
    if (!this->imageTexture) return;
    this->alpha = alpha;
}

auto Image::getAlpha() -> float
{
    return this->alpha;
}
} // namespace Proton