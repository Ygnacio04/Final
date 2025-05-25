#include "Render.h"
#include "EventManager.h"

Render::Render()
{
}

 GLFWwindow* Render::initGLFW()
{
	if (glfwInit() != GLFW_TRUE)
	{
		cout << "ERROR GLFW\n";
		return nullptr;
	}

#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif


	 window = glfwCreateWindow(1080, 720, "Ventana 1",
		nullptr, nullptr);
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	EventManager::init(window);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); //activamos uso de tranparencias
	glBlendFunc(GL_SRC_ALPHA, GL_ZERO); //seleccionamos función de mexclado por alpha





	return window;
}

 void Render::deinitGLFW()
{
	glfwTerminate();
}

 void Render::setupObject(Object* obj)
{
	objectList.push_back(obj);
	bufferObject bo;
	//generar buffers de datos
	glGenVertexArrays(1, &bo.idArray);
	glGenBuffers(1, &bo.idVertexArray);
	glGenBuffers(1, &bo.idIndexArray);
	//por cada buffer
	glBindVertexArray(bo.idArray);
	//activar  buffer
	glBindBuffer(GL_ARRAY_BUFFER, bo.idVertexArray);
	//subir por  bufer sus datos
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * obj->vertexList.size(),
		obj->vertexList.data(), GL_STATIC_DRAW);
	//activar  buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo.idIndexArray);
	//subir por  bufer sus datos
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * obj->indexVertexList.size(),
		obj->indexVertexList.data(), GL_STATIC_DRAW);

	//guardar bufferObject en mapa 
	bufferObjectList[obj->id] = bo;

}

 void Render::drawGL(Object* obj)
{
	//seleccionar malla
	auto model = obj->computeModelMatrix();
	auto view = cam->computeViewMatrix();
	auto projection = cam->computeProjectionMatrix();

	matrix4x4f MVP = projection * view * model;

	//activar buffers
	auto bo = bufferObjectList[obj->id];
	glBindVertexArray(bo.idArray);
	glBindBuffer(GL_ARRAY_BUFFER, bo.idVertexArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo.idIndexArray);
	
	//activar programa
	obj->program->use();
	//copiar datos de matriz MVP
	obj->program->setUniformData(GLProgram::matrix4, MVP.mat1, "MVP");
	obj->program->setUniformData(GLProgram::matrix4, model.mat1, "M");

	
	//setear atributo
	obj->program->setAttributeData("vPos", 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t),
		(void*)offsetof(vertex_t, vPos));
	obj->program->setAttributeData("vColor", 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t),
		(void*)offsetof(vertex_t, vColor));
	obj->program->setAttributeData("vNorm", 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t),
		(void*)offsetof(vertex_t, vNormal));
	obj->program->setAttributeData("vTextureCoord", 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t),
		(void*)offsetof(vertex_t, vTextureCoord));

	
	obj->program->setUniformData(GLProgram::vector4, &(light->pos), "lightPos");
	obj->program->setUniformData(GLProgram::vector4, &(light->color), "lightColor");
	obj->program->setUniformData(GLProgram::floatpoint, &(light->I), "Ik");
	obj->program->setUniformData(GLProgram::integer, &(light->type), "lightType");
	obj->program->setUniformData(GLProgram::vector4, &(light->direction), "lightDirection");

	obj->program->setUniformData(GLProgram::floatpoint, &(obj->mat.Ks), "Ks");
	obj->program->setUniformData(GLProgram::floatpoint, &(obj->mat.Kd), "Kd");
	obj->program->setUniformData(GLProgram::integer, &(obj->mat.shinny), "shinny");


	obj->program->setUniformData(GLProgram::vector4, &(cam->pos), "cameraPos");

	//activar textura
	int textureUnit = 0;
	glActiveTexture(GL_TEXTURE0 + textureUnit); //activamos unidad de texturado 0
	glBindTexture(GL_TEXTURE_2D, obj->mat.texture->textureID); //cargamos la textura en unidad de texturado 0
	//activar sampler2D en shader
	obj->program->setUniformData(GLProgram::integer, &(textureUnit), "textureColor");

	glDrawElements(GL_TRIANGLES, (int)obj->indexVertexList.size(), GL_UNSIGNED_INT, nullptr);
}

 vector<Object*> Render::getCollisions(Object* obj, int ObjectType)
 {
	 vector<Object*> collisions;

	 //por cada objeto del sistema
	 for (auto& obj2 : objectList) 
	 {
		 if (obj2 != obj)//si no es el objeto de test
			 if (obj2->isAlive && obj2->objectType == ObjectType  //si es del tipo buscado
				 && obj->collider->root->collision(obj2->collider->root))// y colisiona con el objeto de test
			 {
				 collisions.push_back(obj2); //añadir a la lista
			 }
	 }
	 return collisions;
 }
