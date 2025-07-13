#include "proton/shapes/rectangle.hh"

namespace Proton
{
Rectangle::Rectangle(const float x, float y, float w, float h, const Color color, Style style)
    : w(w), h(h), color(color), style(style)
{
    this->position.x = x;
    this->position.y = y;
}

Rectangle::~Rectangle()
{
    if (texture)
        SDL_DestroyTexture(texture);
}

void Rectangle::setFillColor(const Color newColor)
{
    color = newColor;
    dirty = true;
}

void Rectangle::resize(const float width, const float height)
{
    if (w != width || h != height)
    {
        w = width;
        h = height;
        dirty = true;
    }
}

void Rectangle::setStyle(const Style newStyle)
{
    if (style != newStyle)
    {
        style = newStyle;
        dirty = true;
    }
}

void Rectangle::paint(SDL_Renderer *renderer, const float rX, const float rY)
{
    if (!texture || dirty)
        this->rebuildTexture(renderer);

    if (!texture)
        return;

    const SDL_FRect dst = {rX + this->position.x, rY + this->position.y, w, h};
    SDL_RenderTextureRotated(renderer, texture, nullptr, &dst, rotation, nullptr, SDL_FLIP_NONE);
}
void Rectangle::rebuildTexture(SDL_Renderer *renderer)
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, static_cast<int>(this->w),
                                static_cast<int>(this->h));
    if (!texture)
        return;

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);

    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());

    const SDL_FRect rect = {0, 0, w, h};
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
} // namespace Proton