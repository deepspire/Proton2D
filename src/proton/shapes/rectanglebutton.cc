#include "proton/shapes/rectanglebutton.hh"

namespace Proton
{
RectangleButton::RectangleButton(float X, float Y, float W, float H, Color color)
    : ButtonArea(X, Y, W, H)
{
    this->rectangle = new Rectangle(X, Y, W, H, color);
}

void RectangleButton::setPosition(const float x, const float y)
{
    this->position.x = x;
    this->position.y = y;
    rectangle->setPosition(x, y);
}

void RectangleButton::resize(const float w, const float h)
{
    this->w = w;
    this->h = h;
    rectangle->resize(w, h);
}
} // namespace Proton