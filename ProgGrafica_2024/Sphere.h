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

    static const int MAX_PARTICLES_PER_NODE = 4;
    static const int MAX_DEPTH = 3;              // Profundidad m�xima

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

    void subdivideRecursive(int depth = 0);
    bool shouldSubdivide() const;
    void distributeParticles();

    Vector4f getCenter() const { return center; }
    float getRadius() const { return radius; }

private:
    void calculateBounds();
    void createChildren();
    void assignParticlesToChildren();

};