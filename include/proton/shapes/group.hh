#pragma once
#include "shape.hh"
#include <vector>

namespace Proton
{
class Group final : public Shape
{
  public:
    Group(const float x, const float y)
    {
        this->x = x;
        this->y = y;
    }

    void paint(SDL_Renderer *render, const float gX, const float gY) override
    {
        for (Shape *shape : this->shapes)
        {
            const float originalRotation = shape->getRotation();
            shape->setRotation(originalRotation + this->rotation);
            shape->paint(render, this->x + gX, this->y + gY);
            shape->setRotation(originalRotation);
        }
    }

    void setRotation(const float angle) override { this->rotation = angle; }

    void setFillColor(Color /*unused*/) override {}

    void update(const float dt) override
    {
        for (Shape *shape : this->shapes)
        {
            shape->update(dt);
        }
    }

    void setPosition(const float x, const float y) override
    {
        this->x = x;
        this->y = y;
    }

    void addObject(Shape *shape) { shapes.push_back(shape); }

    void clear() const
    {
        for (const Shape *shape : this->shapes)
        {
            delete shape;
        }
    }

    ~Group() override { clear(); }

  protected:
    std::vector<Shape *> shapes;
};
} // namespace Proton