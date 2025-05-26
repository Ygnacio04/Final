#pragma once
#include "Object3D/Object3D.h"
#include "prgr/lib.h"
#include <cmath>

using namespace lib;

class Enemy : public Object3D {
private:
    float speed;
    Vector4f direction;
    float health;
    bool active;

    // Patrón de movimiento
    float moveTimer;
    float changeDirectionTime;

public:
    Enemy();
    Enemy(Vector4f startPos);
    ~Enemy();

    void move(double timeStep) override;
    void takeDamage(float damage);
    void updateAI(double timeStep);

    // Getters/Setters
    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }
    float getHealth() const { return health; }
    Vector4f getDirection() const { return direction; }

    // Crear colisionador a nivel de píxel
    void createPixelCollider();

private:
    void changeDirection();
    void bounceOffBounds();
};