#include "Light.h"

Light::Light(Vector4f direction, Vector4f position, Vector4f color, 
            float ka, float kd, float ks, LightType type)
{
    this->direction = direction;
    this->position = position;
    this->color = color;
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->type = type;
}

void Light::move(double timeStep)
{

}
