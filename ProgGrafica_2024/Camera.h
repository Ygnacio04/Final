#pragma once
#include "./prgr/vertex.h"
#include "./prgr/lib.h"
#include "Collider.h"
using namespace lib;

class Camera {
public:
	Vector4f pos;
	Vector4f rot;
	Vector4f lookAt;
	Vector4f up;
	
	float fovy;
	float aspectRatio;
	float zNear;
	float zFar;
	float yaw;
	float pitch;

	Collider* coll;

	Camera(Vector4f pos, Vector4f rot, Vector4f lookAt, Vector4f Up, float fovy, float aspectRatio,
		float zNear, float zFar);

	Matrix4x4f lookAtThis();
	Matrix4x4f perspective();
	void move(double timeStep);
};