#pragma once
#include "buttonarea.hh"
#include "image.hh"

namespace Proton
{
    class ImageButton : public ButtonArea
    {
    public:
        ImageButton(SDL_Texture* texture, int X = 0, int Y = 0, int W = 10, int H = 10) : ButtonArea(X, Y, W, H)
        {
            this->image = new Image(texture, X, Y, W, H);
        }

        ~ImageButton()
        {
            delete this->image;
        }

        void setPosition(int x, int y) override
        {
            this->x = x;
            this->y = y;
            image->setPosition(x, y);
        }

        void resize(int w, int h) override
        {
            this->w = w;
            this->h = h;
            image->resize(w, h);
        }

        void paint(SDL_Renderer *render, int rX, int rY) override
        {
            image->paint(render, rX, rY);
        }

        void setFillColor([[maybe_unused]] Color color) override
        {
        }

        void onClick() override
        {
            std::cout << "image" << std::endl;
        }

    protected:
        Image *image;
    };
};