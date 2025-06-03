#pragma once
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include "../logman.hh"
#include "shape.hh"

namespace Proton
{
  class Image : public Shape
  {
  public:
    Image(SDL_Renderer *randr, const char *imagePath, int x = 0, int y = 0,
          int width = 0, int height = 0)
    {
      SDL_Surface *image = IMG_Load(imagePath);
      if (image == nullptr)
      {
        Proton::Log("Image wasn't loaded: ", SDL_GetError());
        return;
      }

      this->width = width;
      this->height = height;
      this->path = imagePath;
      this->randr = randr;
      this->x = x;
      this->y = y;
      if (width == 0)
        this->width = image->w;
      if (height == 0)
        this->height = image->h;
      this->recreateBounds();

      this->imageTexture = SDL_CreateTextureFromSurface(this->randr, image);
      if (this->imageTexture == nullptr)
      {
        Proton::Log("Image texture wasn't loaded", SDL_GetError());
        return;
      }
      SDL_DestroySurface(image);
      
      std::cout << "Image " << imagePath << " exported to engine" << std::endl;
    }

    ~Image()
    {
      std::cout << "Image " << this->path << " unloaded" << std::endl;
      SDL_DestroyTexture(imageTexture);
    }

    void setFillColor(Color c) override {}

    void resize(int width, int height)
    {
      this->width = width;
      this->height = height;
      this->recreateBounds();
    }

    void setPosition(int x, int y) override
    {
      this->x = x;
      this->y = y;
      this->recreateBounds();
    }

    void paint(SDL_Renderer * /*unused*/) override
    {
      SDL_RenderTexture(this->randr, imageTexture, NULL, &this->bounds);
    }

  private:
    void recreateBounds()
    {
      this->bounds = {(float)this->x, (float)this->y, (float)this->width, (float)this->height};
    }

    int x, y, width, height;
    SDL_FRect bounds;
    const char *path;
    SDL_Texture *imageTexture;
    SDL_Renderer *randr;
  };
}