#pragma once
#include "common.h"

#include "vectorMath.h"
#include "vertex.h"
#include "GlProgram.h"
#include "Texture.h"
#include "BV.h"

class Object {
public:

	typedef struct {
		float Ks, Kd;
		int shinny;
		Texture* texture;
	}material_t;

	bool isAlive = true;

	material_t  mat = { 1,1,10 };
	VBH* collider = nullptr;


	vector4f pos= { 0,0,0,1 };
	vector4f rot = { 0,0,0,0 };
	vector4f size= { 1,1,1,0 };

	vector<vertex_t> vertexList;
	vector<unsigned int> indexVertexList;

	inline static int idCounter = 0;
	int id;

	GLProgram* program=nullptr;

	int objectType = -1;


	Object();

	Object(string file);

	virtual void update(float timeStep);

	void leerVertices(std::ifstream& f);

	void leerColores(std::ifstream& f);

	void leerNormales(std::ifstream& f);

	void leerTexturas(std::ifstream& f);

	void leerCaras(std::ifstream& f);

	void leerProgramas(std::ifstream& f);

	void loadFromFile(string file);

	void updateCollider();
	matrix4x4f computeModelMatrix();

};
