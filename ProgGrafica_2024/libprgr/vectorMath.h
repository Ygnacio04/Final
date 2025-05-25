#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

namespace libPRGR {

    typedef struct {
        union{
            struct{
                float x, y, z, w;
            };
            struct{
                float r, g, b, a;
            };
            float data[4];
        };
    }
    vector4f;

    typedef struct {
        union {
            float mat2D[4][4];
            float mat1[16];
            vector4f rows[4];
        };
    }matrix4x4f;

    /****************************
    * OPERACIONES VECTOR4F
    ****************************/
    inline vector4f make_vector4f(float x, float y, float z, float w) {
        vector4f v;
        v.data[0] = x;
        v.data[1] = y;
        v.data[2] = z;
        v.data[3] = w;
        return v;
    }
    inline float length(vector4f v)
    {
        float length = sqrt(v.data[0] * v.data[0] +
            v.data[1] * v.data[1] +
            v.data[2] * v.data[2]);
        return length;
    }
    inline  vector4f normalize(vector4f v)
    {
        float length = sqrt(v.data[0] * v.data[0] + v.data[1] * v.data[1] + v.data[2] * v.data[2]);
        return make_vector4f(v.data[0] / length, v.data[1] / length, v.data[2] / length, 0.0f);
    }

    inline  float dot(vector4f v1, vector4f v2)
    {
        return v1.data[0] * v2.data[0] + v1.data[1] * v2.data[1] + v1.data[2] * v2.data[2] + v1.data[3] * v2.data[3];
    }

    inline  float dot3f(vector4f v1, vector4f v2)
    {
        return v1.data[0] * v2.data[0] + v1.data[1] * v2.data[1] + v1.data[2] * v2.data[2];
    }
    inline  vector4f cross(vector4f v1, vector4f v2)
    {
        return make_vector4f(v1.data[1] * v2.data[2] - v1.data[2] * v2.data[1],
            -v1.data[0] * v2.data[2] + v1.data[2] * v2.data[0],
            v1.data[0] * v2.data[1] - v1.data[1] * v2.data[0], 0.0f);

    }

    inline  vector4f operator+(vector4f v1, vector4f v2) {
        return make_vector4f(v1.data[0] + v2.data[0], v1.data[1] + v2.data[1], v1.data[2] + v2.data[2], v1.data[3] + v2.data[3]);
    }

    inline  vector4f operator-(vector4f v1, vector4f v2) {
        return make_vector4f(v1.data[0] - v2.data[0], v1.data[1] - v2.data[1], v1.data[2] - v2.data[2], v1.data[3] - v2.data[3]);
    }

    inline float operator* (vector4f v1, vector4f v2) {
    
        float res = 0;
        res = v1.x * v2.x +
            v1.y * v2.y +
            v1.z * v2.z +
            v1.w * v2.w;
        return res;
    }

    inline  vector4f operator*(vector4f v1, float v2)
    {
        return make_vector4f(v1.data[0] * v2, v1.data[1] * v2, v1.data[2] * v2, v1.data[3] * v2);

    }
    inline  vector4f operator*(float v2, vector4f v1)
    {
        return make_vector4f(v1.data[0] * v2, v1.data[1] * v2, v1.data[2] * v2, v1.data[3] * v2);

    }

    inline  vector4f operator/(float v2, vector4f v1)
    {
        return make_vector4f(v1.data[0] / v2, v1.data[1] / v2, v1.data[2] / v2, v1.data[3] / v2);

    }

    inline  vector4f operator/(vector4f v1, float v2)
    {
        return make_vector4f(v1.data[0] / v2, v1.data[1] / v2, v1.data[2] / v2, v1.data[3] / v2);

    }

    inline float distance(vector4f v1, vector4f v2)
    {
        vector4f direction = v1 - v2;
        return length(direction);

    }

    inline  bool operator==(vector4f v1, vector4f v2)
    {
        return memcmp(&v1, &v2,sizeof(vector4f))==0;//(v1.data[0] == v2.data[0]) && (v1.data[1] == v2.data[1]) && (v1.data[2] == v2.data[2]) && (v1.data[3] = v2.data[3]);

    }

    inline vector4f operator*(matrix4x4f m1, vector4f v1)
    {
        vector4f res = { 0,0,0,0 };
        for (int j = 0;j < 4;j++)
        {
            res.data[j] = m1.rows[j] * v1;
        }
        return res;
    }


    inline std::ostream& operator<<(std::ostream& os, vector4f &v1)
    {
        os << "{" << v1.data[0] << "," << v1.data[1] << "," <<
             v1.data[2] << "," << v1.data[3] << "}\n";

        return os;
    }


    inline float toRadians(float angle)
    {
        return (float) (angle * M_PI / (180.0f));
    }

    inline matrix4x4f make_identityf() {
        matrix4x4f m;
        memset(&m, 0, sizeof(matrix4x4f));
        //m.x0 = m.y1 = m.z2 = m.w3 = 1.0f;
        m.mat2D[0][0] = m.mat2D[1][1] = m.mat2D[2][2] = m.mat2D[3][3] = 1.0f;
        return m;

    }


    inline matrix4x4f make_traslate(float x, float y, float z)
    {
        matrix4x4f m = { .rows = {
            {1, 0, 0, x},
            {0, 1, 0, y},
            {0, 0, 1, z},
            {0, 0, 0, 1}} };

        return m;
    }

    inline matrix4x4f operator*(matrix4x4f m1, matrix4x4f m2)
    {
        matrix4x4f res = make_identityf();
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
            {
                res.mat2D[i][j] = 0.0f;
                for (int k = 0; k < 4; k++)
                {
                    res.mat2D[i][j] += m1.mat2D[i][k] * m2.mat2D[k][j];
                }
            }
        }

        return res;
    }

    inline matrix4x4f make_rotate(float x, float y, float z) {
        matrix4x4f m = make_identityf();

        matrix4x4f mx = make_identityf(), my = make_identityf(), mz = make_identityf();
        mx.mat2D[1][1] = (float)cos(toRadians(x));
        mx.mat2D[1][2] = -1.0f * (float)sin(toRadians(x));
        mx.mat2D[2][1] = (float)sin(toRadians(x));
        mx.mat2D[2][2] = (float)cos(toRadians(x));

        my.mat2D[0][0] = (float)cos(toRadians(y));
        my.mat2D[0][2] = (float)sin(toRadians(y));
        my.mat2D[2][0] = -1.0f * (float)sin(toRadians(y));
        my.mat2D[2][2] = (float)cos(toRadians(y));

        mz.mat2D[0][0] = (float)cos(toRadians(z));
        mz.mat2D[0][1] = -1.0f * (float)sin(toRadians(z));
        mz.mat2D[1][0] = (float)sin(toRadians(z));
        mz.mat2D[1][1] = (float)cos(toRadians(z));

        return mx * my * mz;

    }

    inline matrix4x4f make_scale(float x, float y, float z) {
        matrix4x4f m = make_identityf();
        m.mat2D[0][0] = x;
        m.mat2D[1][1] = y;
        m.mat2D[2][2] = z;
        return m;
    }



};

using namespace libPRGR;