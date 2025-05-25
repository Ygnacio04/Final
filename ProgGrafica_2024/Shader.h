#pragma once
#include <string>
#include "prgr/common.h"
using namespace std;

class Shader {
public:
	unsigned int idShader;
	string fileName;
	GLenum type;
	string source;

	Shader(string fileName);
	void readSource();
	void compileShader();
	void checkErrors();
	void clean();
};