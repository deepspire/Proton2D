#pragma once

#include "shape.hh"

namespace Proton
{
  class Line : public Shape
  {
  public:
    explicit Line(const float x1 = 0, const float y1 = 0, const float x2 = 5, const float y2 = 5, const Color color = Color())
    {
      this->x = x1;
      this->y = y1;
      this->endX = x2;
      this->endY = y2;
      this->fillColor = color;
    };

    void setPosition(const float x, const float y) override
    {
      const int dx = this->endX - this->x;
      const int dy = this->endY - this->y;
      this->x = x;
      this->y = y;
      this->endX = dx + x;
      this->endY = dy + y;
    }

    void paint(SDL_Renderer *render, const int rX, const int rY) override
    {
      SDL_SetRenderDrawColor(render, fillColor.getR(), fillColor.getG(),
                             fillColor.getB(), fillColor.getA());
      SDL_RenderLine(render, this->x + rX, this->y + rY,
                     this->endX + rX, this->endY + rY);
    }

    void setFillColor(const Color color) override { this->fillColor = color; }

  private:
    Color fillColor;
    float x, y, endX, endY;
  };
}