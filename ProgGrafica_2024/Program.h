#pragma once
#include "./Shader.h"
using namespace std;

class Program {
public:
	typedef enum {
		integer,
		floatpoint,
		vector4,
		matrix4
	} datatype_e;

	unsigned int idProgram;
	vector<Shader*> shaderList;
	map<string, unsigned int> varList;
	map<string, string> shaderDesc; // Almacena pares clave=valor

	Program(vector<string> shaders);
	void addShader(string fileName);
	void linkProgram();
	void checkErrors();
	void clean();
	void readVarList();
	void use();
	void setUniformData(datatype_e tipo, void* dato, string nombre);
	void setAttributeData(string nombre, GLint size, GLenum type, GLboolean normalized,
							GLsizei stride, const GLvoid* pointer);
	void readShaderDesc(istream& in);
	int getPosAttributeLocation();
	int getColorAttributeLocation();
	int getMVPUniformLocation();

	void initLight(const float lightPos[4],
		const float lightColor[4],
		int lightType,
		const float lightDirection[4],
		float Ik,
		const float cameraPos[4]);

	void initMaterial(float Kd, float Ks, int textureUnit);
};