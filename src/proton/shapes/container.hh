#include "shape.hh"
#include <vector>
#include "../proton.hh"

namespace Proton
{
    class Container : public Shape
    {
    public:
        Container(Display *display, int x, int y, int width, int height)
        {
            this->render = display->getRenderer();

            // блять монслер так сложно было добавить поля x, y в конструкторе
            this->x = x;
            this->y = y;

            this->containerRect = {x, y, width, height};
            this->isVisible = true;
        }

        void paint(int rX, int rY) override
        {
            SDL_SetRenderClipRect(this->render, &this->containerRect);
            for (Shape *shape : this->shapes)
            {
                shape->paint(this->x + rX, this->y + rY);
            }
            SDL_SetRenderClipRect(this->render, NULL);
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

        ~Container()
        {
            this->clear();
        }

    private:
        SDL_Rect containerRect;
        std::vector<Shape *> shapes;
    };
}
