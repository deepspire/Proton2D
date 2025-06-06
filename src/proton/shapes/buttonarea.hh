#pragma once
#include <iostream>
#include "shape.hh"

namespace Proton
{
    class ButtonArea : public Shape
    {
    public:
        ButtonArea(int x = 0, int y = 0, int w = 10, int h = 10)
        {
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
        }

        virtual void onClick()
        {
        }

        virtual void paint() override
        {
        }

        virtual void setPosition(int x, int y) override
        {
            this->x = x;
            this->y = y;
        }

        virtual void setFillColor([[maybe_unused]] Color color) override
        {
        }

        virtual void resize(int w, int h)
        {
            this->w = w;
            this->h = h;
        }

        int getW() const { return this->w; }
        int getH() const { return this->h; }

    protected:
        int w, h;
    };
};