#include "Bullet.h"
#include <iostream>

Bullet::Bullet(Vector4f startPos, Vector4f shootDirection, float bulletSpeed, float bulletDamage)
    : Object3D(), speed(bulletSpeed), direction(shootDirection),
    lifetime(0.0f), maxLifetime(5.0f), active(true), damage(bulletDamage) {

    this->position = startPos;
    this->scale = make_vector4f(0.1f, 0.1f, 0.1f, 1.0f);

    // Normalizar dirección
    direction = normalize(direction);
}

void Bullet::move(double timeStep) {
    if (!active) return;

    // Movimiento de la bala
    position.x += direction.x * speed * (float)timeStep;
    position.y += direction.y * speed * (float)timeStep;
    position.z += direction.z * speed * (float)timeStep;

    // Actualizar tiempo de vida
    updateLifetime(timeStep);

    // Llamar al método padre
    Object3D::move(timeStep);
}

void Bullet::updateLifetime(double timeStep) {
    lifetime += (float)timeStep;

    if (lifetime >= maxLifetime) {
        active = false;
        std::cout << "Bullet expired" << std::endl;
    }
}

bool Bullet::isOutOfBounds(float minX, float maxX, float minY, float maxY) {
    return (position.x < minX || position.x > maxX ||
        position.y < minY || position.y > maxY);
}