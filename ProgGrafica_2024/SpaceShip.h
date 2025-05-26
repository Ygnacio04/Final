#pragma once
#include "Object3D/Object3D.h"
#include "Bullet.h"
#include "prgr/lib.h"
#include <vector>

using namespace lib;

typedef enum {
    TYPE_SPACESHIP = 0,
    TYPE_ENEMY = 1,
    TYPE_BULLET = 2
} GameObjectType;

class SpaceShip : public Object3D {
public:
    GameObjectType objectType;
    bool canShoot;
    double lastShotTime;
    static const double SHOOT_COOLDOWN;

    SpaceShip() : Object3D("data/spaceShip.fiis") {
        objectType = TYPE_SPACESHIP;

        // Posición inicial (parte inferior de la pantalla)
        position = make_vector4f(0, -2, 0, 1);
        rotation = make_vector4f(0, 0, 0, 0);
        scale = make_vector4f(0.8f, 0.8f, 1.0f, 1.0f);

        // Control de disparo
        canShoot = true;
        lastShotTime = 0;

        cout << "SpaceShip creada con ID: " << id << endl;
    }

    void move(double timeStep) override {
        const float speed = 3.0f;  // Velocidad de movimiento

        // Control con WASD o flechas
        if (InputManager::keyState[GLFW_KEY_A] || InputManager::keyState[GLFW_KEY_LEFT]) {
            position.x -= speed * timeStep;
        }
        if (InputManager::keyState[GLFW_KEY_D] || InputManager::keyState[GLFW_KEY_RIGHT]) {
            position.x += speed * timeStep;
        }
        if (InputManager::keyState[GLFW_KEY_W] || InputManager::keyState[GLFW_KEY_UP]) {
            position.y += speed * timeStep;
        }
        if (InputManager::keyState[GLFW_KEY_S] || InputManager::keyState[GLFW_KEY_DOWN]) {
            position.y -= speed * timeStep;
        }

        // Limitar movimiento dentro de la pantalla
        const float LIMIT_X = 2.8f;
        const float LIMIT_Y_TOP = 2.5f;
        const float LIMIT_Y_BOTTOM = -2.8f;

        if (position.x < -LIMIT_X) position.x = -LIMIT_X;
        if (position.x > LIMIT_X) position.x = LIMIT_X;
        if (position.y > LIMIT_Y_TOP) position.y = LIMIT_Y_TOP;
        if (position.y < LIMIT_Y_BOTTOM) position.y = LIMIT_Y_BOTTOM;

        // Control de cooldown de disparo
        double currentTime = glfwGetTime();
        if (currentTime - lastShotTime > SHOOT_COOLDOWN) {
            canShoot = true;
        }
    }

    bool canShootBullet() {
        return canShoot;
    }

    void shoot() {
        if (canShoot) {
            canShoot = false;
            lastShotTime = glfwGetTime();
        }
    }

    Vector4f getBulletSpawnPosition() {
        return make_vector4f(position.x, position.y + 0.6f, position.z, 1.0f);
    }
};