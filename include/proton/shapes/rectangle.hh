#pragma once

#include "shape.hh"
#include <SDL3/SDL.h>

namespace Proton
{

class Rectangle final : public Shape
{
  public:
    enum Style : uint8_t 
    {
        Fill,
        Bevel
    };

    explicit Rectangle(const float x = 0, float y = 0, int w = 10, int h = 10, const Color color = Color(),
                       Style style = Fill)
        : w(w), h(h), color(color), style(style)
    {
        this->position.x = x;
        this->position.y = y;
    }

    ~Rectangle() override
    {
        if (texture)
            SDL_DestroyTexture(texture);
    }

    void setFillColor(const Color newColor) override
    {
        color = newColor;
        dirty = true;
    }

    void resize(const int width, const int height)
    {
        if (w != width || h != height)
        {
            w = width;
            h = height;
            dirty = true;
        }
    }

    void setStyle(const Style newStyle)
    {
        if (style != newStyle)
        {
            style = newStyle;
            dirty = true;
        }
    }

    void setPosition(float x, float y) override
    {
        this->position.x = x;
        this->position.y = y;
    }

    [[nodiscard]] Style getStyle() const { return style; }

    void paint(SDL_Renderer *renderer, const float rX, const float rY) override
    {
        if (!texture || dirty)
            rebuildTexture(renderer);

        if (!texture)
            return;

        const SDL_FRect dst = {rX + this->position.x, rY + this->position.y, static_cast<float>(w), static_cast<float>(h)};
        SDL_RenderTextureRotated(renderer, texture, nullptr, &dst, rotation, nullptr, SDL_FLIP_NONE);
    }

  private:
    int w{}, h{};
    Color color{};
    Style style{};
    SDL_Texture *texture{};
    bool dirty{true};

    void rebuildTexture(SDL_Renderer *renderer)
    {
        if (texture)
        {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
        if (!texture)
            return;

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);

        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());

        const SDL_FRect rect = {0, 0, static_cast<float>(w), static_cast<float>(h)};
        switch (style)
        {
        case Fill:
            SDL_RenderFillRect(renderer, &rect);
            break;
        case Bevel:
            SDL_RenderRect(renderer, &rect);
            break;
        }

        SDL_SetRenderTarget(renderer, nullptr);
        dirty = false;
    }
};

} // namespace Proton
