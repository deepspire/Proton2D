//
// Created by Monsler on 08.07.2025.
//

#pragma once

#include "box2d/types.h"

namespace Proton {
    class Shape;
    class Physics {
    public:
        static void initPhysicsDevice();
        static void destroyPhysicsDevice();
        static void simulationStep();
    };

    class PhysicsBody {
    public:
        enum BodyType : uint8_t {
            Dynamic,
            Static
        };
        explicit PhysicsBody(BodyType type, float bWidth=1.0f, float bHeight=1.0f, float density=1.0f, double rotation=0);
        void bindShape(Shape* shape);
        ~PhysicsBody();
        [[nodiscard]] auto getUsedShape() const -> Shape*;
        [[nodiscard]] auto getPosX() const -> float;
        [[nodiscard]] auto getPosY() const -> float;
        [[nodiscard]] auto getBody() const -> b2BodyId;
        void setPosition(float x, float y);
    private:
        b2BodyType type;
        b2BodyId bodyId{};
        b2ShapeId shapeId{};
        float posX, posY;
        Shape* usedShape;
    };
}
