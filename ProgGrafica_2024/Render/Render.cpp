#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Render.h"
#include "../prgr/common.h"
#include "../InputManager/InputManager.h"
#include <iostream>
#include <map>

using namespace std;

// Constructor
Render::Render(int h, int w) { 
    this->h = h;
    this->w = w;
    //buffer.resize(h, vector<char>(w, '0')); // Se redimensiona y se crea el buffer
    //this->centerX = w / 2;
    //this->centerY = h / 2;
    //buffer[centerY][centerX] = '+'; // Se marca el centro
}

// Destructor
Render::~Render() {
    buffer.clear();
    //Que cierre la ventana
    glfwDestroyWindow(window); 
    glfwTerminate();
}

// Método para poner un pixel
void Render::PutPixel(int x, int y) {
    //cout << "Centro en (" << centerX << ", " << centerY << ")" << endl;

    if (x + centerX >= 0 && x + centerX < w && centerY - y >= 0 && centerY - y < h) {
        buffer[centerY - y][x + centerX] = '1';
    }
}

// Método para resetear el buffer
void Render::resetBuffer() {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if(buffer[i][j] != '+') {
                buffer[i][j] = '0';
            }
        }
    }
}

// Método para dibujar el buffer
void Render::Draw() {
    cout << "INICIO DE LA MATRIZ" << endl;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if(buffer[i][j] == '0') {
                cout << "[ ] ";
            } else if(buffer[i][j] == '+') {
                cout << "[+] ";
            } else {
                cout << "[*] ";
            }
        }
        cout << endl;
    }
    cout << "FIN DE LA MATRIZ" << endl;
}

void Render::initGL() {
    if (glfwInit() != GLFW_TRUE) {
        cout << "Error GLFW\n";
        return;
    }

#ifdef _APPLE_
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    if (this->window) {
        cerr << "Error: La ventana ya fue creada." << endl;
        return;
}

    this->window = glfwCreateWindow(1080, 720, "Ventana 1", nullptr, nullptr);
    if (!this->window) {
        cerr << "Error: No se pudo crear la ventana GLFW." << endl;
        glfwTerminate();
        return;
    }

    // Haz este contexto el activo
    glfwMakeContextCurrent(this->window);

    // Carga las funciones de OpenGL (glad)
    gladLoadGL(glfwGetProcAddress);

    // AHORA puedes llamar a funciones de OpenGL
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    InputManager::init(this->window);
}

void Render::putObject(int ID, Object3D* obj) {
    objectList[ID] = obj;

    bufferObject bo;
    //generar buffers de datos
    glGenVertexArrays(1, &bo.idArray);
    glGenBuffers(1, &bo.idVertexArray);
    glGenBuffers(1, &bo.idIndexArray);
    //por cada buffer
    glBindVertexArray(bo.idArray);
    //activar buffer
    glBindBuffer(GL_ARRAY_BUFFER, bo.idVertexArray);
    // subir por buffer sus datos
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * obj->vertexList.size(), obj->vertexList.data(), GL_STATIC_DRAW);

    //activar buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo.idIndexArray);
    // subir por buffer sus datos
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * obj->idList.size(), obj->idList.data(), GL_STATIC_DRAW);

    //guardar bufferObject en mapa
    this->bufferObjects[obj->id] = bo;
}

Object3D* Render::getObject(int ID)
{
    auto it = objectList.find(ID);
    return (it != objectList.end()) ? it->second : nullptr;
}

void Render::removeObject(Object3D* obj) {
    if (!obj) return;

    objectList.erase(obj->id);

    // Buscar el bufferObject asociado en el mapa
    auto it = bufferObjects.find(obj->id);
    if (it != bufferObjects.end()) {
        bufferObject bo = it->second;

        // Eliminar los buffers de OpenGL
        glDeleteBuffers(1, &bo.idVertexArray);
        glDeleteBuffers(1, &bo.idIndexArray);
        glDeleteVertexArrays(1, &bo.idArray);

        // Eliminar la entrada del mapa
        bufferObjects.erase(it);
    }
}

void Render::drawGL(Object3D* obj) {
    if (!obj) return;

    // Calcular las matrices: modelo, vista y proyección, y su producto MVP
    auto model = obj->computeModelMatrix();
    auto view = cam->lookAtThis();
    auto projection = cam->perspective();
    Matrix4x4f MVP = projection * view * model;

    // Activar los buffers asociados al objeto
    auto bo = bufferObjects[obj->id];
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(bo.idArray);
    glBindBuffer(GL_ARRAY_BUFFER, bo.idVertexArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo.idIndexArray);

    // Usar el programa de shaders del objeto
    obj->prg->use();
    obj->prg->checkErrors();

    // 1) Enviar MVP y modelo
    obj->prg->setUniformData(Program::matrix4, MVP.mat1, "MVP");
    obj->prg->setUniformData(Program::matrix4, model.mat1, "M");

    // 2) Attributes (usa los campos reales de tu Vertex)
    obj->prg->setAttributeData(
        "vPos", 4, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, position)
    );
    obj->prg->setAttributeData(
        "vColor", 4, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, color)
    );
    obj->prg->setAttributeData(
        "vNorm", 4, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, vNormal)
    );
    obj->prg->setAttributeData(
        "vTextureCoord", 4, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, vTextureCoord)
    );

    // 3) Enviar parámetros de la luz (usa light->kd como “Ik”)
    auto& Ls = this->getLight();
    if (Ls.size() >= 2) {
        obj->prg->setUniformData(Program::vector4, Ls[0]->position.data, "lightPos1");
        obj->prg->setUniformData(Program::vector4, Ls[0]->color.data, "lightColor1");
        obj->prg->setUniformData(Program::integer, &Ls[0]->type, "lightType1");
        obj->prg->setUniformData(Program::vector4, Ls[0]->direction.data, "lightDirection1");
        obj->prg->setUniformData(Program::floatpoint, &Ls[0]->kd, "Ik1");

        obj->prg->setUniformData(Program::vector4, Ls[1]->position.data, "lightPos2");
        obj->prg->setUniformData(Program::vector4, Ls[1]->color.data, "lightColor2");
        obj->prg->setUniformData(Program::floatpoint, &Ls[1]->kd, "Ik2");
        obj->prg->setUniformData(Program::integer, &Ls[1]->type, "lightType2");
        obj->prg->setUniformData(Program::vector4, Ls[1]->direction.data, "lightDirection2");
    }

    // 4) Parámetros de material (ajusta el nombre del campo shininess si lo tienes distinto)
    obj->prg->setUniformData(Program::floatpoint, &obj->mat->Kd, "Kd");
    obj->prg->setUniformData(Program::floatpoint, &obj->mat->Ks, "Ks");

    // 5) Posición de cámara
    obj->prg->setUniformData(Program::vector4, cam->pos.data, "cameraPos");

    // 6) Bind de textura y sampler
    int texUnit = 0;
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_2D, obj->mat->texture->textureID);
    obj->prg->setUniformData(Program::integer, &texUnit, "textureColor");

    // Dibujar el objeto
    glDrawElements(GL_TRIANGLES, obj->idList.size(), GL_UNSIGNED_INT, nullptr);

    // Opcional: deshabilitar arrays si es necesario
    // glDisableVertexAttribArray(posLocation);
    // glDisableVertexAttribArray(colorLocation);
}

void Render::mainLoop() {
    // Bucle principal hasta que se cierre la ventana
    while (!glfwWindowShouldClose(window)) {
        // Procesa eventos de entrada (teclado, ratón, etc.)
        glfwPollEvents();

        // Mueve la cámara (y actualiza su collider internamente)
        cam->move(1.0f);

        // Mueve todas las luces de la escena
        for (auto* L : lights) {
            L->move(0.1f);
        }

        // Limpia color y profundidad antes de dibujar
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Actualiza collider, mueve (si procede) y dibuja cada objeto
        for (auto it = objectList.begin(); it != objectList.end(); ++it) {
            Object3D* obj = it->second;
            obj->updateCollider();
            obj->move(1.0f);
            drawGL(obj);
        }

        // Intercambia buffers para presentar el frame recién dibujado
        glfwSwapBuffers(window);
    }

    // Al salir del bucle, destruye la ventana y termina GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Render::putCamera(Camera* camj)
{
    if (camj == nullptr) {
        std::cerr << "Error: Se ha intentado asignar una cámara nula." << std::endl;
        return;
    }
    // Asigna la cámara pasada al atributo interno del render
    this->cam = camj;
}
