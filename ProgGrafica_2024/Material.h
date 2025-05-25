#pragma once
#include "./prgr/common.h"
#include "./Texture.h"
#include <GLFW/stb_image.h>
using namespace std;

class Material {
public:
    string textureName; //Nombre fichero de textura que se usará para colorear el objeto
    unsigned int textureId; //Identificador de la textura cargada en OpenGL
    int width, height;  //Tamaño pixeles
    float Ka, Kd, Ks; //Constantes de ilumación (ver Light)
    Texture* texture;

    // Constructor
    Material(const string& textureName, float ka, float kd, float ks);
    void bind(int textureUnit);
};
