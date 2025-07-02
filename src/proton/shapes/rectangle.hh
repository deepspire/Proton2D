#pragma once

#include <SDL3/SDL.h>
#include <SDL3_gfx/SDL3_gfxPrimitives.h>
#include "shape.hh"
#include "../resourcemanager.hh"

namespace Proton
{

  class Rectangle : public Shape
  {
  public:
    enum Style
    {
      Fill,
      Bevel
    };

    Rectangle(int x = 0, int y = 0, int w = 10, int h = 10,
              Color color = Color(), Sint16 roundness = 0, Style style = Style::Fill)
        : x(x), y(y), w(w), h(h), color(color), roundness(roundness), style(style), roundedTexture(nullptr), lastW(0), lastH(0), lastRoundness(0)
    {
    }

    ~Rectangle()
    {
      if (roundedTexture)
        SDL_DestroyTexture(roundedTexture);
    }

    void paint(SDL_Renderer *render, int rX, int rY) override
    {
      float drawX = static_cast<float>(rX + this->x);
      float drawY = static_cast<float>(rY + this->y);
      SDL_FRect rectToRender = {drawX, drawY, static_cast<float>(this->w), static_cast<float>(this->h)};

      SDL_SetRenderDrawColor(render, color.getR(), color.getG(), color.getB(), color.getA());

      switch (this->style)
      {
      case Style::Fill:
      {
        if (roundness == 0)
        {
          SDL_RenderFillRect(render, &rectToRender);
        }
        else
        {
          int supersample = 24;
          int texW = this->w * supersample;
          int texH = this->h * supersample;

          SDL_Texture *texture = ResourceManager::getInstance().getRoundedRectTexture(render, texW, texH, this->roundness, color.getR(), color.getG(), color.getB(), color.getA());
          //texture = getRoundedTexture(render, texW, texH);
          if (texture)
          {
            SDL_RenderTexture(render, texture, nullptr, &rectToRender);
          }
        }
        break;
      }
      case Style::Bevel:
      {
        if (roundness == 0)
        {
          SDL_RenderRect(render, &rectToRender);
        }
        else
        {
          roundedRectangleRGBA(render, drawX, drawY, drawX + this->w, drawY + this->h,
                               this->roundness, color.getR(), color.getG(), color.getB(), color.getA());
        }
        break;
      }
      }
    }

    void setFillColor(Color color) override
    {
      this->color = color;
      if (roundedTexture)
      {
        SDL_DestroyTexture(roundedTexture);
        roundedTexture = nullptr;
      }
    }

    void resize(int width, int height)
    {
      if (this->w != width || this->h != height)
      {
        this->w = width;
        this->h = height;
        if (roundedTexture)
        {
          SDL_DestroyTexture(roundedTexture);
          roundedTexture = nullptr;
        }
      }
    }

    void setPosition(int x, int y) override
    {
      this->x = x;
      this->y = y;
    }

    Style getStyle() const
    {
      return this->style;
    }

  private:
    Color color;
    int x, y, w, h;
    int lastW, lastH;
    Sint16 roundness, lastRoundness;
    Color lastColor;
    SDL_Texture *roundedTexture;
    Style style;

    SDL_Texture *getRoundedTexture(SDL_Renderer *render, int texW, int texH)
    {
      if (!roundedTexture || texW != lastW || texH != lastH || roundness != lastRoundness)
      {
        if (roundedTexture)
          SDL_DestroyTexture(roundedTexture);

        roundedTexture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_TARGET, texW, texH);
        if (!roundedTexture)
        {
          Proton::Log("Failed to create rounded rectangle texture: ", SDL_GetError());
          return nullptr;
        }

        SDL_SetTextureBlendMode(roundedTexture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureScaleMode(roundedTexture, SDL_SCALEMODE_LINEAR);

        SDL_SetRenderTarget(render, roundedTexture);
        SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
        SDL_RenderClear(render);

        roundedBoxRGBA(render, 0, 0, texW - 1, texH - 1,
                       this->roundness * 4,
                       color.getR(), color.getG(), color.getB(), color.getA());

        SDL_SetRenderTarget(render, nullptr);

        lastW = texW;
        lastH = texH;
        lastRoundness = roundness;
        lastColor = color;
      }

      return roundedTexture;
    }
  };
}