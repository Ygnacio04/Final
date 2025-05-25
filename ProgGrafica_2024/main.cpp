#include <iostream>
#define GLAD_BIN
#define _USE_MATH_DEFINES
#include <math.h>

#include "prgr/lib.h"
#include "Render/Render.h"
#include "Object3D/Object3D.h"
#include "InputManager/InputManager.h"
#include "OrbitalLight.h"

using namespace std;
using namespace lib;

int main() {
    Render render(640, 480);
    render.initGL();

    Camera* cam = new Camera({0,0,3,1},
                            {0,0,0,0},
                            {0,0,0,1},
                            {0,1,0,0},
		90.0f, 640.0f / 480.0f, 0.1f, 100.0f);
    render.putCamera(cam);

    Light* light = new Light(
        { 0,0,0,1 },
        { 3,3,3,1 },
        { 0,0,1,1 },
        0.2f,
        0.8f,
        0.5f,
        Light::POINT
    );
     
    OrbitalLight* light2 = new OrbitalLight(
        { 0, 0, 0, 1 },
        { -3, 3, 0, 1 },
        { 1, 0, 0, 1 },
        0.1f, 0.7f, 0.3f,
        Light::POINT,
        { 0, 0, 0, 1 },
        5.0f,
        1.0f
    );

    render.putLight(light);
	render.putLight(light2);

    // Carga el cubo
    Object3D* cubo = new Object3D("./data/kubo.fiis");
    cubo->position = { 4.0f, 0.0f, 0.0f, 1.0f };
    cubo->rotation.z = 45.0f;
    cubo->rotation.x = 45.0f;

    // Carga la esfera
    Object3D* sphere = new Object3D("./data/sphere.fiis");
    sphere->position = { -4.0f, 0.0f, 0.0f, 1.0f };
    sphere->rotation.z = 45.0f;
    sphere->rotation.x = 45.0f;

    render.putObject(cubo->id, cubo);
    render.putObject(sphere->id, sphere);

    render.mainLoop();
    return 0;
}