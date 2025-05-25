#include "Shader.h"
using namespace std;

Shader::Shader(string fileName)
{
	this->fileName = fileName;
	readSource();
	compileShader();
	checkErrors();
}

void Shader::readSource()
{
	ifstream file(fileName);

	if (!file.is_open()) {
		cerr << "Error al abrir el archivo " << fileName << endl;
		return;
	}

	stringstream ss;
	string line;

	while (getline(file, line)) {
		source += line + "\n";
	}

	file.close();
}

void Shader::compileShader()
{
	if (fileName.ends_with(".vert")) {
		type = GL_VERTEX_SHADER;
	} else if (fileName.ends_with(".frag")) {
		type = GL_FRAGMENT_SHADER;
	} else {
		cerr << "Tipo de shader desconocido para el archivo " << fileName << endl;
		return;
	}

	idShader = glCreateShader(type);

	const char* shaderCode = source.c_str();

	glShaderSource(idShader, 1, &shaderCode, nullptr);
	glCompileShader(idShader);
}

void Shader::checkErrors()
{
	GLint retCode;
	char errorLog[1024];
	GLint fragment_compiled;
	glGetShaderiv(idShader, GL_COMPILE_STATUS, &fragment_compiled);
	if (fragment_compiled != GL_TRUE)
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(idShader, 1024, &log_length, message);
		std::cout << "ERROR " << fileName << "\n" << message << "\n\n";

		// parar, gestion de error
	}
}

void Shader::clean()
{
	if (idShader) {
		glDeleteShader(idShader);
		idShader = 0;
	}
}
