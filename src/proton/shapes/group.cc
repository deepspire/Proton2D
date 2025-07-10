#include "proton/shapes/group.hh"

namespace Proton
{
Group::Group(const float x, const float y)
{
    this->position.x = x;
    this->position.y = y;
}
Group::~Group() { this->clear(); }

void Group::paint(SDL_Renderer *render, const float gX, const float gY)
{
    for (Shape *shape : this->shapes)
    {
        const float originalRotation = shape->getRotation();
        shape->setRotation(originalRotation + this->rotation);
        shape->paint(render, this->position.x + gX, this->position.y + gY);
        shape->setRotation(originalRotation);
    }
}

void Group::update(const float dt)
{
    for (Shape *shape : this->shapes)
    {
        shape->update(dt);
    }
}

void Group::clear() const
{
    for (const Shape *shape : this->shapes)
    {
        delete shape;
    }
}
} // namespace Proton