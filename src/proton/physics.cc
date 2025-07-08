//
// Created by Monsler on 08.07.2025.
//
#include "proton/physics.hh"
#include "box2d/box2d.h"
#include <algorithm>

namespace Proton {
    b2WorldId worldGame;


    void Physics::initPhysicsDevice() {
        constexpr b2Vec2 gravity = {0.0f, -9.8f};
        b2WorldDef world = b2DefaultWorldDef();
        world.gravity = gravity;
        worldGame = b2CreateWorld(&world);

        Log("[PHYSICS] Box2D initialization is successful");
    }

    void Physics::destroyPhysicsDevice() {
        b2DestroyWorld(worldGame);
        worldGame = b2_nullWorldId;
        Log("[PHYSICS] Box2D uninitialization is successful");
    }

    void Physics::simulationStep() {
        b2World_Step(worldGame, 1.0f/60.0f, 6);
    }

    PhysicsBody::PhysicsBody(const BodyType type) {
        switch (type) {
            case Dynamic: {
                this->type = b2_dynamicBody;
            }
            case Static: {
                this->type = b2_staticBody;
            }
            default: {
                this->type = b2_staticBody;
            };
        }
        this->posX = 0.0f;
        this->posY = 0.0f;
        b2BodyDef b2d = b2DefaultBodyDef();
        b2d.type = this->type;
        b2d.position = b2Vec2 {this->posX, this->posY};
        this->bodyId = b2CreateBody(worldGame, &b2d);
        this->usedShape = nullptr;
    }

    void PhysicsBody::bindShape(Shape *shape) {
        /*if (std::find(physicsBodies.begin(), physicsBodies.end(), physicsBodies) != physicsBodies.end()) {
            this->usedShape = shape;
        } else {
            Log("Unexistent physics body. You probably haven't added this body to a scene using addBody(Proton::PhysicsBody body).");
        }*/

    }

    PhysicsBody::~PhysicsBody() {
        this->bodyId = b2_nullBodyId;
    }

    Shape * PhysicsBody::getUsedShape() const {
        return this->usedShape;
    }

    float PhysicsBody::getPosX() const {
        return this->posX;
    }

    float PhysicsBody::getPosY() const {
        return this->posY;
    }

    b2BodyId PhysicsBody::getBody() const {
        return this->bodyId;
    }
}
