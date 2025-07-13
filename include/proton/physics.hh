#pragma once

#include "box2d/types.h"

namespace Proton
{
static float PIXELS_PER_METER = 32.0f;
static float METERS_PER_PIXEL = 1.0f / PIXELS_PER_METER;

class Shape;
class Physics
{
  public:
    static void initPhysicsDevice(float gravityY);
    static void destroyPhysicsDevice();
    static void simulationStep();
    static void update(float dt);

    static float physicsTimeStep;
    static float timePhysics;
};

class PhysicsBody
{
  public:
    enum BodyType : uint8_t
    {
        Dynamic,
        Static
    };
    explicit PhysicsBody(BodyType type, float bWidth = 1.0f, float bHeight = 1.0f, float density = 1.0f,
                         double rotation = 0, bool isCircle = false);
    void bindShape(Shape *shape);

    virtual ~PhysicsBody();
    [[nodiscard]] virtual auto getUsedShape() const -> Shape *;
    [[nodiscard]] auto getPosX() const -> float;
    [[nodiscard]] auto getPosY() const -> float;
    [[nodiscard]] auto getBody() const -> b2BodyId;
    [[nodiscard]] auto getWidth() const -> float;
    [[nodiscard]] auto getHeight() const -> float;
    void setPosition(float x, float y);
    void setPositionInPixels(float px, float py);
    void setRotation(float angle) const;
    void applyForce(float xForce, float yForce) const;
    void applyLinearImpulse(float xForce, float yForce) const;
    void applyAngularImpulse(float angle) const;

  protected:
    b2BodyType type;
    b2BodyId bodyId{};
    b2ShapeId shapeId{};
    float width{}, height{};
    float posX{}, posY{};
    Shape *usedShape{};
};

class PhysicsBoxBody final : public PhysicsBody
{
  public:
    PhysicsBoxBody(BodyType type, float bWidth, float bHeight, float density, double rotation)
        : PhysicsBody(type, bWidth, bHeight, density, rotation, false)
    {
    }
};

class PhysicsCircleBody final : public PhysicsBody
{
  public:
    PhysicsCircleBody(BodyType type, float radius, float density, double rotation)
        : PhysicsBody(type, radius, radius, density, rotation, true)
    {
    }
};
} // namespace Proton
