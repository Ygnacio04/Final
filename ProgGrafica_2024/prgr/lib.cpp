#include "lib.h"
#include "../prgr/common.h"
#include <cmath>
#include <iostream>
using namespace std;

namespace lib {
    /* MÉTODOS PARA VECTORES */

    // Método para crear un Vector4f
    Vector4f make_vector4f(float x, float y, float z, float w) {
        Vector4f v;
        v.x = x;
        v.y = y;
        v.z = z;
        v.w = w;
        return v;
    }

    // Método para normalizar un Vector4f
    Vector4f normalize(Vector4f v) {
        float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z); // Cálculo de la magnitud/longitud del vector

        if(length > 0) {
            return Vector4f{v.x / length, v.y / length, v.z / length, 0}; // Devuelve el vector normalizado si la longitud es mayor que 0
        }else {
        return Vector4f{0, 0, 0, 0}; // Caso seguro para evitar NaN o división por cero
        }
    }

    // Operación suma
    Vector4f sum(Vector4f a, Vector4f b) {
        return Vector4f{a.x + b.x,
                        a.y + b.y, 
                        a.z + b.z, 
                        a.w + b.w};
    }

    // Operación resta
    Vector4f subtract(Vector4f a, Vector4f b) {
        return Vector4f{a.x - b.x,
                        a.y - b.y, 
                        a.z - b.z, 
                        a.w - b.w};
    }

    // Operación multiplicación escalar
    Vector4f dotProduct(Vector4f a, Vector4f b) {
        return Vector4f{a.x * b.x,
                        a.y * b.y, 
                        a.z * b.z, 
                        a.w * b.w};
    }

    // Operación multiplicación vectorial
    Vector4f crossProduct(Vector4f a, Vector4f b) {
        return Vector4f{a.y * b.z - a.z * b.y,
                        a.z * b.x - a.x * b.z, 
                        a.x * b.y - a.y * b.x, 
                        0};
    }

    // Método para crear un cuaternión
    Vector4f make_quaternion(float x, float y, float z, float angle) {
        // Un cuaternión es un vector de 4 dimensiones
        // Sigue la siguiente fórmula: q = (x * sin(a/2), y * sin(a/2), z * sin(a/2), cos(a/2))
        // donde x, y, z son los componentes del vector y a es el ángulo de rotación

        Vector4f quaternion;

        quaternion.x = x * sin(angle / 2);
        quaternion.y = y * sin(angle / 2);
        quaternion.z = z * sin(angle / 2);
        quaternion.w = cos(angle / 2);

        return quaternion;
    }


    /* MÉTODOS PARA MATRICES */

    // Método para hacer la matriz identidad
    Matrix4x4f make_identity() {
        return Matrix4x4f{1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 1};
    }

    // Método para crear una matriz de traslación
    Matrix4x4f make_translate(float x, float y, float z) {
        return Matrix4x4f{1, 0, 0, x,
                          0, 1, 0, y,
                          0, 0, 1, z,
                          0, 0, 0, 1};
    }

    // Método para crear una matriz de rotación
    Matrix4x4f make_rotate(float angleX, float angleY, float angleZ) {
            
        float cosX = cos(angleX), sinX = sin(angleX);
        float cosY = cos(angleY), sinY = sin(angleY);
        float cosZ = cos(angleZ), sinZ = sin(angleZ);

        // Matriz de rotación en X
        Matrix4x4f rotX = {1,    0,     0,  0,
                            0,  cosX, -sinX, 0,
                            0,  sinX,  cosX, 0,
                            0,    0,     0,  1};

        // Matriz de rotación en Y
        Matrix4x4f rotY = { cosY, 0, sinY, 0,
                               0, 1,    0, 0,
                            -sinY, 0, cosY, 0,
                               0, 0,    0, 1};

        // Matriz de rotación en Z
        Matrix4x4f rotZ = {cosZ, -sinZ, 0, 0,
                           sinZ,  cosZ, 0, 0,
                              0,     0, 1, 0,
                              0,     0, 0, 1};

        // Combinación de rotaciones (Rz * Ry * Rx)
        return multiplyMatrices(multiplyMatrices(rotX, rotY), rotZ);
    }

    // Método para crear una matriz de escalado
    Matrix4x4f make_scale(float x, float y, float z) {
        Matrix4x4f scaleMatrix = {}; // Inicialización en ceros

        scaleMatrix.mat2D[0][0] = x; // Escala en X
        scaleMatrix.mat2D[1][1] = y; // Escala en Y
        scaleMatrix.mat2D[2][2] = z; // Escala en Z
        scaleMatrix.mat2D[3][3] = 1; // Última coordenada homogénea

        return scaleMatrix;
    }

    // Operación suma
    Matrix4x4f sum(Matrix4x4f a, Matrix4x4f b) {
        Matrix4x4f result = {};

        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                result.mat2D[i][j] = a.mat2D[i][j] + b.mat2D[i][j];
            }
        }

        return result;
    }

    // Operación resta
    Matrix4x4f subtract(Matrix4x4f a, Matrix4x4f b) {
        Matrix4x4f result = {};

        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                result.mat2D[i][j] = a.mat2D[i][j] - b.mat2D[i][j];
            }
        }

        return result;
    }

    // Operación multiplicación escalar de dos matrices
    float dotProductMxM(Matrix4x4f a, Matrix4x4f b) {
         float result = 0.0f;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result += a.mat2D[i][j] * b.mat2D[i][j];
            }
        }

        return result;
    }

    // Operación multiplicación vectorial de dos matrices
    Matrix4x4f multiplyMatrices(Matrix4x4f a, Matrix4x4f b) {
        Matrix4x4f result = {};

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    result.mat2D[i][j] += a.mat2D[i][k] * b.mat2D[k][j];
                }
            }
        }

        return result;
    }

    // Operación multiplicación de una matriz por un vector
    Vector4f multiplyMxV(Matrix4x4f a, Vector4f v) {
        Vector4f result = {};

        for(int i = 0; i < 4; i++) {
            result.data[i] = a.rows[i].x * v.x + a.rows[i].y * v.y + a.rows[i].z * v.z + a.rows[i].w * v.w;
        }

        return result;
    }

    // Matriz transpuesta
    Matrix4x4f transpose(Matrix4x4f a) {
        Matrix4x4f result;

        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {

                result.mat2D[i][j] = a.mat2D[j][i];
            }
        }

        return result;
    }

    // Matriz inversa
    Matrix4x4f inverse(Matrix4x4f a) {
        if(det4x4(a) == 0) { // Si el determinante de a es 0, no tiene inversa
            cerr << "La matriz no tiene inversa" << endl;
            return make_identity();// Devuelve la matriz identidad como caso seguro
        }

        Matrix4x4f adj; // Matriz adjunta
        float subMat[3][3]; // Submatriz 3x3 para calcular cofactores

        for(int row = 0; row < 4; row++) { // Cálculo de la matriz de cofactores
            for(int col = 0; col < 4; col++) {
                int subi = 0;

                for(int i = 0; i < 4; i++) {
                    if(i != row) { // Se omite la fila actual
                        int subj = 0;

                        for(int j = 0; j < 4; j++) {
                            if(j != col) { // Se omite la columna actual
                                subMat[subi][subj] = a.mat2D[i][j];
                                subj++;
                            }
                        }

                        subi++;
                    }
                }

                // Cálculo del cofactor con signo alterado
                float cofactor = ((row + col) % 2 == 0 ? 1 : -1) * det3x3(subMat);
                adj.mat2D[col][row] = cofactor;
            }
        }
        float det = det4x4(a);//Determinante Matriz Original

        for(int i = 0; i < 4; i++) { // Cálculo de la matriz inversa
            for(int j = 0; j < 4; j++) {
                adj.mat2D[i][j] /= det;
            }
        }

        return adj; // Devuelve la matriz inversa
    }

    // Determinante de una matriz 3x3
    float det3x3(float mat[3][3]) {
        return mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) -
               mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]) +
               mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
    }

    // Determinante de una matriz 4x4
    float det4x4(Matrix4x4f mat) {
        float det = 0;
        float subMat[3][3];
        int sign = 1;
        
        for(int col = 0; col < 4; col++) { // Crear la submatriz 3x3 eliminando la fila 0 y la columna actual
            int subi = 0;

            for(int i = 1; i < 4; i++) { // Se omite la primera fila
                int subj = 0;

                for(int j = 0; j < 4; j++) {
                    if(j != col) { // Se omite la columna actual
                        subMat[subi][subj] = mat.mat2D[i][j];
                        subj++;
                    }
                }

                subi++;
            }

            // Aplicación de la fórmula de Laplace
            det += sign * mat.mat2D[0][col] * det3x3(subMat);
            sign = -sign;
        }

        return det;
    }

    // Método para hacer una rotación a partir de un cuaternión
    Matrix4x4f make_rotate(Vector4f q) { 
        Matrix4x4f rotationMatrix = {}; // Inicializa con ceros

        rotationMatrix.mat2D[0][0] = 1 - 2 * (q.y * q.y + q.z * q.z);
        rotationMatrix.mat2D[0][1] = 2 * (q.x * q.y - q.z * q.w);
        rotationMatrix.mat2D[0][2] = 2 * (q.x * q.z + q.y * q.w);
        rotationMatrix.mat2D[0][3] = 0;

        rotationMatrix.mat2D[1][0] = 2 * (q.x * q.y + q.z * q.w);
        rotationMatrix.mat2D[1][1] = 1 - 2 * (q.x * q.x + q.z * q.z);
        rotationMatrix.mat2D[1][2] = 2 * (q.y * q.z - q.x * q.w);
        rotationMatrix.mat2D[1][3] = 0;

        rotationMatrix.mat2D[2][0] = 2 * (q.x * q.z - q.y * q.w);
        rotationMatrix.mat2D[2][1] = 2 * (q.y * q.z + q.x * q.w);
        rotationMatrix.mat2D[2][2] = 1 - 2 * (q.x * q.x + q.y * q.y);
        rotationMatrix.mat2D[2][3] = 0;

        rotationMatrix.mat2D[3][0] = 0;
        rotationMatrix.mat2D[3][1] = 0;
        rotationMatrix.mat2D[3][2] = 0;
        rotationMatrix.mat2D[3][3] = 1;

        return rotationMatrix;
    }
}