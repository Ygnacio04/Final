#pragma once

#include "./prgr/lib.h"
#include <vector>

using namespace lib;
using std::vector;

class Collider {
public:
    /// Part�cula de colisi�n: punto acotado por un m�nimo y m�ximo
    struct Particle {
        Vector4f min;
        Vector4f max;
    };

    /// Tipos de colisionador soportados
    enum collTypes {
        sphere,
        AABB
    };

    /// Constructor vac�o
    Collider() = default;

    /// Destructor virtual
    virtual ~Collider() = default;

    /// A�ade una part�cula al colisionador
    virtual void addParticle(Particle part) = 0;

    /// Comprueba colisi�n con otro colisionador
    virtual bool test(Collider* c2) = 0;

    /// Actualiza el colisionador a partir de una matriz de transformaci�n
    virtual void update(Matrix4x4f mat) = 0;

    /// Subdivide en colisionadores hijos (opcional)
    virtual void subdivide() = 0;

    void clearParticles() {
        partList.clear();
    }

protected:
    collTypes                type;      ///< Tipo concreto de este colisionador
    vector<Particle>         partList;  ///< Lista de part�culas (puntos) que encierra
    vector<Collider*>        children;  ///< Hijos tras subdividir (si aplica)
};