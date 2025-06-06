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

    unsigned char getR() const { return this->r; }
    unsigned char getG() const { return this->g; }
    unsigned char getB() const { return this->b; }
    unsigned char getA() const { return this->a; }

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
    virtual void paint() = 0;
    virtual void paint(int x, int y) {} // if need to paint relative to anything
    virtual void setFillColor(Color color) = 0;
    virtual void setPosition(int x, int y) = 0;

    void setVisible(bool a) 
    {
      this->isVisible = a;
    }

    bool getVisible() const {
      return this->isVisible;
    }

    int getX() const {
      return this->x;
    }

    int getY() const {
      return this->y;
    }

    virtual void update(float dt) {}
    virtual ~Shape() {}
    
  protected:
    SDL_Renderer *render;
    bool isVisible;
    int x, y;
  };
}