//
// Created by Monsler on 13.07.2025.
//

#ifndef CIRCLEBUTTON_H
#define CIRCLEBUTTON_H
#include "buttonarea.hh"
#include "circle.hh"

namespace Proton
{
    class CircleButton : public ButtonArea
    {
    public:
        explicit CircleButton(float x, float y, int radius = 40, Color fillColor = Color(), Circle::Style style = Circle::Style::Fill);
        void paint(SDL_Renderer *render, float rX, float rY) override { this->circle->paint(render, rX, rY); }
        void setPosition(float x, float y) override;
        void setRotation(float angle) override;
        ~CircleButton() override {
            delete this->circle;
        }
    private:
        Circle* circle;
    };
}

#endif //CIRCLEBUTTON_H
