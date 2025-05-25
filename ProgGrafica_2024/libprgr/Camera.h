#pragma once
#include "vectorMath.h"

class Camera {

public:
    vector4f pos = { 0,0,0,1 };
    vector4f up = { 0,1,0,0 }; //verticalidad de la cámara
    vector4f lookAt = { 0,0,0,1 }; //punto al que miras 
    vector4f vertexLookAt = { 0, 0, 0, 1 };

    vector4f rot={ 0,0,0,0 };
    float mouseX, mouseY;

    float fovy = 45; //ángulo de apertura/visión
    float aspectRatio = 640.0f / 480.0f;//Tamaño de ventana hardcodeado
    float zNear = 0.01f;
    float zFar = 100.0f;



    Camera(vector4f pos, vector4f up, vector4f lookAt);

    matrix4x4f computeViewMatrix();
    matrix4x4f computeProjectionMatrix();
    void update(float timeStep);

};




