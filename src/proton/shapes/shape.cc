//
// Created by Monsler on 10.07.2025.
//
#include "proton/shapes/shape.hh"

#include "proton/physics.hh"

namespace Proton {
    void Shape::setPosition(const float x, const float y) {
        this->position.x = x;
        this->position.y = y;
    }

    void Shape::setRotation(const float angle) { this->rotation = angle; }
}
