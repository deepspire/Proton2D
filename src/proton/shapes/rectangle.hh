#pragma once
#include "shape.hh"

namespace Proton
{
  class Rectangle : public Shape
  {
  public:
    Rectangle(int x = 0, int y = 0, int w = 10, int h = 10,
              Color color = Color(), std::string style="fill")
    {
      this->x = x;
      this->y = y;
      this->color = color;
      this->w = w;
      this->h = h;
      this->isVisible = true;
      this->style = style;
    }

    void paint(SDL_Renderer *render, int rX, int rY) override
    {
      float drawX = static_cast<float>(rX + this->x);
      float drawY = static_cast<float>(rY + this->y);

      SDL_FRect rectToRender = {drawX, drawY, (float)this->w, (float)this->h};

      SDL_SetRenderDrawColor(render, color.getR(), color.getG(), color.getB(), color.getA());
      if(this->style == "fill")
        SDL_RenderFillRect(render, &rectToRender);
      else if(this->style == "bevel")
        SDL_RenderRect(render, &rectToRender);
    }

    void setFillColor(Color color) override { this->color = color; }

    void resize(int width, int height)
    {
      this->w = width;
      this->h = height;
    }

    void setPosition(int x, int y) override
    {
      this->x = x;
      this->y = y;
    }

  private:
    Color color;
    int w, h;
    std::string style;
  };
}