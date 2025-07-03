#pragma once
#include "shape.hh"
#include <vector>

namespace Proton
{
    class Group final : public Shape
    {
    public:
        Group(const int x, const int y)
        {
            this->x = x;
            this->y = y;
        }

        void paint(SDL_Renderer *render, const int gX, const int gY) override
        {
            for (Shape *shape : this->shapes)
            {
                shape->paint(render, this->x + gX, this->y + gY);
            }
        }

        void setFillColor(Color /*unused*/) override
        {
        }

        void update(const float dt) override
        {
            for (Shape *shape : this->shapes)
            {
                shape->update(dt);
            }
        }

        void setPosition(const int x, const int y) override
        {
            this->x = x;
            this->y = y;
        }

        void addObject(Shape *shape)
        {
            shapes.push_back(shape);
        }

        void clear() const {
            for (Proton::Shape *shape : this->shapes)
            {
                delete shape;
            }
        }

        ~Group() override {
            clear();
        }

    protected:
        std::vector<Shape *> shapes;
    };
}