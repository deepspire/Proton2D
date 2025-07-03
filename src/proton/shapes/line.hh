#pragma once

#include "shape.hh"

namespace Proton
{
  class Line : public Shape
  {
  public:
    Line(int x1 = 0, int y1 = 0, int x2 = 5, int y2 = 5, Color color = Color())
    {
      this->x = x1;
      this->y = y1;
      this->endX = x2;
      this->endY = y2;
      this->fillColor = color;
    };

    void setPosition(int x, int y) override
    {
      int dx = this->endX - this->x;
      int dy = this->endY - this->y;
      this->x = x;
      this->y = y;
      this->endX = dx + x;
      this->endY = dy + y;
    }

    void paint(SDL_Renderer *render, int rX, int rY) override
    {

      Sint16 x1 = (Sint16)this->x + rX;
      Sint16 y1 = (Sint16)this->y + rY;
      Sint16 x2 = (Sint16)this->endX + rX;
      Sint16 y2 = (Sint16)this->endY + rY;
      SDL_SetRenderDrawColor(render, fillColor.getR(), fillColor.getG(),
                             fillColor.getB(), fillColor.getA());
      SDL_RenderLine(render, this->x + rX, this->y + rY,
                     this->endX + rX, this->endY + rY);
    }

    void setFillColor(Color color) override { this->fillColor = color; }

  private:
    Color fillColor;
    float x, y, endX, endY;
  };
}