#ifndef RENDER_H
#define RENDER_H

#include "../prgr/common.h"
#include "../Object3D/Object3D.h"
#include <map>
#include <iostream>
#include "../Camera.h"
#include "../Light.h"

using namespace std;

typedef struct {
    unsigned int idArray; //identificador de grupos de arrays
    unsigned int idVertexArray; //identificador datos de vertice
    unsigned int idIndexArray; //identificador de orden de vertices
} bufferObject;

class Render {
private:
    vector<vector<char>> buffer;
public:
    int h, w; // Altura y anchura
    int centerX, centerY; // Centro de la pantalla
    
    GLFWwindow* window;
    //vector<Object3D*> objectList;
    inline static map<int, Object3D*> objectList;
    map<int, bufferObject> bufferObjects;
    Camera* cam;
	vector<Light*> lights;

    // Constructor
    Render(int h, int w);

    // Destructor
    ~Render();

    // Métodos para manipular la luz
    void putLight(Light* l) { 
        lights.push_back(l);
    }
    const vector<Light*>& getLight() { return lights; }

    // Método para poner un pixel
    void PutPixel(int x, int y);

    // Método para resetear el buffer
    void resetBuffer();

    // Método para dibujar el buffer
    void Draw();

    void initGL();
    //void putObject(Object3D* obj);
    void putObject(int ID, Object3D* obj);
    static Object3D* getObject(int ID);
    void removeObject(Object3D* obj);
    void drawGL(Object3D* obj);
    void mainLoop();

    void putCamera(Camera* camj);
};

#endif