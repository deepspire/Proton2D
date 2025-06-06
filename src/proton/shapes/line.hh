#pragma once
#include "shape.hh"

namespace Proton
{
  class Line : public Shape
  {
  public:
    Line(SDL_Renderer *render, int x1 = 0, int y1 = 0, int x2 = 5, int y2 = 5, [[maybe_unused]] Color color = Color())
    {
      this->x = x1;
      this->y = y1;
      this->endX = x2;
      this->endY = y2;
      this->render = render;
      this->isVisible = true;
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

    void paint() override
    {
      SDL_SetRenderDrawColor(this->render, fillColor.getR(), fillColor.getG(),
                             fillColor.getB(), fillColor.getA());
      SDL_RenderLine(this->render, this->x, this->y, this->endX, this->endY);
    }

    void setFillColor(Color color) override { this->fillColor = color; }

  private:
    Color fillColor;
    float x, y, endX, endY;
  };
}