#pragma once
#include "Object3D/Object3D.h"
#include "Bullet.h"
#include "prgr/lib.h"
#include <vector>

using namespace lib;

class SpaceShip : public Object3D {
private:
    float speed;
    float health;
    float maxHealth;
    bool active;

    // Sistema de disparo
    float fireRate;
    double lastFireTime;
    std::vector<Bullet*> bullets;
    int maxBullets;

    // Controles
    bool canMove;

public:
    SpaceShip();
    SpaceShip(Vector4f startPos);
    ~SpaceShip();

    void move(double timeStep) override;
    void handleInput(double timeStep);
    void shoot(Vector4f direction);
    void updateBullets(double timeStep);
    void takeDamage(float damage);

    // Getters/Setters
    bool isActive() const { return active; }
    float getHealth() const { return health; }
    float getMaxHealth() const { return maxHealth; }
    std::vector<Bullet*>& getBullets() { return bullets; }

    void setActive(bool state) { active = state; }
    void setSpeed(float newSpeed) { speed = newSpeed; }

    // Crear colisionador a nivel de píxel
    void createPixelCollider();

    // Cleanup
    void cleanupInactiveBullets();

private:
    void constrainToBounds();
};