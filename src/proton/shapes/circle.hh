#pragma once
#include "shape.hh"
#include <cmath>
#include <math.h>

namespace Proton
{
  class Circle : public Shape
  {
  public:
    enum Style {
      Fill,
      Bevel
    };
    explicit Circle(const float x = 0, const float y = 0, const int radius = 40, const Color fillColor = Color(), const Style style=Fill)
    {
      this->x = x;
      this->y = y;
      this->radius = radius;
      this->color = fillColor;
      this->style = style;
    }

    void setPosition(const float x, const float y) override
    {
      this->x = x;
      this->y = y;
    }

    void paint(SDL_Renderer *render, const int rX, const int rY) override
    {
      switch (this->style) {
        case Bevel: {
          SDL_SetRenderDrawColor(render, color.getR(), color.getG(), color.getB(), color.getA());
          for (int angle = 0; angle < 360; ++angle)
          {
            const float rad = angle * M_PI / 180.0f;
            const int dx = static_cast<int>(radius * std::cos(rad));
            const int dy = static_cast<int>(radius * std::sin(rad));
            SDL_RenderPoint(render, this->x + rX + dx, this->y + rY + dy);
          }
          break;
        }
        case Fill:
        default: {
          SDL_SetRenderDrawColor(render, color.getR(), color.getG(), color.getB(), color.getA());
          for (int dy = -radius; dy <= radius; dy++)
          {
            const int dx = static_cast<int>(std::sqrt(radius * radius - dy * dy));
            SDL_RenderLine(render, (this->x + rX) - dx, (this->y + rY) + dy, (this->x + rX) + dx, (this->y + rY) + dy);
          }
          break;
        }
      }
    }

    void setFillColor(const Color newColor) override { this->color = newColor; }

  private:
    Color color;
    int radius;
    Style style;
  };
}