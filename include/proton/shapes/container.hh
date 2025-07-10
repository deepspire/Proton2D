#pragma once

#include "shape.hh"
#include <vector>

namespace Proton
{
class Container : public Shape
{
  public:
    explicit Container(const float x, const float y, const int width, const int height);

    void paint(SDL_Renderer *render, const float rX, const float rY) override;

    void setFillColor(Color /*unused*/) override {}

    void addObject(Shape *shape) { shapes.push_back(shape); }

    void clear();

    void setPosition(float x, float y) override;

    void update(const float dt) override;

    [[nodiscard]] auto getW() const -> float override { return this->containerRect.w; }
    [[nodiscard]] auto getH() const -> float override { return this->containerRect.h; }

    ~Container() override { this->clear(); }

  private:
    SDL_FRect containerRect{};
    std::vector<Shape *> shapes;
};
} // namespace Proton
