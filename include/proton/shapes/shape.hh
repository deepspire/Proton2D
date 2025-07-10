#pragma once
#include <SDL3/SDL.h>

namespace Proton
{
  class PhysicsBody;
struct Point
{
    float x;
    float y;

    auto operator+(const Point &other) const -> Point { return {x + other.x, y + other.y}; }

    auto operator*(const float mul) const -> Point { return {x * mul, y * mul}; }
};

class Color
{
  public:
    explicit Color(const unsigned char r = 0, const unsigned char g = 0, const unsigned char b = 0,
                   const unsigned char a = 255)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    [[nodiscard]] auto getR() const -> unsigned char { return this->r; }
    [[nodiscard]] auto getG() const -> unsigned char { return this->g; }
    [[nodiscard]] auto getB() const -> unsigned char { return this->b; }
    [[nodiscard]] auto getA() const -> unsigned char { return this->a; }

    void setR(const unsigned char v) { this->r = v; }

    void setG(const unsigned char v) { this->g = v; }

    void setB(const unsigned char v) { this->b = v; }

    void setA(const unsigned char v) { this->a = v; }

  private:
    unsigned char r, g, b, a;
};

class Shape
{
  public:
    virtual void paint(SDL_Renderer *render, float rX, float rY) = 0;
    virtual void setFillColor(Color color) = 0;

    virtual void setPosition(float x, float y);

    void setVisible(const bool a) { this->isVisible = a; }

    [[nodiscard]] auto getVisible() const -> bool { return this->isVisible; }

    [[nodiscard]] auto getX() const -> float { return this->position.x; }
    [[nodiscard]] auto getY() const -> float { return this->position.y; }

    [[nodiscard]] virtual auto getW() const -> float = 0;
    [[nodiscard]] virtual auto getH() const -> float = 0;

    [[nodiscard]] auto getRotation() const -> float { return this->rotation; }

    virtual void setRotation(float angle);

    virtual void update(float dt) {}
    virtual ~Shape() = default;
    void setBody(PhysicsBody* body);

  private:
    PhysicsBody* body = nullptr;

  protected:
    bool isVisible = true;
    Point position = {0, 0};
    float rotation = 0;

};
} // namespace Proton