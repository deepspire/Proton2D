#include "proton/shapes/container.hh"

namespace Proton
{
Container::Container(const float x, const float y, const int width, const int height)
{
    this->position.x = x;
    this->position.y = y;

    this->containerRect = {x, y, static_cast<float>(width), static_cast<float>(height)};
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
        shape->paint(render, this->position.x + rX, this->position.y + rY);
    }
    SDL_SetRenderClipRect(render, nullptr);
}

void Container::clear()
{
    for (const Shape *shape : this->shapes)
    {
        delete shape;
    }
    shapes.clear();
    buttons.clear();
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
} // namespace Proton