#include "proton/shapes/buttonarea.hh"

namespace Proton
{
ButtonArea::ButtonArea(const float x, const float y, const float w, const float h)
{
    this->position.x = x;
    this->position.y = y;
    this->w = w;
    this->h = h;
}

void ButtonArea::setPosition(const float x, const float y)
{
    this->position.x = x;
    this->position.y = y;
}

void ButtonArea::resize(const float w, const float h)
{
    this->w = w;
    this->h = h;
}

void ButtonArea::onClick()
{
    if (clickListener)
        clickListener();
}
void ButtonArea::setClickListener(void (*f)()) { this->clickListener = f; }
} // namespace Proton