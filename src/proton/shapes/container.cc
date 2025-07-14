#include "proton/shapes/container.hh"
#include <algorithm>

namespace Proton
{
Container::Container(const float x, const float y, const int width, const int height)
{
    this->position.x = x;
    this->position.y = y;

    this->containerRect = {x, y, static_cast<float>(width), static_cast<float>(height)};
    this->calculateContentBounds();
}
Container::~Container() { this->clear(); }

void Container::paint(SDL_Renderer *render, const float rX, const float rY)
{
    const SDL_Rect absoluteClipRect = {static_cast<int>(this->position.x + rX), static_cast<int>(this->position.y + rY),
                                       static_cast<int>(this->containerRect.w),
                                       static_cast<int>(this->containerRect.h)};

    SDL_SetRenderClipRect(render, &absoluteClipRect);
    for (Shape *shape : this->shapes)
    {
        shape->setRotation(this->rotation);
        shape->paint(render, this->position.x + rX - this->scrollX, this->position.y + rY - this->scrollY);
    }
    SDL_SetRenderClipRect(render, nullptr);
}

auto Container::containsPoint(const float x, const float y) const -> bool {
    return (this->getX() <= x && x <= this->getX() + this->getW()) &&
            (this->getY() <= y && y <= this->getY() + this->getH());
}

void Container::clear()
{
    for (const Shape *shape : this->shapes)
    {
        delete shape;
    }
    shapes.clear();
    buttons.clear();
    this->calculateContentBounds();
}

void Container::setPosition(float x, float y)
{
    this->position.x = x;
    this->position.y = y;
    this->containerRect = {x, y, this->containerRect.w, this->containerRect.h};
}

void Container::update(const float dt)
{
    for (Shape *shape : this->shapes)
    {
        shape->update(dt);
    }

    for (ButtonArea *button : this->buttons)
    {
        button->update(dt);
    }
}

void Container::scrollBy(float dx, float dy)
{
    this->scrollX += dx;
    this->scrollY += dy;

    float maxScrollX = /* std::max(0.0f, contentWidth - this->containerRect.w); */ 1000;
    float maxScrollY = /* std::max(0.0f, contentHeight - this->containerRect.h); */ 1000;

    if (this->scrollX < 0)
        this->scrollX = 0;
    if (this->scrollX > maxScrollX)
        this->scrollX = maxScrollX;

    if (this->scrollY < 0)
        this->scrollY = 0;
    if (this->scrollY > maxScrollY)
        this->scrollY = maxScrollY;
}

void Container::calculateContentBounds()
{
    contentWidth = 0.0f;
    contentHeight = 0.0f;

    if (shapes.empty()) return;

    for (const auto& shape : shapes)
    {
        contentWidth = std::max(contentWidth, shape->getX() + shape->getW());
        contentHeight = std::max(contentHeight, shape->getY() + shape->getH());
    }
}
} // namespace Proton