//
// Created by Monsler on 13.07.2025.
//
#include "proton/shapes/circlebutton.hh"

namespace Proton
{
     CircleButton::CircleButton(const float x, const float y, const int radius, const Color fillColor, const Circle::Style style) : ButtonArea(x, y, radius, radius)
     {
         this->position.x = x;
         this->position.y = y;
         this->circle = new Proton::Circle(x, y, radius, fillColor, style);
     }

     void CircleButton::setPosition(const float x, const float y)
     {
         this->position.x = x;
         this->position.y = y;
         this->circle->setPosition(x, y);
     }

    void CircleButton::setRotation(const float angle) { this->circle->setRotation(angle); }
     } // namespace Proton