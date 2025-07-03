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

    Rectangle(int x = 0, int y = 0, int w = 10, int h = 10,
              Color color = Color(), Sint16 roundness = 0, Style style = Style::Fill)
        : x(x), y(y), w(w), h(h), color(color), style(style)
    {
    }

    ~Rectangle()
    {
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
        SDL_RenderFillRect(render, &rectToRender);
        break;
      }
      case Style::Bevel:
      {
        SDL_RenderRect(render, &rectToRender);
        break;
      }
      }
    }

    void setFillColor(Color color) override
    {
      this->color = color;
    }

    void resize(int width, int height)
    {
      if (this->w != width || this->h != height)
      {
        this->w = width;
        this->h = height;
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
    Color lastColor;
    Style style;
  };
}