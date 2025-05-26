#pragma once
#include "Object3D/Object3D.h"
#include "Bullet.h"
#include "prgr/lib.h"
#include <vector>

class Enemy : public Object3D {
public:
    GameObjectType objectType;
    float baseSpeed;
    float currentSpeed;
    bool movingRight;
    float originalX;

    Enemy() : Object3D("data/spaceShip.fiis") {
        objectType = TYPE_ENEMY;

        // Configuración visual (enemigo rotado 180°)
        position = make_vector4f(0, 2, 0, 1);
        rotation = make_vector4f(0, 0, 180, 0);  // Rotado para que apunte hacia abajo
        scale = make_vector4f(0.6f, 0.6f, 1.0f, 1.0f);

        // Configuración de movimiento
        baseSpeed = 0.8f;
        currentSpeed = baseSpeed;
        movingRight = true;
        originalX = position.x;

        cout << "Enemy creado con ID: " << id << endl;
    }

    // Constructor con posición específica
    Enemy(float x, float y) : Object3D("data/spaceShip.fiis") {
        objectType = TYPE_ENEMY;

        position = make_vector4f(x, y, 0, 1);
        rotation = make_vector4f(0, 0, 180, 0);
        scale = make_vector4f(0.6f, 0.6f, 1.0f, 1.0f);

        baseSpeed = 0.8f;
        currentSpeed = baseSpeed;
        movingRight = (rand() % 2 == 0);  // Dirección aleatoria inicial
        originalX = x;

        cout << "Enemy creado en posición (" << x << ", " << y << ") con ID: " << id << endl;
    }

    void move(double timeStep) override {
        // Movimiento horizontal (izquierda-derecha)
        const float HORIZONTAL_LIMIT = 2.5f;
        const float HORIZONTAL_SPEED = 1.5f;

        if (movingRight) {
            position.x += HORIZONTAL_SPEED * timeStep;
            if (position.x > originalX + HORIZONTAL_LIMIT) {
                movingRight = false;
                position.y -= 0.3f;  // Bajar un poco al cambiar dirección
            }
        }
        else {
            position.x -= HORIZONTAL_SPEED * timeStep;
            if (position.x < originalX - HORIZONTAL_LIMIT) {
                movingRight = true;
                position.y -= 0.3f;  // Bajar un poco al cambiar dirección
            }
        }

        // Movimiento vertical lento hacia abajo
        position.y -= currentSpeed * timeStep;

        // Si sale completamente de la pantalla por abajo, reaparecer arriba
        if (position.y < -3.5f) {
            respawn();
        }
    }

    void respawn() {
        position.y = 3.0f + static_cast<float>(rand()) / RAND_MAX * 1.0f;  // Entre 3.0 y 4.0
        position.x = originalX + (-1.0f + static_cast<float>(rand()) / RAND_MAX * 2.0f);  // Variación aleatoria
        movingRight = (rand() % 2 == 0);
        currentSpeed = baseSpeed + static_cast<float>(rand()) / RAND_MAX * 0.5f;  // Velocidad variable
    }

    bool isOutOfBounds() {
        return position.y < -4.0f;
    }

    // Método para verificar si fue destruido (para colisiones)
    bool shouldBeDestroyed() {
        return isOutOfBounds();
    }
};