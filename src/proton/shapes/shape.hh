#pragma once
#include <SDL3/SDL.h>

namespace Proton
{
  class Color
  {
  public:
    Color(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255)
    {
      this->r = r;
      this->g = g;
      this->b = b;
      this->a = a;
    }

    unsigned char getR() { return r; }
    unsigned char getG() { return g; }
    unsigned char getB() { return b; }
    unsigned char getA() { return a; }

    void setR(unsigned char v)
    {
      this->r = v;
    }

    void setG(unsigned char v)
    {
      this->g = v;
    }

    void setB(unsigned char v)
    {
      this->b = v;
    }

    void setA(unsigned char v)
    {
      this->a = v;
    }

  private:
    unsigned char r, g, b, a;
  };

  class Shape
  {
  public:
    virtual void paint(SDL_Renderer *randr) = 0;
    virtual void setFillColor(Color color) = 0;
    virtual void setPosition(int x, int y) = 0;
    virtual ~Shape() {}
  };
}