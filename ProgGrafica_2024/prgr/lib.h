#pragma once
#include "lib.h"
#include "../prgr/common.h"
#include <cmath>
#include <iostream>
using namespace std;

namespace lib {
    // Estructura Vector4f
    typedef struct Vector4f {
        union {
            struct {
                float x, y, z, w;
            };

            struct {
                float r, g, b, a;
            };

            float data[4];
        };
    } Vector4f;

    // Estructura Matrix4x4f
    typedef struct Matrix4x4f {
        union {
            float mat2D[4][4];
            float mat1[16];
            Vector4f rows[4];
        };
    } Matrix4x4f;

    Vector4f make_vector4f(float x, float y, float z, float w);
    Vector4f normalize(Vector4f v);
    Vector4f sum(Vector4f a, Vector4f b);
    Vector4f subtract(Vector4f a, Vector4f b);
    Vector4f dotProduct(Vector4f a, Vector4f b);
    Vector4f crossProduct(Vector4f a, Vector4f b);
    Vector4f make_quaternion(float x, float y, float z, float angle);
    inline float dot(const Vector4f& v1, const Vector4f& v2);
    inline Vector4f operator*(const Vector4f& v, float scalar);
    inline Vector4f operator*(float scalar, const Vector4f& v);
    inline Matrix4x4f operator*(const Matrix4x4f& m1, const Matrix4x4f& m2);

    Matrix4x4f make_identity();
    Matrix4x4f make_translate(float x, float y, float z);
    Matrix4x4f make_rotate(float angleX, float angleY, float angleZ);
    Matrix4x4f make_scale(float x, float y, float z);
    Matrix4x4f sum(Matrix4x4f a, Matrix4x4f b);
    Matrix4x4f subtract(Matrix4x4f a, Matrix4x4f b);
    float dotProductMxM(Matrix4x4f a, Matrix4x4f b);
    Matrix4x4f multiplyMatrices(Matrix4x4f a, Matrix4x4f b);
    Vector4f multiplyMxV(Matrix4x4f a, Vector4f v);
    Matrix4x4f transpose(Matrix4x4f a);
    Matrix4x4f inverse(Matrix4x4f a);
    float det3x3(float mat[3][3]);
    float det4x4(Matrix4x4f mat);
    Matrix4x4f make_rotate(Vector4f quaternion);

    inline float toRadians(float angle) {
        return angle * M_PI / (180.0f);
    }

    inline Vector4f operator-(const Vector4f& v1, const Vector4f& v2) {
        return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
    }

    inline Vector4f operator-(const Vector4f& v) {
        return { -v.x, -v.y, -v.z, -v.w };
    }

    inline float dot(const Vector4f& v1, const Vector4f& v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    inline Vector4f operator*(const Vector4f& v, float scalar) {
        return Vector4f{ v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar };
    }

    inline Vector4f operator*(float scalar, const Vector4f& v) {
        return v * scalar;
    }

    inline Matrix4x4f operator*(const Matrix4x4f& m1, const Matrix4x4f& m2) {
        Matrix4x4f result = make_identity();
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.mat2D[i][j] = 0.0f;
                for (int k = 0; k < 4; k++) {
                    result.mat2D[i][j] += m1.mat2D[i][k] * m2.mat2D[k][j];
                }
            }
        }
        return result;
    }
}