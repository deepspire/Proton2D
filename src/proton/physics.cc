//
// Created by Monsler on 08.07.2025.
//
#include "proton/physics.hh"
#include "box2d/box2d.h"
#include "proton/logman.hh"

namespace Proton
{
    b2WorldId worldGame;
    std::vector<PhysicsBody*> physicsBodies;

    void Physics::initPhysicsDevice()
    {
        constexpr b2Vec2 gravity = {0.0f, -15.8f};
        b2WorldDef world = b2DefaultWorldDef();
        world.gravity = gravity;
        worldGame = b2CreateWorld(&world);

        Log("[PHYSICS] Box2D initialization is successful");
    }

    void Physics::destroyPhysicsDevice()
    {
        b2DestroyWorld(worldGame);
        worldGame = b2_nullWorldId;
        Log("[PHYSICS] Box2D uninitialization is successful");
    }

    void Physics::simulationStep() { b2World_Step(worldGame, 1.0f / 60.0f, 6); }

    PhysicsBody::PhysicsBody(const BodyType type, const float bWidth, const float bHeight, const float density)
    {
        switch (type)
        {
        case Dynamic:
        {
            this->type = b2_dynamicBody;
            break;
        }
        case Static:
        {
            this->type = b2_staticBody;
            break;
        }
        default:
        {
            this->type = b2_staticBody;
            break;
        };
        }
        const b2Polygon box = b2MakeBox(bWidth, bHeight);
        b2ShapeDef shape = b2DefaultShapeDef();
        shape.density = density;
        shape.isSensor = false;
        this->posX = 0.0f;
        this->posY = 0.0f;
        b2BodyDef b2d = b2DefaultBodyDef();
        b2d.type = this->type;
        b2d.position = b2Vec2{this->posX, this->posY};
        this->bodyId = b2CreateBody(worldGame, &b2d);
        this->shapeId = b2CreatePolygonShape(bodyId, &shape, &box);
        this->usedShape = nullptr;
    }
    void PhysicsBody::bindShape(Shape* shape)
    {
        this->usedShape = shape;
        physicsBodies.push_back(this);
        Log("[PHYSICS] body added. Total: ", physicsBodies.size());
    }


    PhysicsBody::~PhysicsBody() { b2DestroyBody(this->bodyId); this->bodyId = b2_nullBodyId; this->shapeId = b2_nullShapeId; }

    auto PhysicsBody::getUsedShape() const -> Shape * { return this->usedShape; }

    auto PhysicsBody::getPosX() const -> float { return this->posX; }

    auto PhysicsBody::getPosY() const -> float { return this->posY; }

    auto PhysicsBody::getBody() const -> b2BodyId { return this->bodyId; }

    void PhysicsBody::setPosition(const float x, const float y)
    {
        this->posX = x;
        this->posY = y;
        b2Body_SetTransform(this->bodyId, b2Vec2{x, y}, b2Body_GetRotation(this->bodyId));
    }
} // namespace Proton
