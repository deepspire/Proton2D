#pragma once

#include "shape.hh"
#include <cmath>

namespace Proton
{
class Line : public Shape
{
  public:
    explicit Line(float x1 = 0, float y1 = 0, float x2 = 5, float y2 = 5, Color color = Color())
        : endX(x2), endY(y2), fillColor(color)
    {
        this->x = x1;
        this->y = y1;
    }

    void setRotation(float angle) override { this->rotation = angle; }

    void setPosition(const float x, const float y) override
    {
        float dx = endX - this->x;
        float dy = endY - this->y;
        this->x = x;
        this->y = y;
        this->endX = x + dx;
        this->endY = y + dy;
    }

    void setFillColor(Color color) override { this->fillColor = color; }

    void paint(SDL_Renderer *render, float rX, float rY) override
    {
        SDL_SetRenderDrawColor(render, fillColor.getR(), fillColor.getG(), fillColor.getB(), fillColor.getA());

        const float dx = endX - x;
        const float dy = endY - y;

        const float rad = rotation * (M_PI / 180.0f);
        const float rotatedDX = dx * std::cos(rad) - dy * std::sin(rad);
        const float rotatedDY = dx * std::sin(rad) + dy * std::cos(rad);

        const float startX = x + rX;
        const float startY = y + rY;
        const float endRotatedX = x + rotatedDX + rX;
        const float endRotatedY = y + rotatedDY + rY;

        SDL_RenderLine(render, startX, startY, endRotatedX, endRotatedY);
    }

  private:
    Color fillColor;
    float endX, endY;
};
} // namespace Proton
