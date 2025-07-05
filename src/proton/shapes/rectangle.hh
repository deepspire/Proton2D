#pragma once

#include <SDL3/SDL.h>
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

    explicit Rectangle(const float x = 0, const float y = 0, const int w = 10, const int h = 10,
              const Color color = Color(), Sint16 roundness = 0, const Style style = Fill)
        : color(color), w(w), h(h), style(style)
    {
      this->x = x;
      this->y = y;
    }

    ~Rectangle() override = default;

    void paint(SDL_Renderer *render, const float rX, const float rY) override
    {
      const float drawX = rX + this->x;
      const float drawY = rY + this->y;
      const SDL_FRect rectToRender = {drawX, drawY, static_cast<float>(this->w), static_cast<float>(this->h)};

      SDL_SetRenderDrawColor(render, color.getR(), color.getG(), color.getB(), color.getA());

      switch (this->style)
      {
      case Fill:
      {
        SDL_RenderFillRect(render, &rectToRender);
        break;
      }
      case Bevel:
      {
        SDL_RenderRect(render, &rectToRender);
        break;
      }
      }
    }

    void setFillColor(const Color color) override
    {
      this->color = color;
    }

    void resize(const int width, const int height)
    {
      if (this->w != width || this->h != height)
      {
        this->w = width;
        this->h = height;
      }
    }

    void setPosition(const float x, const float y) override
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
    int w, h;
    Color lastColor;
    Style style;
  };
}