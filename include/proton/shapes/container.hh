#pragma once
#include "shape.hh"
#include <vector>
#include "../proton.hh"

namespace Proton
{
    class Container final : public Shape
    {
    public:
        explicit Container(const float x, const float y, const int width, const int height)
        {
            this->x = x;
            this->y = y;

            this->containerRect = {x, y, static_cast<float>(width), static_cast<float>(height)};
        }

        void paint(SDL_Renderer *render, const float rX, const float rY) override
        {
            const SDL_Rect absoluteClipRect = {
                static_cast<int>(this->x + rX), static_cast<int>(this->y + rY),
                static_cast<int>(this->containerRect.w), static_cast<int>(this->containerRect.h)};

            SDL_SetRenderClipRect(render, &absoluteClipRect);
            for (Shape *shape : this->shapes)
            {
                shape->setRotation(this->rotation);
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

        void setPosition(float x, float y) override
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
        SDL_FRect containerRect{};
        std::vector<Shape *> shapes;
    };
}
