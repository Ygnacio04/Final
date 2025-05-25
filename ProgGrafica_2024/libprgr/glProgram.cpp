#include "glProgram.h"

GLProgram::GLProgram(vector<string> shaderList)
{
    //crear programId
    programId = glCreateProgram();
    //por cada archivo
    for(auto &file: shaderList)
    {
        //check tipo de shader
        unsigned int tipo = -1;
        if (file.ends_with(".vert")) //shader de vertices
        {
            tipo = GL_VERTEX_SHADER;
        }
        else if (file.ends_with(".frag"))
        {
            tipo = GL_FRAGMENT_SHADER;
        }
        //leer archivo
        string code;
        std::ifstream f(file);
        if (f.is_open()) {
            code = std::string(std::istreambuf_iterator<char>(f), {});
        }
        else {
            std::cout << "ERROR: FICHERO NO ENCONTRADO " <<
                __FILE__ << ":" << __LINE__ << " " << file << "\n";
        }
        //compilar
        const char* shaderCode = code.c_str();
        int shaderID = glCreateShader(tipo);
        glShaderSource(shaderID, 1, &shaderCode, nullptr);
        glCompileShader(shaderID);

        checkShaderError(shaderID, file);
        //atar al programa
        glAttachShader(programId, shaderID);
    }
    //linkar programa
    glLinkProgram(programId);
    //check error de programa
    checkProgramError(programId);
    //liberar memoria de shaders
    //TO DO
    readVarList();

}

void GLProgram::checkShaderError(unsigned int shaderId, string fileName)
{
    GLint retCode;
    char errorLog[1024];
    GLint fragment_compiled;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &fragment_compiled);
    if (fragment_compiled != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(shaderId, 1024, &log_length, message);
        std::cout << "ERROR " << fileName << "\n" << message << "\n\n";
        //parar, gestion de error
    }

}

void GLProgram::checkProgramError(unsigned int programId)
{
    GLint program_linked;
    glGetProgramiv(programId, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(programId, 1024, &log_length, message);
        std::cout << "ERROR \n" << message << "\n\n";
    }

}

void GLProgram::readVarList()
{
    
        int numAttributes = 0;
        int numUniforms = 0;
        glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &numAttributes);
        for (int i = 0; i < numAttributes; i++)
        {
            char varName[100];
            int bufSize = 100, length = 0, size = 0;
            GLenum type = -1;
            glGetActiveAttrib(programId, (GLuint)i, bufSize, &length, &size, &type, varName);
            varList[std::string(varName)] = glGetAttribLocation(programId, varName);
        }
        glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &numUniforms);
        for (int i = 0; i < numUniforms; i++)
        {
            string varName; varName.resize(100);
            int bufSize = 100, length = 0, size = 0;
            GLenum type = -1;
            glGetActiveUniform(programId, (GLuint)i, bufSize, &length, &size, &type, varName.data());
            varName = std::string(varName.c_str()); //interrogar con nombre
            if (varName[varName.length() - 1] == ']') {//si es de tipo array
                std::string arrName = varName.substr(0, varName.find('['));
                for (int i = 0; i < size; i++) //coneguir la lista completa de nombres
                {
                    std::string arrNameIdx = arrName + "[" + std::to_string(i) + "]";
                    varList[arrNameIdx] = glGetUniformLocation(programId, arrNameIdx.c_str());
                }
            }
            else
                varList[varName] = glGetUniformLocation(programId, varName.c_str());
        }
}

void GLProgram::setUniformData(dataType_e tipo, void* dato, string nombre)
{
    if (varList.find(nombre) == varList.end())
    {
        cout << "ERROR: Variable de shader " << nombre << " no encontrada" << endl;
    }else{
        switch (tipo)
        {
            case matrix4:
            {
                glUniformMatrix4fv(varList[nombre], 1, GL_TRUE, (float*)dato);
            } break;
            case integer:
            {
                glUniform1i(varList[nombre],  ((int*)dato)[0]);
            } break;
            case floatpoint:
            {
                glUniform1f(varList[nombre], ((float*)dato)[0]);
            } break;
            case vector4:
            {
                glUniform4f(varList[nombre], 
                    ((float*)dato)[0], ((float*)dato)[1], ((float*)dato)[2], ((float*)dato)[3]);
            } break;
            
            default:
                cout << "ERROR: tipo de dato no soportado" << endl;
            break;
        }
    }
}

void GLProgram::setAttributeData(string nombre,GLint size, GLenum type, GLboolean
    normalized, GLsizei stride, const GLvoid* pointer)
{
    if (varList.find(nombre) == varList.end())
    {
        cout << "ERROR: Variable de shader " << nombre << " no encontrada" << endl;
    }
    else {
        unsigned int index = varList[nombre];
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type,
                normalized, stride, pointer);
    }
}

void GLProgram::use()
{
    glUseProgram(programId);
}

GLProgram::~GLProgram()
{
}
