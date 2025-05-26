#include "Enemy.h"
#include "Sphere.h"
#include <random>
#include <iostream>

Enemy::Enemy() : Object3D("data/spaceShip.fiis") {
    speed = 1.5f;
    health = 100.0f;
    active = true;
    direction = make_vector4f(1.0f, 0.0f, 0.0f, 0.0f);
    moveTimer = 0.0f;
    changeDirectionTime = 2.0f; // Cambia dirección cada 2 segundos

    // Posición inicial aleatoria
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-3.0f, 3.0f);

    position = make_vector4f(dis(gen), dis(gen), 0.0f, 1.0f);
    scale = make_vector4f(0.5f, 0.5f, 0.5f, 1.0f);

    // Rota el enemigo 180 grados para que mire hacia abajo
    rotation.z = 180.0f;

    createPixelCollider();
}

Enemy::Enemy(Vector4f startPos) : Enemy() {
    position = startPos;
}

Enemy::~Enemy() {
    // Destructor
}

void Enemy::move(double timeStep) {
    if (!active) return;

    updateAI(timeStep);

    // Mover según la dirección
    position.x += direction.x * speed * timeStep;
    position.y += direction.y * speed * timeStep;

    bounceOffBounds();
    updateCollider();
}

void Enemy::updateAI(double timeStep) {
    moveTimer += timeStep;

    // Cambiar dirección periódicamente
    if (moveTimer >= changeDirectionTime) {
        changeDirection();
        moveTimer = 0.0f;
    }
}

void Enemy::changeDirection() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> angleDis(0.0f, 2.0f * M_PI);

    float angle = angleDis(gen);
    direction = make_vector4f(
        std::cos(angle),
        std::sin(angle),
        0.0f,
        0.0f
    );

    // Ajustar rotación visual
    rotation.z = 180.0f + (angle * 180.0f / M_PI);
}

void Enemy::bounceOffBounds() {
    const float bounds = 4.5f;

    // Rebotar en los límites de la pantalla
    if (position.x > bounds || position.x < -bounds) {
        direction.x = -direction.x;
        position.x = std::max(-bounds, std::min(bounds, position.x));
    }

    if (position.y > bounds || position.y < -bounds) {
        direction.y = -direction.y;
        position.y = std::max(-bounds, std::min(bounds, position.y));
    }
}

void Enemy::takeDamage(float damage) {
    health -= damage;
    if (health <= 0) {
        active = false;
        std::cout << "¡Enemigo destruido!" << std::endl;
    }
}

void Enemy::createPixelCollider() {
    if (!mat || !mat->texture) {
        std::cout << "Error: No hay textura para crear colisionador de píxel" << std::endl;
        return;
    }

    coll->clearParticles();

    float pixelSize = 0.01f;
    float textureWidth = static_cast<float>(mat->texture->w);
    float textureHeight = static_cast<float>(mat->texture->h);

    // Recorrer cada píxel de la textura
    for (int y = 0; y < mat->texture->h; y++) {
        for (int x = 0; x < mat->texture->w; x++) {
            auto& pixel = mat->texture->pixels[y * mat->texture->w + x];

            // Si el píxel no es transparente
            if (pixel.a > 128) { // Umbral de transparencia
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