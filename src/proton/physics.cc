//
// Created by Monsler on 08.07.2025.
//
#include "proton/physics.hh"
#include "box2d/box2d.h"
#include "proton/logman.hh"
#include "proton/shapes/shape.hh"

namespace Proton
{
b2WorldId worldGame;
float Physics::physicsTimeStep = 1.0f / 60.0f;
float Physics::timePhysics = 0.0f;

void Physics::update(float dt)
{
    timePhysics += dt;

    while (timePhysics >= physicsTimeStep)
    {
        simulationStep();
        timePhysics -= physicsTimeStep;
    }
}

void Physics::initPhysicsDevice(float gravityY)
{
    b2Vec2 gravity = {0.0f, gravityY};
    b2WorldDef world = b2DefaultWorldDef();
    world.gravity = gravity;
    worldGame = b2CreateWorld(&world);

    LogNew(Info, "[PHYSICS] Box2D initialization is successful");
}

void Physics::destroyPhysicsDevice()
{
    b2DestroyWorld(worldGame);
    worldGame = b2_nullWorldId;
    LogNew(Info, "[PHYSICS] Box2D uninitialization is successful");
}

void Physics::simulationStep() { b2World_Step(worldGame, Physics::physicsTimeStep, 6); }

PhysicsBody::PhysicsBody(const BodyType type, const float bWidth, const float bHeight, const float density,
                         const double rotation, const bool isCircle)
{
    switch (type)
    {
    case Dynamic:
        this->type = b2_dynamicBody;
        break;
    /*case Static: // Switch has 2 consecutive identical branches (clang-tidybugprone-branch-clone)
        this->type = b2_staticBody;
        break;*/
    default:
        this->type = b2_staticBody;
        break;
    }

    b2ShapeDef shape = b2DefaultShapeDef();
    shape.density = density;
    shape.isSensor = false;

    this->posX = 0.0f;
    this->posY = 0.0f;
    this->width = bWidth;
    this->height = bHeight;
    b2BodyDef b2d = b2DefaultBodyDef();
    b2d.type = this->type;
    b2d.position = b2Vec2{this->posX, this->posY};
    b2d.rotation = b2MakeRot(rotation * 180.0f / M_PI);
    this->bodyId = b2CreateBody(worldGame, &b2d);

    if (isCircle)
    {
        b2Circle circle;
        circle.radius = bWidth;
        circle.center = b2Vec2{0.0f, 0.0f};
        this->shapeId = b2CreateCircleShape(bodyId, &shape, &circle);
    }
    else
    {
        const b2Polygon box = b2MakeBox(bWidth / 2.0f, bHeight / 2.0f);
        this->shapeId = b2CreatePolygonShape(bodyId, &shape, &box);
    }

    this->usedShape = nullptr;
}

void PhysicsBody::bindShape(Shape *shape)
{
    this->usedShape = shape;
    shape->setBody(this);
}

PhysicsBody::~PhysicsBody()
{
    LogNew(Warn, "PhysicsBody destroyed");
    b2DestroyBody(this->bodyId);
    this->bodyId = b2_nullBodyId;
    this->shapeId = b2_nullShapeId;
}

auto PhysicsBody::getUsedShape() const -> Shape * { return this->usedShape; }

auto PhysicsBody::getPosX() const -> float { return this->posX; }

auto PhysicsBody::getPosY() const -> float { return this->posY; }

auto PhysicsBody::getBody() const -> b2BodyId { return this->bodyId; }

auto PhysicsBody::getWidth() const -> float { return width; }

auto PhysicsBody::getHeight() const -> float { return height; }

void PhysicsBody::setPosition(const float x, const float y)
{
    this->posX = x;
    this->posY = -y;
    b2Body_SetTransform(this->bodyId, b2Vec2{this->posX, this->posY}, b2Body_GetRotation(this->bodyId));
}

void PhysicsBody::setPositionInPixels(const float px, const float py)
{
    setPosition(px * METERS_PER_PIXEL, py * METERS_PER_PIXEL);
}

void PhysicsBody::setRotation(const float angle) const {
    const float radians = -(angle * (M_PI / 180));
    b2Body_SetTransform(this->bodyId, b2Body_GetTransform(this->bodyId).p, b2MakeRot(radians));
}

void PhysicsBody::applyForce(const float xForce, const float yForce) const {
    b2Body_ApplyForce(this->bodyId, b2Vec2{xForce, yForce}, b2Vec2{this->posX, this->posY}, true);
}

void PhysicsBody::applyLinearImpulse(const float xForce, const float yForce) const {
    b2Body_ApplyLinearImpulseToCenter(this->bodyId, b2Vec2{xForce, yForce}, true);
}

void PhysicsBody::applyAngularImpulse(const float angle) const {
    const float radians = -(angle * (M_PI / 180));
    b2Body_ApplyAngularImpulse(this->bodyId, radians, true);
}
} // namespace Proton
