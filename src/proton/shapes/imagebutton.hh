#pragma once
#include "buttonarea.hh"
#include "image.hh"

namespace Proton
{
    class ImageButton : public ButtonArea
    {
    public:
        explicit ImageButton(SDL_Texture* texture, const int x = 0, const int y = 0, const int w = 10, const int h = 10) : ButtonArea(x, y, w, h)
        {
            this->image = new Image(texture, x, y, w, h);
        }

        ~ImageButton() override {
            delete this->image;
        }

        void setPosition(const float x, const float y) override
        {
            this->x = x;
            this->y = y;
            image->setPosition(x, y);
        }

        void resize(const int w, const int h) override
        {
            this->w = w;
            this->h = h;
            image->resize(w, h);
        }

        void paint(SDL_Renderer *render, const int rX, const int rY) override
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