#pragma once
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include "../logman.hh"
#include "shape.hh"
#include "../resourcemanager.hh"

namespace Proton
{
  class Image : public Shape
  {
  public:
    Image(SDL_Texture* texture, int x = 0, int y = 0,
          int width = 0, int height = 0)
    {
      if (texture == nullptr)
      {
        Proton::Log("у тебя два айкью????");
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

    ~Image()
    {
    }

    void setFillColor([[maybe_unused]] Color c) override {}

    void resize(int width, int height)
    {
      this->width = width;
      this->height = height;
    }

    void setPosition(int x, int y) override
    {
      this->x = x;
      this->y = y;
    }

    void paint(SDL_Renderer *render, int rX, int rY) override
    {
      float drawX = static_cast<float>(rX + this->x);
      float drawY = static_cast<float>(rY + this->y);

      SDL_FRect rectToRender = {drawX, drawY, (float)this->width, (float)this->height};

      SDL_RenderTexture(render, imageTexture, NULL, &rectToRender);
    }

  private:
    int width, height;
    SDL_Texture *imageTexture;
  };
}