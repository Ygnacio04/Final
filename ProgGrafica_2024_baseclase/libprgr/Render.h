#pragma once
#include "Object3D.h"
#include "Camera.h"
#include "Light.h"

class Render {

public:
	typedef struct {
		unsigned int idArray; //identificador de grupos de arrays
		unsigned int idVertexArray; //identificador datos de vértice
		unsigned int idIndexArray; //identificador de orden de vértices
	}bufferObject;

	map<int, bufferObject> bufferObjectList;
	//object list se puede acceder desde métodos move de objetos
	inline static list<Object*> objectList;
	//añadir objetos al sistema
	inline static list<Object*> newObjectList;

	GLFWwindow* window=nullptr;

	Camera* cam = nullptr;
	Light* light = nullptr;

	Render();
	GLFWwindow* initGLFW();

	void deinitGLFW();

	void setupObject(Object* obj);

	void drawGL(Object* obj);

	//devuelve lista de objetos que colisionan con "obj" que sean del tipo "objecttype"
	static vector<Object*> getCollisions(Object* obj, int ObjectType);


};