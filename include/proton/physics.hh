//
// Created by Monsler on 08.07.2025.
//

#ifndef PHYSICS_H
#define PHYSICS_H
#include "logman.hh"
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
        enum BodyType {
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

#endif //PHYSICS_H
