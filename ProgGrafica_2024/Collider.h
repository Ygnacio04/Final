#pragma once

#include "./prgr/lib.h"
#include <vector>

using namespace lib;
using std::vector;

class Collider {
public:
    /// Partícula de colisión: punto acotado por un mínimo y máximo
    struct Particle {
        Vector4f min;
        Vector4f max;
    };

    /// Tipos de colisionador soportados
    enum collTypes {
        sphere,
        AABB
    };

    /// Constructor vacío
    Collider() = default;

    /// Destructor virtual
    virtual ~Collider() = default;

    /// Añade una partícula al colisionador
    virtual void addParticle(Particle part) = 0;

    /// Comprueba colisión con otro colisionador
    virtual bool test(Collider* c2) = 0;

    /// Actualiza el colisionador a partir de una matriz de transformación
    virtual void update(Matrix4x4f mat) = 0;

    /// Subdivide en colisionadores hijos (opcional)
    virtual void subdivide() = 0;

    void clearParticles() {
        partList.clear();
    }

protected:
    collTypes                type;      ///< Tipo concreto de este colisionador
    vector<Particle>         partList;  ///< Lista de partículas (puntos) que encierra
    vector<Collider*>        children;  ///< Hijos tras subdividir (si aplica)
};