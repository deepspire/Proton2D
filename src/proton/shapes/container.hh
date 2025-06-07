#include "shape.hh"
#include <vector>
#include "../proton.hh"

namespace Proton
{
    class Container : public Shape
    {
    public:
        Container(int x, int y, int width, int height)
        {
            // блять монслер так сложно было добавить поля x, y в конструкторе
            this->x = x;
            this->y = y;

            this->containerRect = {x, y, width, height};
            this->isVisible = true;
        }

        void paint(SDL_Renderer *render, int rX, int rY) override
        {
            SDL_Rect absoluteClipRect = {
                this->x + rX, this->y + rY,
                this->containerRect.w,this->containerRect.h
            };

            SDL_SetRenderClipRect(render, &absoluteClipRect);
            for (Shape *shape : this->shapes)
            {
                shape->paint(render, this->x + rX, this->y + rY);
            }
            SDL_SetRenderClipRect(render, NULL);
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
            for (Shape *shape : this->shapes)
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

        void update(float dt) override
        {
            for (Shape *shape : this->shapes)
            {
                shape->update(dt);
            }
        }

        ~Container()
        {
            this->clear();
        }

    private:
        SDL_Rect containerRect;
        std::vector<Shape *> shapes;
    };
}
