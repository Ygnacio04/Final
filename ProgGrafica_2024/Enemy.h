#pragma once
#include "Object3D/Object3D.h"
#include "prgr/lib.h"
#include "Collider.h"
#include <vector>

using namespace lib;

class Enemy : public Object3D {
private:
    float speed;
    Vector4f direction;
    float health;
    bool active;

public:
    Enemy(Vector4f startPos, Vector4f moveDirection, float moveSpeed = 1.0f);

    void move(double timeStep) override;
    void takeDamage(float damage);
    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }

    // Métodos específicos de Enemy
    void updateAI(double timeStep);
    bool isOutOfBounds(float minX, float maxX, float minY, float maxY);

    // Getters
    float getHealth() const { return health; }
    Vector4f getDirection() const { return direction; }
    float getSpeed() const { return speed; }

    // Setters
    void setDirection(Vector4f newDirection) { direction = newDirection; }
    void setSpeed(float newSpeed) { speed = newSpeed; }
};