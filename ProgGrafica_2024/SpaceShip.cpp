#include "SpaceShip.h"
#include "InputManager/InputManager.h"
#include "Sphere.h"
#include <iostream>

SpaceShip::SpaceShip() : Object3D("data/spaceShip.fiis") {
    speed = 3.0f;
    health = 100.0f;
    maxHealth = 100.0f;
    active = true;
    fireRate = 0.2f; // 5 disparos por segundo
    lastFireTime = 0.0;
    maxBullets = 10;
    canMove = true;

    position = make_vector4f(0.0f, -3.0f, 0.0f, 1.0f);
    scale = make_vector4f(0.8f, 0.8f, 0.8f, 1.0f);

    createPixelCollider();
}

SpaceShip::SpaceShip(Vector4f startPos) : SpaceShip() {
    position = startPos;
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
    constrainToBounds();
    updateCollider();
}

void SpaceShip::handleInput(double timeStep) {
    if (!canMove) return;

    Vector4f movement = make_vector4f(0.0f, 0.0f, 0.0f, 0.0f);

    // Movimiento con WASD
    if (InputManager::keyState[GLFW_KEY_A]) {
        movement.x -= speed * timeStep;
    }
    if (InputManager::keyState[GLFW_KEY_D]) {
        movement.x += speed * timeStep;
    }
    if (InputManager::keyState[GLFW_KEY_W]) {
        movement.y += speed * timeStep;
    }
    if (InputManager::keyState[GLFW_KEY_S]) {
        movement.y -= speed * timeStep;
    }

    position.x += movement.x;
    position.y += movement.y;

    // Disparar con espacio
    if (InputManager::keyState[GLFW_KEY_SPACE]) {
        Vector4f shootDirection = make_vector4f(0.0f, 1.0f, 0.0f, 0.0f);
        shoot(shootDirection);
    }
}

void SpaceShip::shoot(Vector4f direction) {
    double currentTime = glfwGetTime();

    // Control de cadencia de disparo
    if (currentTime - lastFireTime < fireRate) {
        return;
    }

    // Límite de balas simultáneas
    if (bullets.size() >= maxBullets) {
        cleanupInactiveBullets();
        if (bullets.size() >= maxBullets) {
            return;
        }
    }

    // Crear nueva bala
    Vector4f bulletPos = make_vector4f(
        position.x,
        position.y + 0.5f, // Disparar desde la parte superior de la nave
        position.z,
        1.0f
    );

    Bullet* newBullet = new Bullet(bulletPos, direction, 8.0f);
    bullets.push_back(newBullet);

    lastFireTime = currentTime;
    std::cout << "¡Disparo! Balas activas: " << bullets.size() << std::endl;
}

void SpaceShip::updateBullets(double timeStep) {
    for (auto* bullet : bullets) {
        if (bullet->isActive()) {
            bullet->move(timeStep);
        }
    }

    // Limpiar balas inactivas periódicamente
    static double lastCleanup = 0.0;
    double currentTime = glfwGetTime();
    if (currentTime - lastCleanup > 1.0) { // Limpiar cada segundo
        cleanupInactiveBullets();
        lastCleanup = currentTime;
    }
}

void SpaceShip::constrainToBounds() {
    const float bounds = 4.0f;

    position.x = std::max(-bounds, std::min(bounds, position.x));
    position.y = std::max(-4.0f, std::min(3.0f, position.y));
}

void SpaceShip::takeDamage(float damage) {
    health -= damage;
    std::cout << "¡Nave dañada! Salud: " << health << "/" << maxHealth << std::endl;

    if (health <= 0) {
        active = false;
        std::cout << "¡GAME OVER!" << std::endl;
    }
}

void SpaceShip::createPixelCollider() {
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