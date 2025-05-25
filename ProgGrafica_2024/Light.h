#pragma once
#include "./prgr/common.h"
#include "./prgr/lib.h"

using namespace lib;

class Light {
public:
    typedef enum {
        DIRECTIONAL=0,
        POINT=1
    } LightType;

    Vector4f direction = { 1,0,0,1 }, position = { 0,0,0,1 }, color = {0.5f,0,0,1};
    float ka, kd, ks;
    LightType type;

    Light(Vector4f direction, Vector4f position, Vector4f color, float ka, float kd, float ks, LightType type);

    virtual void move(double timeStep);
};
