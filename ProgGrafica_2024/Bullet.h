#pragma once
#include "Object3D/Object3D.h"
#include "prgr/lib.h"

using namespace lib;

class Bullet : public Object3D {
private:
    float speed;
    Vector4f direction;
    float lifetime;
    float maxLifetime;
    bool active;
    float damage;

public:
    Bullet(Vector4f startPos, Vector4f shootDirection, float bulletSpeed = 5.0f, float bulletDamage = 25.0f);

    void move(double timeStep) override;
    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }

    // Métodos específicos de Bullet
    void updateLifetime(double timeStep);
    bool isOutOfBounds(float minX, float maxX, float minY, float maxY);

    // Getters
    float getDamage() const { return damage; }
    Vector4f getDirection() const { return direction; }
    float getSpeed() const { return speed; }
    float getLifetime() const { return lifetime; }

    // Setters
    void setDirection(Vector4f newDirection) { direction = newDirection; }
    void setSpeed(float newSpeed) { speed = newSpeed; }
};