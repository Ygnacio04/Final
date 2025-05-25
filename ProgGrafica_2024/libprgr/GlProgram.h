#pragma once
#include "common.h"
using namespace std;

class GLProgram {

public:

    typedef enum {
        integer,
        floatpoint,
        vector4,
        matrix4
    } dataType_e;

    unsigned int programId=-1;
    map<string, unsigned int> varList;
    GLProgram(vector<string> shaderList);

    void checkShaderError(unsigned int shaderId, string fileName);

    void checkProgramError(unsigned int programId);


    void readVarList(); //lee variables de shader
    void setUniformData(dataType_e tipo, void* dato, string nombre);
    void setAttributeData(string nombre, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
    void use(); //activa el uso de este programa
    ~GLProgram();

};