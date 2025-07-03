#pragma once
#include "shape.hh"
#include <vector>
#include "../proton.hh"

namespace Proton
{
    class Container final : public Shape
    {
    public:
        explicit Container(const int x, const int y, const int width, const int height)
        {
            this->x = x;
            this->y = y;

            this->containerRect = {x, y, width, height};
        }

        void paint(SDL_Renderer *render, int rX, int rY) override
        {
            const SDL_Rect absoluteClipRect = {
                this->x + rX, this->y + rY,
                this->containerRect.w, this->containerRect.h};

            SDL_SetRenderClipRect(render, &absoluteClipRect);
            for (Shape *shape : this->shapes)
            {
                shape->paint(render, this->x + rX, this->y + rY);
            }
            SDL_SetRenderClipRect(render, nullptr);
        }

        void setFillColor(Color /*unused*/) override
        {
        }

        void addObject(Shape *shape)
        {
            shapes.push_back(shape);
        }

        void clear()
        {
            for (const Shape *shape : this->shapes)
            {
                delete shape;
            }
            shapes.clear();
        }

        void setPosition(int x, int y) override
        {
            this->x = x;
            this->y = y;
            this->containerRect = {x, y, this->containerRect.w, this->containerRect.h};
        }

        void update(const float dt) override
        {
            for (Shape *shape : this->shapes)
            {
                shape->update(dt);
            }
        }

        ~Container() override {
            this->clear();
        }

    private:
        SDL_Rect containerRect{};
        std::vector<Shape *> shapes;
    };
}
