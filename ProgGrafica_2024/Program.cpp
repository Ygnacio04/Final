#include "Program.h"
using namespace std;

Program::Program(vector<string> shaders)
{
	idProgram = glCreateProgram();
	for (const auto& shaderFile : shaders) {
		cout << "Shader cargado: " << shaderFile << endl;
		addShader(shaderFile);  // Llamamos a la función addShader para añadir cada shader
	}
	linkProgram();
}

void Program::addShader(string fileName)
{
	unsigned int tipo = -1;
	if (fileName.ends_with(".vert")) {
		tipo = GL_VERTEX_SHADER;
	}
	else if (fileName.ends_with(".frag")) {
		tipo = GL_FRAGMENT_SHADER;
	}

	std::ifstream f(fileName);
	std::string code{ std::istreambuf_iterator<char>(f), {} };
	const char* src = code.c_str();

	// 3) Crear, compilar y comprobar
	GLuint shaderId = glCreateShader(tipo);
	glShaderSource(shaderId, 1, &src, nullptr);
	glCompileShader(shaderId);

	GLint compiled = GL_FALSE;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE) {
		char log[1024]; GLsizei len = 0;
		glGetShaderInfoLog(shaderId, 1024, &len, log);
		std::cerr << "ERROR COMPILANDO " << fileName << ":\n" << log << "\n";
		glDeleteShader(shaderId);
		return; // no adjuntes este shader
	}

	// 4) Adjuntar sólo si compiló
	glAttachShader(idProgram, shaderId);
}

void Program::linkProgram()
{
	glBindAttribLocation(idProgram, 0, "vPos");
	glBindAttribLocation(idProgram, 1, "vColor");
	glBindAttribLocation(idProgram, 2, "vNorm");
	glBindAttribLocation(idProgram, 3, "vTextureCoord");

	glLinkProgram(idProgram);
	checkErrors();
	// liberar memoria de shaders (TODO)
	readVarList();
}

void Program::checkErrors()
{
	// Verificación de errores en los shaders
	for (Shader* shader : shaderList) {  // Asegúrate de que shaderList sea un vector de punteros a Shader
        GLuint shaderId = shader->idShader; // Asumiendo que tienes un método getId() para obtener el ID del shader
        GLint compiled;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
        if (compiled != GL_TRUE) {
            GLsizei log_length = 0;
            GLchar message[1024];
            glGetShaderInfoLog(shaderId, 1024, &log_length, message);
            std::cout << "ERROR COMPILANDO SHADER: \n" << message << "\n\n";
        }
    }

	// Verificación de errores en el programa
	GLint program_linked;
	glGetProgramiv(idProgram, GL_LINK_STATUS, &program_linked);
	if (program_linked != GL_TRUE) {
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetProgramInfoLog(idProgram, 1024, &log_length, message);
		std::cout << "ERROR AL ENLAZAR EL PROGRAMA: \n" << message << "\n\n";
	}
}

void Program::clean()
{
}

void Program::readVarList()
{
	int numAttributes = 0;
	int numUniforms = 0;
	glGetProgramiv(idProgram, GL_ACTIVE_ATTRIBUTES, &numAttributes);
	for (int i = 0; i < numAttributes; i++)
	{
		char varName[100];
		int bufSize = 100, length = 0, size = 0;
		GLenum type = -1;
		glGetActiveAttrib(idProgram, (GLuint)i, bufSize, &length, &size, &type, varName);
		varList[std::string(varName)] = glGetAttribLocation(idProgram, varName);
	}
	glGetProgramiv(idProgram, GL_ACTIVE_UNIFORMS, &numUniforms);
	for (int i = 0; i < numUniforms; i++)
	{
		string varName; varName.resize(100);
		int bufSize = 100, length = 0, size = 0;
		GLenum type = -1;
		glGetActiveUniform(idProgram, (GLuint)i, bufSize, &length, &size, &type, varName.data());
		varName = std::string(varName.c_str()); //interrogar con nombre
		if (varName[varName.length() - 1] == ']') {//si es de tipo array
			std::string arrName = varName.substr(0, varName.find('['));
			for (int i = 0; i < size; i++) //coneguir la lista completa de nombres
			{
				std::string arrNameIdx = arrName + "[" + std::to_string(i) + "]";
				varList[arrNameIdx] = glGetUniformLocation(idProgram, arrNameIdx.c_str());
			}
		}
		else
			varList[varName] = glGetUniformLocation(idProgram, varName.c_str());
	}
}

void Program::use()
{
	cout << "USANDO PROGRAMA " << idProgram << "\n";
	glUseProgram(idProgram);
	//checkErrors();
}

void Program::setUniformData(datatype_e tipo, void* dato, string nombre)
{
	if (varList.find(nombre) == varList.end()) {
		cout << "ERROR: Variable no encontrada " << nombre << "\n";
	}
	else {
		switch (tipo)
		{
		case integer: {
			glUniform1i(varList[nombre], ((int*)dato)[0]);
		} break;
		case floatpoint: {
			glUniform1f(varList[nombre], ((float*)dato)[0]);
		} break;
		case vector4: {
			glUniform4f(varList[nombre],
				((float*)dato)[0],
				((float*)dato)[1],
				((float*)dato)[2],
				((float*)dato)[3]);
		} break;
		case matrix4: {
			glUniformMatrix4fv(varList[nombre], 1, GL_TRUE, (float*)dato);
		} break;
		default: {
			cout << "ERROR: Tipo de dato no soportado\n" << endl;
		} break;
		}
	}
}

void Program::setAttributeData(string nombre, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
{
	if (varList.find(nombre) == varList.end()) {
		cout << "ERROR: Variable no encontrada " << nombre << "\n";
	}
	else {
		unsigned int index = varList[nombre];
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	}
}

void Program::readShaderDesc(istream& in)
{
	std::string line;
	while (std::getline(in, line)) {
		if (line == "end") // Fin de la sección de descripción
			break;
		if (line.empty())
			continue;
		// Se espera el formato "clave=valor"
		size_t pos = line.find('=');
		if (pos != std::string::npos) {
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 1);
			shaderDesc[key] = value;
		}
	}
}

int Program::getPosAttributeLocation()
{
	auto it = shaderDesc.find("attributeVertPos");
	if (it != shaderDesc.end())
		return glGetAttribLocation(idProgram, it->second.c_str());
	return -1;
}

int Program::getColorAttributeLocation()
{
	auto it = shaderDesc.find("attributeVertColor");
	if (it != shaderDesc.end())
		return glGetAttribLocation(idProgram, it->second.c_str());
	return -1;
}

int Program::getMVPUniformLocation()
{
	auto it = shaderDesc.find("uniformMVP");
	if (it != shaderDesc.end())
		return glGetUniformLocation(idProgram, it->second.c_str());
	return -1;
}

void Program::initLight(const float lightPos[4], const float lightColor[4], int lightType, const float lightDirection[4], float Ik, const float cameraPos[4])
{
	setUniformData(vector4, (void*)lightPos, "lightPos");
	setUniformData(vector4, (void*)lightColor, "lightColor");
	setUniformData(integer, (void*)&lightType, "lightType");
	setUniformData(vector4, (void*)lightDirection, "lightDirection");
	setUniformData(floatpoint, (void*)&Ik, "Ik");
	setUniformData(vector4, (void*)cameraPos, "cameraPos");
}

void Program::initMaterial(float Kd, float Ks, int textureUnit)
{
	setUniformData(floatpoint, (void*)&Kd, "Kd");
	setUniformData(floatpoint, (void*)&Ks, "Ks");

	// Configuramos el sampler que en el shader se llama "textureColor"
	GLint loc = glGetUniformLocation(idProgram, "textureColor");
	if (loc == -1) {
		std::cerr << "WARNING: Uniform 'textureColor' no encontrado." << std::endl;
	}
	else {
		glUniform1i(loc, textureUnit);
	}
}
