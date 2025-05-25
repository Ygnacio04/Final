#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <GLFW/stb_image.h>

void Texture::loadFile(string fileName)
{
    int channel = 4;
    //cargar datos de fichero
    unsigned char* data = stbi_load(fileName.c_str(), &w, &h, &channel, 4);
    //si no hay error
    if (data != nullptr)
    {
        //copiar a vector de datos de pixel
        pixels.resize(w * h); //redimensionar a  w * h
        memcpy(pixels.data(), data, pixels.size() * sizeof(pixel_t)); //copiar datos en fichero
        stbi_image_free(data); //borra datos de imagen
    }
    else
        cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " fichero no encontrado" << endl;
}

void Texture::updateGPU()
{

    //crear identificador de textura
    if (textureID == -1)
    { //no se ha creado aún, se crea
        glGenTextures(1, &textureID);
    }

    //bindear/atar textura a unidad activa
    glBindTexture(GL_TEXTURE_2D, textureID);
    //set propiedades de filtro imagen
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //filtro bilinear por defecto
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //set propiedades de repetición de imagen
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//repetición eje x
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//repetición en eje y
    //copiar datos a gpu
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
}