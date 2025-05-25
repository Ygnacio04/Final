#include "Object3D.h"
#include "EventManager.h"
#include <limits>

 Object::Object() {
	id = idCounter++;
	this->pos = { 0,0,0,1 };
	this->vertexList.push_back({ { 0, 0.5,0,1 },{ 1,0,0,1 } });
	this->vertexList.push_back({ { -0.5,-0.5,0,1 },{ 0,1,0,1 } });
	this->vertexList.push_back({ { 0.5,-0.5,0,1 },{ 0,0,1,1 } });

	this->indexVertexList = { 0,1,2 };

}

 Object::Object(string file) {
	 id = idCounter++;
	 this->pos = { 0,0,0,1 };
	 loadFromFile(file);
 }



 void Object::update(float timeStep) {
	 /*
	if (EventManager::keyState[GLFW_KEY_LEFT])
		this->pos.x -= 0.001f;

	if (EventManager::keyState[GLFW_KEY_RIGHT])
		this->pos.x += 0.001f;

	if (EventManager::keyState[GLFW_KEY_Q])
		this->rot.y -= 0.1f;

	if (EventManager::keyState[GLFW_KEY_E])
		this->rot.y += 0.1f;
		*/
}

 void Object::leerVertices(std::ifstream &f)
 {
	 //repetir
	 string linea="";
	 do{
	 //	leer linea
		  std::getline(f, linea);

		  //si no es comentario
		  if((linea[0]!='/'&& linea[1] != '/')&&(linea!="end"))
		  {
	 // // separar linea en identificador y posiciones
			  std::stringstream l(linea);
			  string identificador;
			  string posiciones;
			  l >> identificador;
			  l >> posiciones;
			  std::vector<float> pos = splitString<float>(posiciones, ',');
	 // // asignar posiciones a nuevo vertice
			  vertex_t v;
			  v.vPos.x = pos[0];
			  v.vPos.y = pos[1];
			  v.vPos.z = pos[2];
			  v.vPos.w = 1.0f;
			  this->vertexList.push_back(v);
		  }
	 //hasta "end"
	 }while(linea != "end");
 }


 void Object::leerColores(std::ifstream& f)
 {
	 //repetir
	 string linea = "";
	 do {
		 //	leer linea
		 std::getline(f, linea);

		 //si no es comentario
		 if ((linea[0] != '/' && linea[1] != '/') && (linea != "end"))
		 {
			 // // separar linea en identificador y posiciones
			 std::stringstream l(linea);
			 string identificador;
			 string colores;
			 l >> identificador;
			 l >> colores;
			 std::vector<float> color = splitString<float>(colores, ',');
			 int vertexId = splitString<int>(identificador, ':')[0];
			 this->vertexList[vertexId - 1].vColor = { color[0],color[1],color[2],color[3] };
			}
		 //hasta "end"
	 } while (linea != "end");
 }


 void Object::leerNormales(std::ifstream& f)
 {
	 //repetir
	 string linea = "";
	 do {
		 //	leer linea
		 std::getline(f, linea);

		 //si no es comentario
		 if ((linea[0] != '/' && linea[1] != '/') && (linea != "end"))
		 {
			 // // separar linea en identificador y posiciones
			 std::stringstream l(linea);
			 string identificador;
			 string normal;
			 l >> identificador;
			 l >> normal;
			 std::vector<float> n = splitString<float>(normal, ',');
			 int vertexId = splitString<int>(identificador, ':')[0];
			 this->vertexList[vertexId - 1].vNormal = { n[0],n[1],n[2],n[3] };
		 }
		 //hasta "end"
	 } while (linea != "end");
 }


 void Object::leerTexturas(std::ifstream& f)
 {
	 //repetir
	 string linea = "";
	 do { //primer bucle: leer coordenadas de textura hasta "end"
		 //	leer linea
		 std::getline(f, linea);

		 //si no es comentario
		 if ((linea[0] != '/' && linea[1] != '/') && (linea != "end"))
		 {
			 // // separar linea en identificador y posiciones
			 std::stringstream l(linea);
			 string identificador;
			 string textureCoord;
			 l >> identificador;
			 l >> textureCoord;
			 std::vector<float> tc = splitString<float>(textureCoord, ',');
			 int vertexId = splitString<int>(identificador, ':')[0];
			 this->vertexList[vertexId - 1].vTextureCoord = { tc[0],tc[1],-1,-1 };
		 }
		 //hasta "end"
	 } while (linea != "end");

	 do {//leer fichero de textura
		 //	leer linea
		 std::getline(f, linea);
		 //si no es comentario
		 if ((linea[0] != '/' && linea[1] != '/') && (linea != "end"))
		 {
			//fichero de textura en variable linea
			 this->mat.texture = new Texture(linea);
		 }
		 //hasta "end"
	 } while (linea != "end");
 }




 
 void Object::leerCaras(std::ifstream& f)
 {
	 //repetir
	 string linea = "";
	 do {
		 //	leer linea
		 std::getline(f, linea);

		 //si no es comentario
		 if ((linea[0] != '/' && linea[1] != '/') && (linea != "end"))
		 {
			 // // separar linea en identificador y posiciones
			 std::stringstream l(linea);
			 string identificador;
			 string vertexIds;
			 l >> identificador;
			 l >> vertexIds;
			 std::vector<int> vIds = splitString<int>(vertexIds, ',');
			 this->indexVertexList.push_back(vIds[0] - 1);
			 this->indexVertexList.push_back(vIds[1] - 1);
			 this->indexVertexList.push_back(vIds[2] - 1);
		 }
		 //hasta "end"
	 } while (linea != "end");
 }


 void Object::leerProgramas(std::ifstream& f)
 {
	 //repetir
	 std::vector<string> shaderList;
	 string linea = "";
	 do {
		 //	leer linea
		 std::getline(f, linea);

		 //si no es comentario
		 if ((linea[0] != '/' && linea[1] != '/') && (linea != "end"))
		 {
			 // // separar linea en identificador y posiciones
			 shaderList.push_back(linea);
		 }
		 //hasta "end"
	 } while (linea != "end");
	 this->program = new GLProgram(shaderList);
 }

void Object::loadFromFile(string file)
{
	//abrir fichero
	std::ifstream f(file);
	if(f.is_open())
	{
	//	//leerVertices
		leerVertices(f);
		leerColores(f);
		leerNormales(f);
		leerTexturas(f);
		leerCaras(f);
		leerProgramas(f);
		updateCollider();
	}
	else {
		cout << "ERROR: Fichero " << file << " no existe" << endl;
	}
}

void Object::updateCollider()
{
	//crear esfera
		//calcular radio
		//calcular centro de objeto

	//calcular máximos xyz
	//calcular mínimos xyz
	if (collider == nullptr) {
		
		vector4f max = { -numeric_limits<float>::max(),-numeric_limits<float>::max(),-numeric_limits<float>::max(),1 };
		vector4f min = { numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max(),1 };

		for (auto& v : vertexList)
		{
			max.x = max.x < v.vPos.x ? v.vPos.x : max.x;
			max.y = max.y < v.vPos.y ? v.vPos.y : max.y;
			max.z = max.z < v.vPos.z ? v.vPos.z : max.z;

			min.x = min.x > v.vPos.x ? v.vPos.x : min.x;
			min.y = min.y > v.vPos.y ? v.vPos.y : min.y;
			min.z = min.z > v.vPos.z ? v.vPos.z : min.z;
		}
		float textureW = max.x - min.x;
		float textureH = max.y - min.y;
		float pixelW = textureW/this->mat.texture->w;
		float pixelH = textureH/this->mat.texture->h;
		
		this->collider = new VBH();
		/*vector4f center = (max + min) / 2.0f;
		center.w = 1;
		float radious = 0.5;//distance(max, min) / 2.0f;

		BV* particle= new SphereBV(center, radious);
		
		this->collider = new VBH();
		this->collider->addParticle(particle);*/
		//por cada pixel que no es transparente
		for (int y = 0;y < this->mat.texture->h;y++)
			for (int x = 0;x < this->mat.texture->w ;x++)
		{
			auto& p = this->mat.texture->pixels[ y* this->mat.texture->w+x];
			//si no es transparente:
			if (p.a > 100)
			{
				vector4f center = {min.x + pixelW * (float)x + pixelW / 2.0f,
								   min.y + pixelH * (float)y + pixelH / 2.0f,
								   this->pos.z,1};
				float radius= (pixelW / 2.0f> pixelH / 2.0f)? 
					pixelW  / 2.0f:
					pixelH / 2.0f;

				BV* particle = new SphereBV(center, radius);
				this->collider->addParticle(particle);
			}

		}
		this->collider->subdivide();
			

	}
	//actualizar nada más crear el collider
	auto mat = computeModelMatrix();
	this->collider->update(mat,this->size);
}

matrix4x4f Object::computeModelMatrix()
{
	matrix4x4f model=make_identityf();
	model = make_traslate(pos.x, pos.y, pos.z);
	model = model*make_rotate(rot.x, rot.y, rot.z);
	model = model*make_scale(size.x, size.y, size.z);
	return model;
}
