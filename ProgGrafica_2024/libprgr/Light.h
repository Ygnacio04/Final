#pragma once
#include "common.h"
#include "vectorMath.h"


class Light {
public:
    typedef enum {
        DIRECTIONAL=0,
        POINT=1
    }lightTypes_e;
    vector4f pos = { 0,0,0,1 };
    vector4f color = { 0.5f,0,0,1 };
    float I=1.0f;

    lightTypes_e type = DIRECTIONAL;
    vector4f direction = { 0,0,0,0 };

    
    Light(lightTypes_e type,vector4f pos, vector4f color, float I, vector4f direction = { 0,0,0,0 }) :
        type(type),pos(pos),color(color),I(I),direction(direction)
    {};

    void update();
};