#include "Sphere.h"
#include <algorithm>
#include <cmath>

using namespace lib;
using std::vector;

Sphere::Sphere()
    : center(make_vector4f(0.0f, 0.0f, 0.0f, 1.0f))
    , radius(0.0f)
{
    type = collTypes::sphere;
}

Sphere::~Sphere() {
    for (auto* child : children) {
        delete child;
    }
    children.clear();
}

void Sphere::addParticle(Particle part) {
    partList.push_back(part);
}

bool Sphere::test(Collider* c2) {
    auto* s2 = dynamic_cast<Sphere*>(c2);
    if (!s2) return false;

    float dx = center.x - s2->center.x;
    float dy = center.y - s2->center.y;
    float dz = center.z - s2->center.z;
    float dist = std::sqrt(dx * dx + dy * dy + dz * dz);
    return dist <= (radius + s2->radius);
}

void Sphere::update(Matrix4x4f mat) {
    calculateBounds();

    // Aplica transformación al centro
    center = multiplyMxV(mat, center);

    // Extrae factor de escala máximo de la matriz
    float sx = std::sqrt(
        mat.mat2D[0][0] * mat.mat2D[0][0] +
        mat.mat2D[0][1] * mat.mat2D[0][1] +
        mat.mat2D[0][2] * mat.mat2D[0][2]
    );
    float sy = std::sqrt(
        mat.mat2D[1][0] * mat.mat2D[1][0] +
        mat.mat2D[1][1] * mat.mat2D[1][1] +
        mat.mat2D[1][2] * mat.mat2D[1][2]
    );
    float sz = std::sqrt(
        mat.mat2D[2][0] * mat.mat2D[2][0] +
        mat.mat2D[2][1] * mat.mat2D[2][1] +
        mat.mat2D[2][2] * mat.mat2D[2][2]
    );
    float s = std::max({ sx, sy, sz });
    radius *= s;

    // Actualiza los hijos si existen
    for (auto* child : children) {
        child->update(mat);
    }

}

void Sphere::subdivide() {
    subdivideRecursive(0);
}

void Sphere::subdivideRecursive(int depth) {
    // Condiciones de parada
    if (depth >= MAX_DEPTH || !shouldSubdivide()) {
        return;
    }

    std::cout << "Subdividiendo en profundidad " << depth
        << " con " << partList.size() << " partículas" << std::endl;

    createChildren();
    assignParticlesToChildren();

    // Subdivide recursivamente cada hijo
    for (auto* child : children) {
        static_cast<Sphere*>(child)->subdivideRecursive(depth + 1);
    }
}

bool Sphere::shouldSubdivide() const {
    return partList.size() > MAX_PARTICLES_PER_NODE;
}

void Sphere::createChildren() {
    // Crear 4 subesferas (cuadrantes en el plano XY)
    children.clear();

    float halfRadius = radius * 0.5f;
    float quarterRadius = radius * 0.25f;

    // Posiciones de los 4 cuadrantes
    Vector4f offsets[4] = {
        make_vector4f(-quarterRadius, -quarterRadius, 0.0f, 0.0f), // Inferior izquierdo
        make_vector4f(quarterRadius, -quarterRadius, 0.0f, 0.0f), // Inferior derecho
        make_vector4f(-quarterRadius,  quarterRadius, 0.0f, 0.0f), // Superior izquierdo
        make_vector4f(quarterRadius,  quarterRadius, 0.0f, 0.0f)  // Superior derecho
    };

    for (int i = 0; i < 4; i++) {
        Sphere* child = new Sphere();
        child->center = make_vector4f(
            center.x + offsets[i].x,
            center.y + offsets[i].y,
            center.z + offsets[i].z,
            1.0f
        );
        child->radius = halfRadius;
        children.push_back(child);
    }
}

void Sphere::assignParticlesToChildren() {
    if (children.size() != 4) return;

    // Distribuye las partículas entre los 4 hijos según su posición
    for (const auto& particle : partList) {
        // Calcula el centro de la partícula
        Vector4f particleCenter = make_vector4f(
            (particle.min.x + particle.max.x) * 0.5f,
            (particle.min.y + particle.max.y) * 0.5f,
            (particle.min.z + particle.max.z) * 0.5f,
            1.0f
        );

        // Determina a qué cuadrante pertenece
        int quadrant = 0;
        if (particleCenter.x >= center.x) quadrant += 1; // Derecha
        if (particleCenter.y >= center.y) quadrant += 2; // Arriba

        // Añade la partícula al hijo correspondiente
        if (quadrant < 4) {
            static_cast<Sphere*>(children[quadrant])->addParticle(particle);
        }
    }

    // Recalcula bounds de cada hijo
    for (auto* child : children) {
        static_cast<Sphere*>(child)->calculateBounds();
    }

    // Limpia las partículas del padre (ahora están en los hijos)
    partList.clear();
}

void Sphere::calculateBounds() {
    if (partList.empty()) {
        center = make_vector4f(0.0f, 0.0f, 0.0f, 1.0f);
        radius = 0.0f;
        return;
    }
    // Calcula bounds locales
    float minX = partList[0].min.x, maxX = partList[0].max.x;
    float minY = partList[0].min.y, maxY = partList[0].max.y;
    float minZ = partList[0].min.z, maxZ = partList[0].max.z;

    for (const auto& p : partList) {
        minX = std::min(minX, p.min.x);
        maxX = std::max(maxX, p.max.x);
        minY = std::min(minY, p.min.y);
        maxY = std::max(maxY, p.max.y);
        minZ = std::min(minZ, p.min.z);
        maxZ = std::max(maxZ, p.max.z);
    }

    // Centro = punto medio del bounding box
    center = make_vector4f(
        (minX + maxX) * 0.5f,
        (minY + maxY) * 0.5f,
        (minZ + maxZ) * 0.5f,
        1.0f
    );

    // Radio = distancia máxima desde el centro a cualquier punto
    radius = 0.0f;
    for (const auto& p : partList) {
        // Calcula distancia a las 8 esquinas del bounding box de la partícula
        Vector4f corners[8] = {
            make_vector4f(p.min.x, p.min.y, p.min.z, 1.0f),
            make_vector4f(p.max.x, p.min.y, p.min.z, 1.0f),
            make_vector4f(p.min.x, p.max.y, p.min.z, 1.0f),
            make_vector4f(p.max.x, p.max.y, p.min.z, 1.0f),
            make_vector4f(p.min.x, p.min.y, p.max.z, 1.0f),
            make_vector4f(p.max.x, p.min.y, p.max.z, 1.0f),
            make_vector4f(p.min.x, p.max.y, p.max.z, 1.0f),
            make_vector4f(p.max.x, p.max.y, p.max.z, 1.0f)
        };

        for (const auto& corner : corners) {
            float dx = corner.x - center.x;
            float dy = corner.y - center.y;
            float dz = corner.z - center.z;
            float d = std::sqrt(dx * dx + dy * dy + dz * dz);
            radius = std::max(radius, d);
        }
    }
}