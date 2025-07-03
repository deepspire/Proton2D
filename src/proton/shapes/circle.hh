#pragma once
#include "shape.hh"
#include <cmath>
#include <math.h>

namespace Proton
{
  class Circle : public Shape
  {
  public:
    Circle(int x = 0, int y = 0, int radius = 40, Color fillColor = Color())
    {
      this->x = x;
      this->y = y;
      this->radius = radius;
      this->color = fillColor;
    }

    void setPosition(int x, int y) override
    {
      this->x = x;
      this->y = y;
    }

    void paint(SDL_Renderer *render, int rX, int rY) override
    {
      SDL_SetRenderDrawColor(render, color.getR(), color.getG(), color.getB(), color.getA());
      for (int dy = -radius; dy <= radius; dy++)
      {
        int dx = (int)std::sqrt(radius * radius - dy * dy);
        SDL_RenderLine(render, (this->x + rX) - dx, (this->y + rY) + dy, (this->x + rX) + dx, (this->y + rY) + dy);
      }
    }

    void setFillColor(Color newColor) override { this->color = newColor; }

  private:
    Color color;
    int radius;
  };
}