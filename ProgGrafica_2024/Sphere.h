#pragma once

#include "Collider.h"
#include "./prgr/lib.h"
#include <vector>

using namespace lib;
using std::vector;

class Sphere : public Collider {
public:
    Vector4f center;
    float radius;

    Sphere();
    virtual ~Sphere();

    /// Añade una partícula al colisionador
    void addParticle(Particle part) override;

    /// Test de colisión esfera-esfera
    bool test(Collider* c2) override;

    /// Reconstruye la esfera y aplica la transformación
    void update(Matrix4x4f mat) override;

    /// No subdividimos para esferas
    void subdivide() override;
};