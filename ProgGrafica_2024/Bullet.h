#pragma once
#include "Object3D/Object3D.h"
#include "prgr/lib.h"

using namespace lib;

class Bullet : public Object3D {
private:
    Vector4f velocity;
    bool active;
    float lifeTime;
    float maxLifeTime;
    float damage;

public:
    Bullet();
    Bullet(Vector4f startPos, Vector4f direction, float speed = 5.0f);
    ~Bullet();

    void move(double timeStep) override;
    void updateLifetime(double timeStep);

    // Getters/Setters
    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }
    float getDamage() const { return damage; }
    Vector4f getVelocity() const { return velocity; }

    // Crear colisionador a nivel de píxel
    void createPixelCollider();

private:
    void checkBounds();
};