#include "Bullet.h"
#include "Sphere.h"
#include <iostream>

Bullet::Bullet() : Object3D("data/bullet.fiis") {
    velocity = make_vector4f(0.0f, 0.0f, 0.0f, 0.0f);
    active = true;
    lifeTime = 0.0f;
    maxLifeTime = 3.0f; // 3 segundos de vida
    damage = 25.0f;

    scale = make_vector4f(0.1f, 0.1f, 0.1f, 1.0f);
    createPixelCollider();
}

Bullet::Bullet(Vector4f startPos, Vector4f direction, float speed) : Bullet() {
    position = startPos;
    velocity = make_vector4f(
        direction.x * speed,
        direction.y * speed,
        direction.z * speed,
        0.0f
    );

    // Rotar la bala según la dirección
    float angle = std::atan2(direction.y, direction.x) * 180.0f / M_PI;
    rotation.z = angle;
}

Bullet::~Bullet() {
    // Destructor
}

void Bullet::move(double timeStep) {
    if (!active) return;

    // Mover según velocidad
    position.x += velocity.x * timeStep;
    position.y += velocity.y * timeStep;
    position.z += velocity.z * timeStep;

    updateLifetime(timeStep);
    checkBounds();
    updateCollider();
}

void Bullet::updateLifetime(double timeStep) {
    lifeTime += timeStep;
    if (lifeTime >= maxLifeTime) {
        active = false;
    }
}

void Bullet::checkBounds() {
    const float bounds = 6.0f;

    // Desactivar si sale de los límites
    if (position.x > bounds || position.x < -bounds ||
        position.y > bounds || position.y < -bounds) {
        active = false;
    }
}

void Bullet::createPixelCollider() {
    if (!mat || !mat->texture) {
        // Crear colisionador simple para balas sin textura
        coll->clearParticles();

        Collider::Particle p;
        float half = 0.05f;
        p.min = make_vector4f(-half, -half, -half, 1.0f);
        p.max = make_vector4f(half, half, half, 1.0f);
        coll->addParticle(p);

        return;
    }

    coll->clearParticles();

    float pixelSize = 0.005f; // Balas más pequeñas
    float textureWidth = static_cast<float>(mat->texture->w);
    float textureHeight = static_cast<float>(mat->texture->h);

    // Recorrer cada píxel de la textura
    for (int y = 0; y < mat->texture->h; y++) {
        for (int x = 0; x < mat->texture->w; x++) {
            auto& pixel = mat->texture->pixels[y * mat->texture->w + x];

            // Si el píxel no es transparente
            if (pixel.a > 128) {
                Collider::Particle p;

                // Convertir coordenadas de píxel a mundo
                float worldX = (x / textureWidth - 0.5f) * scale.x;
                float worldY = (y / textureHeight - 0.5f) * scale.y;

                p.min = make_vector4f(
                    worldX - pixelSize / 2,
                    worldY - pixelSize / 2,
                    -pixelSize / 2,
                    1.0f
                );
                p.max = make_vector4f(
                    worldX + pixelSize / 2,
                    worldY + pixelSize / 2,
                    pixelSize / 2,
                    1.0f
                );

                coll->addParticle(p);
            }
        }
    }

    // Solo subdivide si hay muchos píxeles (optimización)
    if (coll->getParticleCount() > 8) {
        coll->subdivide();
    }
}