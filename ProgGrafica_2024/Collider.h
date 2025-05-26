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

    bool isLeaf() const {
        return children.empty(); 
    }
    /// Añade un hijo al colisionador
    void addChild(Collider* child) {
        children.push_back(child);
    }

    /// Obtiene la lista de hijos
    const vector<Collider*>& getChildren() const {
        return children;
    }

    /// Obtiene el número de partículas
    size_t getParticleCount() const {
        return partList.size();
    }

    /// Test de colisión jerárquico (nuevo método)
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
    vector<Particle>         partList;  ///< Lista de partículas (puntos) que encierra
    vector<Collider*>        children;  ///< Hijos tras subdividir (si aplica)
};