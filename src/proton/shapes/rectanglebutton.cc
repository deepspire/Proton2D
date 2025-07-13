#include "proton/shapes/rectanglebutton.hh"

namespace Proton
{
RectangleButton::RectangleButton(float x, float y, float w, float h, Color color)
    : ButtonArea(x, y, w, h)
{
    this->position.x = x;
    this->position.y = y;
    this->rectangle = new Rectangle(x, y, w, h, color);
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