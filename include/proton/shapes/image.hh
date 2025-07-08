#pragma once

#include "../logman.hh"
#include "shape.hh"

namespace Proton
{
class Image : public Shape
{
  public:
    explicit Image(SDL_Texture *texture, const float x = 0, const float y = 0, const float width = 0,
                   const float height = 0)
    {
        if (texture == nullptr)
        {
            Log("Invalid texture or didn't loaded successfully");
            return;
        }
        this->imageTexture = texture;

        this->width = width;
        this->height = height;
        this->isVisible = true;
        this->position.x = x;
        this->position.y = y;
        if (width == 0)
            this->width = imageTexture->w;
        if (height == 0)
            this->height = imageTexture->h;
    }

    ~Image() override = default;

    void setFillColor([[maybe_unused]] Color c) override {}

    void resize(const int width, const int height)
    {
        this->width = width;
        this->height = height;
    }

    void setPosition(const float x, const float y) override
    {
        this->position.x = x;
        this->position.y = y;
    }

    void paint(SDL_Renderer *render, const float rX, const float rY) override
    {
        const float drawX = rX + this->position.x;
        const float drawY = rY + this->position.y;

        const SDL_FRect rectToRender = {drawX, drawY, static_cast<float>(this->width),
                                        static_cast<float>(this->height)};

        SDL_RenderTextureRotated(render, imageTexture, nullptr, &rectToRender, rotation, nullptr, SDL_FLIP_NONE);
    }

    void setRotation(const float angle) override { this->rotation = angle; }

    void setTexture(SDL_Texture *texture)
    {
        this->imageTexture = texture;
        this->width = this->imageTexture->w;
        this->height = this->imageTexture->h;
    }

  private:
    float width, height;
    SDL_Texture *imageTexture;
};
} // namespace Proton