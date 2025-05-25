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

    /// A�ade una part�cula al colisionador
    void addParticle(Particle part) override;

    /// Test de colisi�n esfera-esfera
    bool test(Collider* c2) override;

    /// Reconstruye la esfera y aplica la transformaci�n
    void update(Matrix4x4f mat) override;

    /// No subdividimos para esferas
    void subdivide() override;
};