#pragma once
#include "Light.h"
#include <cmath>

class OrbitalLight : public Light {
public:
	OrbitalLight(const Vector4f &direction,
		const Vector4f &position,
		const Vector4f &color,
		float ka, float kd, float ks,
		LightType type,
		const Vector4f &center,
		float radius,
		float speed);

	void move(double timeStep) override;

private:
	Vector4f _center;
	float _radius;
	float _speed;
	float _angle;
};