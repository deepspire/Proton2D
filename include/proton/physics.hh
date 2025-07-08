//
// Created by Monsler on 08.07.2025.
//

#pragma once
#include <cstdint>
#include <vector>

#include "box2d/types.h"
#include "shapes/shape.hh"

namespace Proton {

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
        explicit PhysicsBody(BodyType type);
        void bindShape(Shape* shape);
        ~PhysicsBody();
        [[nodiscard]] Shape* getUsedShape() const;
        [[nodiscard]] float getPosX() const;
        [[nodiscard]] float getPosY() const;
        [[nodiscard]] b2BodyId getBody() const;
    private:
        b2BodyType type;
        b2BodyId bodyId{};
        float posX, posY;
        Shape* usedShape;
    };

    static std::vector<PhysicsBody> physicsBodies;
}
