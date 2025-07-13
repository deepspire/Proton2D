#include "proton/shapes/line.hh"
#include <cmath>

namespace Proton
{
Line::Line(float x1, float y1, float x2, float y2, Color color) : endX(x2), endY(y2), fillColor(color)
{
    this->position.x = x1;
    this->position.y = y1;
}

void Line::setPosition(const float x, const float y)
{
    float dx = endX - this->position.x;
    float dy = endY - this->position.y;
    this->position.x = x;
    this->position.y = y;
    this->endX = x + dx;
    this->endY = y + dy;
}

void Line::paint(SDL_Renderer *render, float rX, float rY)
{
    SDL_SetRenderDrawColor(render, fillColor.getR(), fillColor.getG(), fillColor.getB(), fillColor.getA());

    const float dx = endX - this->position.x;
    const float dy = endY - this->position.y;

    const float rad = rotation * (static_cast<float>(M_PI) / 180.0f);
    const float rotatedDX = dx * std::cos(rad) - dy * std::sin(rad);
    const float rotatedDY = dx * std::sin(rad) + dy * std::cos(rad);

    const float startX = this->position.x + rX;
    const float startY = this->position.y + rY;
    const float endRotatedX = this->position.x + rotatedDX + rX;
    const float endRotatedY = this->position.y + rotatedDY + rY;

    SDL_RenderLine(render, startX, startY, endRotatedX, endRotatedY);
}

[[nodiscard]] auto Line::getW() const -> float { return std::abs(this->endX - this->position.x); }

[[nodiscard]] auto Line::getH() const -> float { return std::abs(this->endY - this->position.y); }
} // namespace Proton