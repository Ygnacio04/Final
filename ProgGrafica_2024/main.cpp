#include <iostream>
#define GLAD_BIN
#define _USE_MATH_DEFINES
#include <math.h>

#include "prgr/lib.h"
#include "Render/Render.h"
#include "Object3D/Object3D.h"
#include "InputManager/InputManager.h"
#include "OrbitalLight.h"

#include "SpaceShip.h"
#include "Enemy.h"
#include "Bullet.h"

using namespace std;
using namespace lib;

/*
int main() {
    Render render(640, 480);
    render.initGL();

    Camera* cam = new Camera({0,0,3,1},
                            {0,0,0,0},
                            {0,0,0,1},
                            {0,1,0,0},
		90.0f, 640.0f / 480.0f, 0.1f, 100.0f);
    render.putCamera(cam);

    Light* light = new Light(
        { 0,0,0,1 },
        { 3,3,3,1 },
        { 0,0,1,1 },
        0.2f,
        0.8f,
        0.5f,
        Light::POINT
    );
     
    OrbitalLight* light2 = new OrbitalLight(
        { 0, 0, 0, 1 },
        { -3, 3, 0, 1 },
        { 1, 0, 0, 1 },
        0.1f, 0.7f, 0.3f,
        Light::POINT,
        { 0, 0, 0, 1 },
        5.0f,
        1.0f
    );

    render.putLight(light);
	render.putLight(light2);

    // Carga el cubo
    Object3D* cubo = new Object3D("./data/kubo.fiis");
    cubo->position = { 4.0f, 0.0f, 0.0f, 1.0f };
    cubo->rotation.z = 45.0f;
    cubo->rotation.x = 45.0f;

    // Carga la esfera
    Object3D* sphere = new Object3D("./data/sphere.fiis");
    sphere->position = { -4.0f, 0.0f, 0.0f, 1.0f };
    sphere->rotation.z = 45.0f;
    sphere->rotation.x = 45.0f;

    render.putObject(cubo->id, cubo);
    render.putObject(sphere->id, sphere);

    render.mainLoop();
    return 0;
}
*/

/**/
int main() {
    Render render(640, 480);
    render.initGL();

    // Cámara para vista 2D
    Camera* cam = new Camera({0,0,5,1},    // Posición más alejada para vista 2D
                            {0,0,0,0},
                            {0,0,0,1},
                            {0,1,0,0},
                            45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
    render.putCamera(cam);

    // Luces para el escenario 2D
    Light* light = new Light(
        { 0,0,0,1 },
        { 0,0,5,1 },   // Luz desde arriba
        { 1,1,1,1 },   // Luz blanca
        0.3f, 0.7f, 0.2f,
        Light::POINT
    );
    render.putLight(light);

    std::cout << "=== ESCENARIO NAVES2D ===" << std::endl;
    std::cout << "Controles:" << std::endl;
    std::cout << "WASD - Mover nave" << std::endl;
    std::cout << "ESPACIO - Disparar" << std::endl;
    std::cout << "=========================" << std::endl;

    // === CREAR OBJETOS DEL JUEGO ===
    
    // Nave del jugador
    SpaceShip* player = new SpaceShip();
    render.putObject(100, player); // ID especial para el jugador
    
    // Crear enemigos
    std::vector<Enemy*> enemies;
    for (int i = 0; i < 3; i++) {
        Vector4f enemyPos = make_vector4f(
            -3.0f + i * 3.0f,  // Distribuir horizontalmente
            2.0f,               // Parte superior de la pantalla
            0.0f, 1.0f
        );
        
        Enemy* enemy = new Enemy(enemyPos);
        enemies.push_back(enemy);
        render.putObject(200 + i, enemy); // IDs 200, 201, 202...
    }
    
    std::cout << "Juego iniciado con " << enemies.size() << " enemigos" << std::endl;

    // === BUCLE PRINCIPAL DEL JUEGO ===
    while (!glfwWindowShouldClose(render.window)) {
        glfwPollEvents();

        // Actualizar lógica del juego
        
        // 1. Mover jugador
        if (player->isActive()) {
            player->move(1.0f);
        }
        
        // 2. Mover enemigos
        for (auto* enemy : enemies) {
            if (enemy->isActive()) {
                enemy->move(1.0f);
            }
        }
        
        // 3. Detectar colisiones bala-enemigo
        if (player->isActive()) {
            auto& bullets = player->getBullets();
            
            for (auto* bullet : bullets) {
                if (!bullet->isActive()) continue;
                
                for (auto* enemy : enemies) {
                    if (!enemy->isActive()) continue;
                    
                    // Test de colisión píxel a píxel
                    if (bullet->coll && enemy->coll) {
                        if (bullet->coll->testHierarchical(enemy->coll)) {
                            std::cout << "¡Impacto! Enemigo alcanzado" << std::endl;
                            enemy->takeDamage(bullet->getDamage());
                            bullet->setActive(false);
                            break;
                        }
                    }
                }
            }
        }
        
        // 4. Detectar colisiones enemigo-jugador
        if (player->isActive()) {
            for (auto* enemy : enemies) {
                if (!enemy->isActive()) continue;
                
                if (player->coll && enemy->coll) {
                    if (player->coll->testHierarchical(enemy->coll)) {
                        std::cout << "¡Colisión con enemigo!" << std::endl;
                        player->takeDamage(10.0f);
                        enemy->setActive(false);
                    }
                }
            }
        }

        // Renderizar todo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Dibujar jugador
        if (player->isActive()) {
            render.drawGL(player);
        }
        
        // Dibujar enemigos
        for (auto* enemy : enemies) {
            if (enemy->isActive()) {
                render.drawGL(enemy);
            }
        }
        
        // Dibujar balas
        if (player->isActive()) {
            auto& bullets = player->getBullets();
            for (auto* bullet : bullets) {
                if (bullet->isActive()) {
                    render.drawGL(bullet);
                }
            }
        }

        glfwSwapBuffers(render.window);
        
        // Salir si el jugador muere
        if (!player->isActive()) {
            std::cout << "Presiona cualquier tecla para salir..." << std::endl;
            std::cin.get();
            break;
        }
    }

    // Cleanup
    delete player;
    for (auto* enemy : enemies) {
        delete enemy;
    }

    return 0;
}
/**/