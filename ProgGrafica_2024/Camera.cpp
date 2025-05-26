#include "Camera.h"
#include "./prgr/common.h"
#include "./prgr/lib.h"
#include "InputManager/InputManager.h"
#include "Collider.h"
#include "Sphere.h"
#include "Render/Render.h"
#define DEG2RAD 3.14159f / 180.0f
#define CUBE_ID 0

Camera::Camera(Vector4f pos, Vector4f rot, Vector4f lookAt, Vector4f Up, float fovy, float aspectRatio, float zNear, float zFar)
{
    this->pos = pos;
    this->rot = rot;
    this->lookAt = lookAt;
    this->up = Up;
    this->fovy = fovy;
    this->aspectRatio = aspectRatio;
    this->zNear = zNear;
    this->zFar = zFar;
    this->yaw = 90.0f;
    this->pitch = 0.0f;
    this->coll = new Sphere();
    {
        Collider::Particle p;
        float half = .005f;
        p.min = make_vector4f(-half, -half, -half, 1.0f);
        p.max = make_vector4f(half, half, half, 1.0f);
        coll->addParticle(p);
    }
}

Matrix4x4f Camera::lookAtThis()
{
    Vector4f front;
    front.x = cosf(yaw * DEG2RAD) * cosf(pitch * DEG2RAD);
    front.y = sinf(pitch * DEG2RAD);
    front.z = sinf(yaw * DEG2RAD) * cosf(pitch * DEG2RAD);
    front = normalize(front);
    lookAt = make_vector4f(pos.x + front.x,
                pos.y + front.y,
                pos.z + front.z,
                1.0f);
    
    Matrix4x4f view = make_identity();
    Vector4f f = normalize(subtract(this->lookAt, pos));
    Vector4f r = normalize(crossProduct(f, normalize(up)));
    Vector4f u = normalize(crossProduct(r, f));
    Vector4f p = make_vector4f(- dot(r, pos), -dot(u, pos), dot(f, pos), 1);

    view.rows[0] = r;
    view.rows[1] = u;
    view.rows[2] = f * -1.0f;
    view.mat2D[0][3] = p.x;
    view.mat2D[1][3] = p.y;
    view.mat2D[2][3] = p.z;

    return view;
}

Matrix4x4f Camera::perspective()
{
	Matrix4x4f proj = make_identity();

	double fovy = toRadians(this->fovy);
	proj.mat2D[0][0] = 1.0f / (aspectRatio * std::tan(fovy * .5f));
	proj.mat2D[1][1] = 1.0f / std::tan(fovy * .5f);
	proj.mat2D[2][2] = -1.0f * (zFar + zNear) / (zFar - zNear);
	proj.mat2D[3][2] = -1.0f;

	proj.mat2D[2][3] = 2.0f * zFar * zNear / (zFar - zNear);
	proj.mat2D[3][3] = 1;

	return proj;
}

void Camera::move(double timeStep)
{
    Vector4f oldPos = pos;
    float speed = 0.01f * timeStep;

    Vector4f front;
    front.x = cosf(this->yaw * DEG2RAD) * cosf(pitch * DEG2RAD);
    front.y = sinf(this->pitch * DEG2RAD);
    front.z = sinf(this->yaw * DEG2RAD) * cosf(pitch * DEG2RAD);
    front.w = 0.0f;
    front = normalize(front);

    Vector4f up = make_vector4f(0.0f, 1.0f, 0.0f, 0.0f);
    Vector4f right = normalize(crossProduct(front, up));

    if (InputManager::keyState[GLFW_KEY_W]) {
        pos.x += front.x * speed;
        pos.y += front.y * speed;
        pos.z += front.z * speed;
    }
    if (InputManager::keyState[GLFW_KEY_S]) {
        pos.x -= front.x * speed;
        pos.y -= front.y * speed;
        pos.z -= front.z * speed;
    }
    if (InputManager::keyState[GLFW_KEY_A]) {
        pos.x -= right.x * speed;
        pos.y -= right.y * speed;
        pos.z -= right.z * speed;
    }
    if (InputManager::keyState[GLFW_KEY_D]) {
        pos.x += right.x * speed;
        pos.y += right.y * speed;
        pos.z += right.z * speed;
    }

    static double lastMouseX = InputManager::mouseState.x;
    static double lastMouseY = InputManager::mouseState.y;
    double deltaX = InputManager::mouseState.x - lastMouseX;
    double deltaY = InputManager::mouseState.y - lastMouseY;
    lastMouseX = InputManager::mouseState.x;
    lastMouseY = InputManager::mouseState.y;

    float mouseSensitivity = 0.1f;
	this->yaw += deltaX * mouseSensitivity;
	this->pitch -= deltaY * mouseSensitivity;

    if (InputManager::keyState[GLFW_KEY_J]) { this->yaw -= 1.0f * timeStep; }
    if (InputManager::keyState[GLFW_KEY_L]) { this->yaw += 1.0f * timeStep; }
    if (InputManager::keyState[GLFW_KEY_I]) { this->pitch += 1.0f * timeStep; }
    if (InputManager::keyState[GLFW_KEY_K]) { this->pitch -= 1.0f * timeStep; }

    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    lookAt.x = pos.x + front.x;
    lookAt.y = pos.y + front.y;
    lookAt.z = pos.z + front.z;
    lookAt.w = 1.0f;

    Matrix4x4f trans = make_identity();
    trans.mat2D[0][3] = pos.x;
    trans.mat2D[1][3] = pos.y;
    trans.mat2D[2][3] = pos.z;
    coll->update(trans);

    // — comprueba colisión con el cubo de la escena — 
    if (auto cubeObj = Render::getObject(CUBE_ID)) {
        if (cubeObj->coll && coll->test(cubeObj->coll)) {
            // hay colisión: retrocedemos
            pos = oldPos;
            // recomputa lookAt tras retroceso
            lookAt.x = pos.x + normalize(subtract(this->lookAt, oldPos)).x;
            lookAt.y = pos.y + normalize(subtract(this->lookAt, oldPos)).y;
            lookAt.z = pos.z + normalize(subtract(this->lookAt, oldPos)).z;
            lookAt.w = 1.0f;
            // y actualiza el colisionador a la posición restaurada
            trans.mat2D[0][3] = pos.x;
            trans.mat2D[1][3] = pos.y;
            trans.mat2D[2][3] = pos.z;
            coll->update(trans);
        }
    }
}
