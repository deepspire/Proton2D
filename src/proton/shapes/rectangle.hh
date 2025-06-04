#pragma once
#include "shape.hh"

namespace Proton
{
  class Rectangle : public Shape
  {
  public:
    Rectangle(SDL_Renderer *render, int x = 0, int y = 0, int w = 10, int h = 10,
              Color color = Color())
    {
      this->x = x;
      this->y = y;
      this->color = color;
      this->w = w;
      this->h = h;
      this->render = render;
      redefineRect();
    }

    void paint() override
    {
      SDL_SetRenderDrawColor(this->render, color.getR(), color.getG(), color.getB(),
                             color.getA());
      SDL_RenderFillRect(this->render, &this->rect);
    }

    void setFillColor(Color color) override { this->color = color; }

    void resize(int width, int height)
    {
      this->w = width;
      this->h = height;
      redefineRect();
    }

    void setPosition(int x, int y) override
    {
      this->x = x;
      this->y = y;
      redefineRect();
    }

  private:
    void redefineRect()
    {
      rect = {(float)this->x, (float)this->y, (float)this->w, (float)this->h};
    }

    Color color;
    int x, y, w, h;
    SDL_FRect rect;
  };
}