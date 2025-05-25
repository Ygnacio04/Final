#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "../prgr/lib.h"
#include "../prgr/vertex.h"
#include "../prgr/common.h"
#include "../Program.h"
#include "../Material.h"
#include "../Collider.h"
#include <vector>

using namespace lib;
using namespace std;

class Object3D {
public:
    Vector4f position = { 0,0,0,1 };
    Vector4f rotation = { 0,0,0,0 };
    Vector4f scale = { 1,1,1,0 };

    Matrix4x4f modelMatrix;

    vector<Vertex> vertexList;
    vector<unsigned int> idList;
    inline static int idCounter = 0;
    int id;

	Program* prg;
    Material* mat;
    Collider* coll;

    Object3D();
    Object3D(string file);
    void createTriangle();
    virtual void move(double timeStep);
    void updateModelMatrix();
    void leerVertices(ifstream& f);
    void leerColores(ifstream& f);
    void leerNormales(ifstream& f);
    void leerTexturas(ifstream& f);
    void leerCaras(ifstream& f);
    void leerProgramas(ifstream& f);
    void leerAtributos(std::ifstream& f);
    void loadFromFile(string file);
    Matrix4x4f computeModelMatrix();
    void updateCollider();
};

#endif