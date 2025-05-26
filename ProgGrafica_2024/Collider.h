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

    bool isLeaf() const {
        return children.empty(); 
    }
    /// A�ade un hijo al colisionador
    void addChild(Collider* child) {
        children.push_back(child);
    }

    /// Obtiene la lista de hijos
    const vector<Collider*>& getChildren() const {
        return children;
    }

    /// Obtiene el n�mero de part�culas
    size_t getParticleCount() const {
        return partList.size();
    }

    /// Test de colisi�n jer�rquico (nuevo m�todo)
    virtual bool testHierarchical(Collider* c2) {
        // Si ambos son hojas, usa test normal
        if (isLeaf() && c2->isLeaf()) {
            return test(c2);
        }

        // Si este tiene hijos, testea cada hijo
        if (!isLeaf()) {
            for (auto* child : children) {
                if (child->testHierarchical(c2)) {
                    return true;
                }
            }
        }

        // Si el otro tiene hijos, testea cada hijo
        if (!c2->isLeaf()) {
            for (auto* child : c2->getChildren()) {
                if (testHierarchical(child)) {
                    return true;
                }
            }
        }

        return false;
    }

protected:
    collTypes                type;      ///< Tipo concreto de este colisionador
    vector<Particle>         partList;  ///< Lista de part�culas (puntos) que encierra
    vector<Collider*>        children;  ///< Hijos tras subdividir (si aplica)
};