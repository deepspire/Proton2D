#pragma once

#include "shape.hh"
#include <SDL3/SDL.h>
#include <cmath>


namespace Proton
{
class Circle : public Shape
{
  public:
    enum Style
    {
        Fill,
        Bevel
    };

    explicit Circle(float x = 0, float y = 0, int radius = 40, Color fillColor = Color(), Style style = Fill)
        : radius(radius), color(fillColor), style(style), texture(nullptr), dirty(true)
    {
        this->x = x;
        this->y = y;
    }

    ~Circle() override
    {
        if (texture)
            SDL_DestroyTexture(texture);
    }

    void setPosition(float x, float y) override
    {
        this->x = x;
        this->y = y;
    }

    void setFillColor(Color newColor) override
    {
        color = newColor;
        dirty = true;
    }

    void setStyle(Style s)
    {
        if (s != style)
        {
            style = s;
            dirty = true;
        }
    }

    void paint(SDL_Renderer *render, float rX, float rY) override
    {
        if (!texture || dirty)
            rebuildTexture(render);

        if (!texture)
            return;

        SDL_FRect dst = {x + rX - radius, y + rY - radius, static_cast<float>(2 * radius),
                         static_cast<float>(2 * radius)};
        SDL_RenderTextureRotated(render, texture, nullptr, &dst, getRotation(), nullptr, SDL_FLIP_NONE);
    }

  private:
    int radius;
    Color color;
    Style style;
    SDL_Texture *texture;
    bool dirty;

    void rebuildTexture(SDL_Renderer *render)
    {
        if (texture)
        {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }

        const int diameter = 2 * radius;
        texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, diameter, diameter);
        if (!texture)
            return;

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        // SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);
        SDL_SetRenderTarget(render, texture);
        SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
        SDL_RenderClear(render);

        SDL_SetRenderDrawColor(render, color.getR(), color.getG(), color.getB(), color.getA());

        switch (style)
        {
        case Bevel:
            for (int angle = 0; angle < 360; ++angle)
            {
                const float rad = angle * M_PI / 180.0f;
                const float dx = radius + radius * std::cos(rad);
                const float dy = radius + radius * std::sin(rad);
                SDL_RenderPoint(render, dx, dy);
            }
            break;

        case Fill:
        default:
            for (int dy = -radius; dy <= radius; ++dy)
            {
                const double dx = std::sqrt(radius * radius - dy * dy);
                SDL_RenderLine(render, radius - dx, radius + dy, radius + dx, radius + dy);
            }
            break;
        }

        SDL_SetRenderTarget(render, nullptr);
        dirty = false;
    }
};
} // namespace Proton
