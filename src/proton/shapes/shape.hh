#pragma once
#include <SDL3/SDL.h>

namespace Proton
{
  class Color
  {
  public:
    explicit Color(const unsigned char r = 0, const unsigned char g = 0, const unsigned char b = 0, const unsigned char a = 255)
    {
      this->r = r;
      this->g = g;
      this->b = b;
      this->a = a;
    }

    [[nodiscard]] unsigned char getR() const { return this->r; }
    [[nodiscard]] unsigned char getG() const { return this->g; }
    [[nodiscard]] unsigned char getB() const { return this->b; }
    [[nodiscard]] unsigned char getA() const { return this->a; }

    void setR(const unsigned char v)
    {
      this->r = v;
    }

    void setG(const unsigned char v)
    {
      this->g = v;
    }

    void setB(const unsigned char v)
    {
      this->b = v;
    }

    void setA(const unsigned char v)
    {
      this->a = v;
    }

  private:
    unsigned char r, g, b, a;
  };

  class Shape
  {
  public:
    virtual void paint(SDL_Renderer *render, float rX, float rY) = 0;
    virtual void setFillColor(Color color) = 0;
    virtual void setPosition(float x, float y) = 0;

    void setVisible(const bool a)
    {
      this->isVisible = a;
    }

    [[nodiscard]] bool getVisible() const
    {
      return this->isVisible;
    }

    [[nodiscard]] float getX() const
    {
      return this->x;
    }

    [[nodiscard]] float getY() const
    {
      return this->y;
    }

    virtual void update(float dt) {}
    virtual ~Shape() = default;

  protected:
    bool isVisible = true;
    float x = 0, y = 0;
  };
}