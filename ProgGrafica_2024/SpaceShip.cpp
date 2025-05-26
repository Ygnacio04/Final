#include "SpaceShip.h"
#include "InputManager/InputManager.h"
#include "prgr/common.h"
#include <iostream>

SpaceShip::SpaceShip(Vector4f startPos, float moveSpeed)
    : Object3D(), speed(moveSpeed), health(100.0f), maxHealth(100.0f),
    active(true), fireRate(0.3f), lastFireTime(0.0f) {

    this->position = startPos;
    this->scale = make_vector4f(0.8f, 0.8f, 0.8f, 1.0f);
}

SpaceShip::~SpaceShip() {
    // Limpiar balas
    for (auto* bullet : bullets) {
        delete bullet;
    }
    bullets.clear();
}

void SpaceShip::move(double timeStep) {
    if (!active) return;

    handleInput(timeStep);
    updateBullets(timeStep);
    cleanupInactiveBullets();

    // Llamar al método padre
    Object3D::move(timeStep);
}

void SpaceShip::handleInput(double timeStep) {
    float deltaTime = (float)timeStep;

    // Movimiento con WASD
    if (InputManager::keyState[GLFW_KEY_W]) {
        position.y += speed * deltaTime;
    }
    if (InputManager::keyState[GLFW_KEY_S]) {
        position.y -= speed * deltaTime;
    }
    if (InputManager::keyState[GLFW_KEY_A]) {
        position.x -= speed * deltaTime;
    }
    if (InputManager::keyState[GLFW_KEY_D]) {
        position.x += speed * deltaTime;
    }

    // Disparo con ESPACIO
    if (InputManager::keyState[GLFW_KEY_SPACE]) {
        double currentTime = glfwGetTime();
        if (currentTime - lastFireTime >= fireRate) {
            shoot(make_vector4f(0, 1, 0, 0)); // Disparar hacia arriba
            lastFireTime = currentTime;
        }
    }
}

void SpaceShip::shoot(Vector4f direction) {
    Vector4f bulletPos = position;
    bulletPos.y += 0.5f; // Disparar desde arriba de la nave

    Bullet* newBullet = new Bullet(bulletPos, direction, 8.0f, 25.0f);
    bullets.push_back(newBullet);

    std::cout << "SpaceShip fired! Total bullets: " << bullets.size() << std::endl;
}

void SpaceShip::updateBullets(double timeStep) {
    for (auto* bullet : bullets) {
        if (bullet->isActive()) {
            bullet->move(timeStep);

            // Comprobar límites
            if (bullet->isOutOfBounds(-10.0f, 10.0f, -10.0f, 10.0f)) {
                bullet->setActive(false);
            }
        }
    }
}

void SpaceShip::takeDamage(float damage) {
    health -= damage;
    if (health <= 0) {
        active = false;
        health = 0;
        std::cout << "SpaceShip destroyed!" << std::endl;
    }
}

void SpaceShip::cleanupInactiveBullets() {
    auto it = bullets.begin();
    while (it != bullets.end()) {
        if (!(*it)->isActive()) {
            delete* it;
            it = bullets.erase(it);
        }
        else {
            ++it;
        }
    }
}