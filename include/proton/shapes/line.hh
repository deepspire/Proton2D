#pragma once

#include "shape.hh"

namespace Proton
{
class Line : public Shape
{
  public:
    explicit Line(float x1 = 0, float y1 = 0, float x2 = 5, float y2 = 5, Color color = Color());

    void setRotation(float angle) override { this->rotation = angle; }
    void setPosition(const float x, const float y) override;
    void setFillColor(Color color) override { this->fillColor = color; }
    void paint(SDL_Renderer *render, float rX, float rY) override;

    [[nodiscard]] auto getW() const -> float override;
    [[nodiscard]] auto getH() const -> float override;

  private:
    Color fillColor;
    float endX, endY;
};
} // namespace Proton
