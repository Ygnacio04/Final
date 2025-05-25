#include "Camera.h"
#include "EventManager.h"

Camera::Camera(vector4f pos, vector4f up, vector4f lookAt) :pos(pos), lookAt(lookAt), up(up) {
    vertexLookAt = lookAt - pos;
    vertexLookAt.w = 1;

    mouseX = EventManager::mouseState.x;
    mouseY = EventManager::mouseState.y;


}

matrix4x4f Camera::computeViewMatrix()
{
    matrix4x4f view = make_identityf();
    vector4f f = normalize(lookAt - pos);
    vector4f r = normalize(cross(f, normalize(up)));
    vector4f u = normalize(cross(r, f));
    vector4f p = { -dot(r, pos), -dot(u, pos), dot(f, pos), 1 };

    view.rows[0] = r;
    view.rows[1] = u;
    view.rows[2] = -1.0f * f;
    view.mat2D[0][3] = p.x;
    view.mat2D[1][3] = p.y;
    view.mat2D[2][3] = p.z;

    return view;
}

matrix4x4f Camera::computeProjectionMatrix()
{
    matrix4x4f proj = make_identityf();

    double fovy = toRadians(this->fovy);

    proj.mat2D[0][0] = 1.0f /(aspectRatio * std::tan(fovy * 0.5f));
    proj.mat2D[1][1] = 1.0f / std::tan(fovy * 0.5f);
    proj.mat2D[2][2] = -(zNear + zFar) / (zFar - zNear);
    proj.mat2D[3][3] = 1;
    proj.mat2D[2][3] =  2.0f*(zNear * zFar) / (zFar - zNear);
    proj.mat2D[3][2] = -1;

    return proj;
}

void Camera::update(float timeStep)
{
    if (EventManager::keyState[GLFW_KEY_LEFT_SHIFT])
    {
        float mouseDirX = mouseX - EventManager::mouseState.x;
        float mouseDirY = mouseY - EventManager::mouseState.y;
        float speed = 1.0f;

        float speedRot = 10.0f;
        vector4f speedRotv = { speedRot * mouseDirY,speedRot * mouseDirX,0,0 };
        //actualizar nuevas posiciones
        mouseX = EventManager::mouseState.x;
        mouseY = EventManager::mouseState.y;


        vector4f direction = normalize(pos - lookAt);
        vector4f strafeDirection = normalize(cross(up, direction));

        if (EventManager::keyState[GLFW_KEY_W]) { pos = pos - speed * timeStep * direction; }
        if (EventManager::keyState[GLFW_KEY_S]) { pos = pos + speed * timeStep * direction; }
        if (EventManager::keyState[GLFW_KEY_A]) { pos = pos - speed * timeStep * strafeDirection; }
        if (EventManager::keyState[GLFW_KEY_D]) { pos = pos + speed * timeStep * strafeDirection; }

        rot = rot + speedRotv * timeStep;

        matrix4x4f model = make_traslate(pos.x, pos.y, pos.z);
        model = model * make_rotate(0, rot.y, 0);
        model = model * make_rotate(rot.x, 0, 0);
        lookAt = model * vertexLookAt;
    }
}
