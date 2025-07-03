#pragma once
#include "shape.hh"

namespace Proton
{
    class ButtonArea : public Shape
    {
    public:
        explicit ButtonArea(const int x = 0, const int y = 0, const int w = 10, const int h = 10)
        {
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
        }

        virtual void onClick()
        {
        }

        void paint(SDL_Renderer *render, int rX, int rY) override
        {
        }

        void setPosition(const int x, const int y) override
        {
            this->x = x;
            this->y = y;
        }

        void setFillColor([[maybe_unused]] Color color) override
        {
        }

        virtual void resize(const int w, const int h)
        {
            this->w = w;
            this->h = h;
        }

        [[nodiscard]] int getW() const { return this->w; }
        [[nodiscard]] int getH() const { return this->h; }

    protected:
        int w, h;
    };
};