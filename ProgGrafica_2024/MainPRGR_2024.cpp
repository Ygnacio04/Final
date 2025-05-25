#include <iostream>
#include "libprgr/vectorMath.h"
#define GLAD_BIN
#include "libprgr/common.h"
#include "libprgr/EventManager.h"
#include "libprgr/Object3D.h"
#include "libprgr/Render.h"
#include "libprgr/SpaceShip.h"
using namespace libPRGR;



int main(int argc, char** argv)
{
	Render* r = new Render();
	r->cam = new Camera({ 0,0,13,1 }, { 0,1,0,0 }, { 0,0,0,1 });
	r->light = new Light(Light::DIRECTIONAL,{ 0,0,3,1 }, { 1,1,1,1 }, 1.0f,{0,0,-1,0});

	GLFWwindow* window=r->initGLFW();
	//añadir enemigo
	Object* enemy = new Enemy();
	enemy->pos = { -5.0f,5,-1,1 };//delante del plano/tapa
	r->setupObject(enemy);

	//añadir nave protagonista
	Object* spaceShip = new SpaceShip();
	spaceShip->pos = { 0,-5,-1,1 };//delante del plano/tapa
	r->setupObject(spaceShip);


	//gestión de tiempo
	float timeStep = 0;
	float oldTime = glfwGetTime(); //tiempo actual en segundos
	float newTime = 0;
	while (!glfwWindowShouldClose(window))
	{	//actualizar tiempo


		newTime = glfwGetTime();
		timeStep = newTime - oldTime;
		oldTime = newTime;

		//check eventos

		glfwPollEvents();
		for(auto obj: r->objectList)
			obj->update(timeStep);
		//borrar objetos que no estén vivos
		auto it = r->objectList.begin();
		do
		{
			if (!(*it)->isAlive)
			{
				it=r->objectList.erase(it);
			}
			if (it != r->objectList.end())
				it++;
		}while (it != r->objectList.end());

		//añadir nuevos objetos creados
		
		 while(r->newObjectList.size()>0)//mientras haya objetos
		 {
			 //sacar nuevo
			 Object* obj = r->newObjectList.front();
			 r->newObjectList.pop_front();
			 //añadir
			 r->setupObject(obj);
		 }
		
		r->cam->update(timeStep);
		//dibujar
			//limpiar buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//mandar triángulos
		for (auto obj : r->objectList)
			r->drawGL(obj);
	//cambiarBuffers
		glfwSwapBuffers(window);

	}

	delete r;
	return 0;
}