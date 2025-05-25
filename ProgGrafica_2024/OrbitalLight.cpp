#include "OrbitalLight.h"
using namespace std;

void OrbitalLight::move(double timeStep)
{
	_angle += _speed * (float)timeStep;

	this->position.x = _center.x + _radius * cos(_angle);
	this->position.y = _center.y + _radius * sin(_angle);

	this->position.x = _center.x + _radius * cos(_angle);
	this->position.z = _center.z + _radius * sin(_angle);
	this->position.y = _center.y;
}

OrbitalLight::OrbitalLight(
    const Vector4f& direction,
    const Vector4f& position,
    const Vector4f& color,
    float ka, float kd, float ks,
    LightType type,
    const Vector4f& center,
    float radius,
    float speed
)
    : Light(direction, position, color, ka, kd, ks, type)
    , _center(center)
    , _radius(radius)
    , _speed(speed)
    , _angle(0.0f)
{
    this->position.x = _center.x + _radius * std::cos(_angle);
    this->position.y = _center.y;
    this->position.z = _center.z + _radius * std::sin(_angle);
    this->position.w = 1.0f;
}