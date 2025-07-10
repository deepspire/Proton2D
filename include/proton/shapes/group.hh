#pragma once
#include "shape.hh"
#include <vector>

namespace Proton
{
class Group : public Shape
{
  public:
    explicit Group(const float x, const float y);

    void paint(SDL_Renderer *render, const float gX, const float gY) override;

    void setRotation(const float angle) override { this->rotation = angle; }

    void setFillColor(Color /*unused*/) override {}

    void update(const float dt) override;

    void addObject(Shape *shape) { shapes.push_back(shape); }

    void clear() const;

    [[nodiscard]] auto getW() const -> float override { return 0.0f; }
    [[nodiscard]] auto getH() const -> float override { return 0.0f; }

    ~Group() override;

  protected:
    std::vector<Shape *> shapes;
};
} // namespace Proton