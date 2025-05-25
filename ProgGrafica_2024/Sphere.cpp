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

Sphere::~Sphere() {}

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
    if (partList.empty()) {
        center = make_vector4f(0.0f, 0.0f, 0.0f, 1.0f);
        radius = 0.0f;
    }
    else {
        // Calcula bounds locales
        float minX = partList[0].min.x, maxX = partList[0].max.x;
        float minY = partList[0].min.y, maxY = partList[0].max.y;
        float minZ = partList[0].min.z, maxZ = partList[0].max.z;

        for (auto& p : partList) {
            minX = std::min(minX, p.min.x);
            maxX = std::max(maxX, p.max.x);
            minY = std::min(minY, p.min.y);
            maxY = std::max(maxY, p.max.y);
            minZ = std::min(minZ, p.min.z);
            maxZ = std::max(maxZ, p.max.z);
        }

        center = make_vector4f(
            (minX + maxX) * 0.5f,
            (minY + maxY) * 0.5f,
            (minZ + maxZ) * 0.5f,
            1.0f
        );

        radius = 0.0f;
        for (auto& p : partList) {
            float dx = p.min.x - center.x;
            float dy = p.min.y - center.y;
            float dz = p.min.z - center.z;
            float d = std::sqrt(dx * dx + dy * dy + dz * dz);
            radius = std::max(radius, d);
        }
    }

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
}

void Sphere::subdivide() {
    // No implementado para esfera
}