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

void ButtonArea::onClick() const {
    if (clickListener)
        clickListener();
}

void ButtonArea::onClickEnded() const {
    if (unclickListener)
        unclickListener();
}

void ButtonArea::setClickListener(void (*f)()) { this->clickListener = f; }

void ButtonArea::setClickEndedListener(void(*f)()) {
    this->unclickListener = f;
}
} // namespace Proton