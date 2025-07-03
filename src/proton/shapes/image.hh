#pragma once

#include "../logman.hh"
#include "shape.hh"

namespace Proton
{
  class Image : public Shape
  {
  public:
    explicit Image(SDL_Texture* texture, const int x = 0, const int y = 0,
                   const int width = 0, const int height = 0)
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
      this->x = x;
      this->y = y;
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

    void setPosition(const int x, const int y) override
    {
      this->x = x;
      this->y = y;
    }

    void paint(SDL_Renderer *render, const int rX, const int rY) override
    {
      const auto drawX = static_cast<float>(rX + this->x);
      const auto drawY = static_cast<float>(rY + this->y);

      const SDL_FRect rectToRender = {drawX, drawY, static_cast<float>(this->width), static_cast<float>(this->height)};

      SDL_RenderTexture(render, imageTexture, nullptr, &rectToRender);
    }

    void setTexture(SDL_Texture *texture)
    {
      this->imageTexture = texture;
      this->width = this->imageTexture->w;
      this->height = this->imageTexture->h;
    }

  private:
    int width, height;
    SDL_Texture *imageTexture;
  };
}