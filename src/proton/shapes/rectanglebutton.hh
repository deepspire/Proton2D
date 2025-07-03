#pragma once
#include "buttonarea.hh"
#include "rectangle.hh"

void search();

namespace Proton
{
    class RectangleButton : public ButtonArea
    {
    public:
        RectangleButton(int X = 0, int Y = 0, int W = 10, int H = 10, Color color = Color(0, 0, 0)) : ButtonArea(X, Y, W, H)
        {
            this->rectangle = new Rectangle(X, Y, W, H, color);
        }

        ~RectangleButton()
        {
            delete this->rectangle;
        }

        void setPosition(int x, int y) override
        {
            this->x = x;
            this->y = y;
            rectangle->setPosition(x, y);
        }

        void setClickListener(void (*f)())
        {
            this->clickListener = f;
        }

        void resize(int w, int h) override
        {
            this->w = w;
            this->h = h;
            rectangle->resize(w, h);
        }

        void paint(SDL_Renderer *render, int rX, int rY) override
        {
            rectangle->paint(render, rX, rY);
        }

        void setFillColor(Color color) override
        {
            rectangle->setFillColor(color);
        }

        void onClick() override
        {
            if (clickListener)
                clickListener();
        }

    protected:
        Rectangle *rectangle;
        void (*clickListener)();
    };
}