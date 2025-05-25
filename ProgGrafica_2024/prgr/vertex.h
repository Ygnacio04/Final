#ifndef VERTEX_H
#define VERTEX_H

#include "lib.h"
using namespace lib;

typedef struct {
    Vector4f position;
    Vector4f color;
    Vector4f vNormal;
    Vector4f vTextureCoord;
} Vertex;

#endif