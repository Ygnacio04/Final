#pragma once
#include "Object3D/Object3D.h"
#include "Bullet.h"
#include "prgr/lib.h"
#include <vector>


class Bullet : public Object3D {
public:
    GameObjectType objectType;
    float speed;
    bool isPlayerBullet;  // true = del jugador, false = del enemigo

    Bullet() : Object3D("data/bullet.fiis") {
        objectType = TYPE_BULLET;

        // Configuraci�n visual
        position = make_vector4f(0, 0, 0, 1);
        rotation = make_vector4f(0, 0, 0, 0);
        scale = make_vector4f(0.3f, 0.3f, 1.0f, 1.0f);

        // Configuraci�n de movimiento
        speed = 5.0f;
        isPlayerBullet = true;  // Por defecto es del jugador

        cout << "Bullet creada con ID: " << id << endl;
    }

    // Constructor con posici�n espec�fica
    Bullet(Vector4f startPos, bool fromPlayer = true) : Object3D("data/bullet.fiis") {
        objectType = TYPE_BULLET;

        position = startPos;
        rotation = make_vector4f(0, 0, 0, 0);
        scale = make_vector4f(0.3f, 0.3f, 1.0f, 1.0f);

        speed = 5.0f;
        isPlayerBullet = fromPlayer;

        // Si es bala de enemigo, cambiar direcci�n y color
        if (!isPlayerBullet) {
            speed = 3.0f;  // M�s lenta
            rotation.z = 180.0f;  // Rotar para que apunte hacia abajo
        }

        cout << "Bullet creada en posici�n (" << startPos.x << ", " << startPos.y
            << ") - Jugador: " << (fromPlayer ? "S�" : "No") << " - ID: " << id << endl;
    }

    void move(double timeStep) override {
        if (isPlayerBullet) {
            // Balas del jugador van hacia arriba
            position.y += speed * timeStep;
        }
        else {
            // Balas de enemigos van hacia abajo
            position.y -= speed * timeStep;
        }
    }

    bool isOutOfBounds() {
        if (isPlayerBullet) {
            return position.y > 3.5f;   // Sali� por arriba
        }
        else {
            return position.y < -3.5f;  // Sali� por abajo
        }
    }

    // M�todo para verificar si debe ser eliminada
    bool shouldBeDestroyed() {
        return isOutOfBounds();
    }

    // M�todo para obtener el tipo como string (para debug)
    string getType() {
        return isPlayerBullet ? "Player" : "Enemy";
    }
};