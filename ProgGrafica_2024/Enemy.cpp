#include "Enemy.h"
#include "InputManager/InputManager.h"
#include <iostream>

Enemy::Enemy(Vector4f startPos, Vector4f moveDirection, float moveSpeed)
    : Object3D(), speed(moveSpeed), direction(moveDirection), health(100.0f), active(true) {

    this->position = startPos;
    this->scale = make_vector4f(0.5f, 0.5f, 0.5f, 1.0f);

    // Normalizar dirección
    direction = normalize(direction);
}

void Enemy::move(double timeStep) {
    if (!active) return;

    // Movimiento básico del enemigo
    position.x += direction.x * speed * (float)timeStep;
    position.y += direction.y * speed * (float)timeStep;
    position.z += direction.z * speed * (float)timeStep;

    // Actualizar AI
    updateAI(timeStep);

    // Llamar al método padre para actualizaciones adicionales
    Object3D::move(timeStep);
}

void Enemy::takeDamage(float damage) {
    health -= damage;
    if (health <= 0) {
        active = false;
        std::cout << "Enemy destroyed!" << std::endl;
    }
}

void Enemy::updateAI(double timeStep) {
    // AI básica: cambiar dirección ocasionalmente
    static double aiTimer = 0.0;
    aiTimer += timeStep;

    if (aiTimer > 2.0) { // Cada 2 segundos
        // Cambiar dirección aleatoriamente
        direction.x += (rand() % 100 - 50) / 100.0f * 0.1f;
        direction.y += (rand() % 100 - 50) / 100.0f * 0.1f;
        direction = normalize(direction);
        aiTimer = 0.0;
    }
}

bool Enemy::isOutOfBounds(float minX, float maxX, float minY, float maxY) {
    return (position.x < minX || position.x > maxX ||
        position.y < minY || position.y > maxY);
}