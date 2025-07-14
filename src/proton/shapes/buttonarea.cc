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

void ButtonArea::onClick() const
{
    if (clickListener)
        clickListener();
}

void ButtonArea::onClickEnded() const
{
    if (unclickListener)
        unclickListener();
}

auto ButtonArea::containsPoint(const float x, const float y) const -> bool {
    return (this->getX() <= x && x <= this->getX() + this->getW()) &&
            (this->getY() <= y && y <= this->getY() + this->getH());
}

void ButtonArea::setClickListener(const std::function<void()> &f) { this->clickListener = f; }

void ButtonArea::setClickEndedListener(const std::function<void()> &f) { this->unclickListener = f; }
auto ButtonArea::getIsFocused() const -> bool {
    return this->isFocused;
}
void ButtonArea::setIsFocused(const bool a)
{
    this->isFocused = a;
}
} // namespace Proton