#pragma once
#include "shape.hh"
#include <cmath>
#include <math.h>

namespace Proton
{
  class Circle : public Shape
  {
  public:
    Circle(SDL_Renderer *render, int x = 0, int y = 0, int radius = 40, Color fillColor = Color())
    {
      this->x = x;
      this->y = y;
      this->radius = radius;
      this->color = fillColor;
      this->render = render;
      this->isVisible = true;
    }

    void setPosition(int x, int y) override
    {
      this->x = x;
      this->y = y;
    }

    void paint() override
    {
      SDL_SetRenderDrawColor(this->render, color.getR(), color.getG(), color.getB(),
                             color.getA());
      for (int dy = -radius; dy <= radius; dy++)
      {
        int dx = (int)std::sqrt(radius * radius - dy * dy);
        SDL_RenderLine(this->render, x - dx, y + dy, x + dx, y + dy);
      }
    }

    void setFillColor(Color newColor) override { this->color = newColor; }

  private:
    Color color;
    int x, y, radius;
  };
}