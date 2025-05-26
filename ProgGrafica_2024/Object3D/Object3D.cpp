#include "Object3D.h"
#include "../prgr/lib.h"
#include "../prgr/vertex.h"
#include "../InputManager/InputManager.h"
#include "../Program.h"
#include "../Texture.h"
#include "../Material.h"
#include "../Collider.h"
#include "../Sphere.h"
#include <vector>
using namespace lib;
using namespace std;

// Constructor
Object3D::Object3D() {
	id = idCounter++; // Asigna a identificador el valor del contador, luego se incrementa el valor del contador (de la práctica anterior)
	this->position = make_vector4f(0,0,0,1);
	this->rotation = make_vector4f(0,0,0,0);
	this->scale = make_vector4f(1,1,1,1);
	this->vertexList.push_back({ { 0, 0.5, 0, 1 },{ 1,0,0,1 } });
	this->vertexList.push_back({ { -0.5, -0.5, 0, 1 },{ 0,1,0,1 } });
	this->vertexList.push_back({ { 0.5, -0.5, 0, 1 },{ 0,0,1,1 } });

	this->idList = { 0, 1, 2 };

	coll = new Sphere();
	updateCollider();
}

Object3D::Object3D(string file) {
	id = idCounter++;
	this->position = make_vector4f(0,0,0,1);
	loadFromFile(file);

	coll = new Sphere();
	updateCollider();
}

// Método para crear un triángulo
void Object3D::createTriangle() {
	Object3D();
}

// Método para mover un triángulo
void Object3D::move(double timeStep) {
	static double lastFrameTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = static_cast<float>(currentTime - lastFrameTime);
	lastFrameTime = currentTime;

	const float angularSpeed = 45.0f;
	rotation.y += angularSpeed * deltaTime;

	// normalizar ángulo a [0,360)
	if (rotation.y >= 360.0f) rotation.y -= 360.0f;

	if (InputManager::keyState[GLFW_KEY_Q]) {
		rotation.y += 3.0f * timeStep;
		cout << "Tecla izquierda pulsada! rotation.y = " << rotation.y << endl;
	}

	if (InputManager::keyState[GLFW_KEY_E]) {
		rotation.y -= 3.0f * timeStep;
		cout << "Tecla derecha pulsada! rotation.y = " << rotation.y << endl;
	}

	// MOVIMIENTO
	if (InputManager::keyState[GLFW_KEY_LEFT]) {
		position.x -= .01f * timeStep;
	}

	if (InputManager::keyState[GLFW_KEY_RIGHT]) {
		position.x += .01f * timeStep;
	}
}

// Método para actualizar la matriz de modelo
void Object3D::updateModelMatrix() {
	modelMatrix = make_identity();
	Matrix4x4f translationMatrix = make_translate(position.x, position.y, position.z);

	float radiansZ = rotation.z * (M_PI / 180.0f);
	Matrix4x4f rotationMatrix = make_rotate(0, 0, radiansZ);

	Matrix4x4f scaleMatrix = make_scale(scale.x, scale.y, scale.z);
	modelMatrix = multiplyMatrices(translationMatrix, multiplyMatrices(rotationMatrix, scaleMatrix));

	cout << "Matriz de Modelo con rotación Z = " << rotation.z << " grados:\n";
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << modelMatrix.mat2D[i][j] << " ";
		}
		cout << endl;
	}
}

void Object3D::leerVertices(std::ifstream& f) {
	std::string linea;

	// Bucle para leer cada línea
	while (true) {
		// Si no se pudo leer (EOF o error), salimos
		if (!std::getline(f, linea)) {
			std::cout << "[leerVertices] Fin de archivo o error al leer.\n";
			break;
		}

		// Ignorar líneas vacías
		if (linea.empty()) {
			continue;
		}

		// Ignorar comentarios (empiezan con "//")
		if (linea.size() >= 2 && linea.substr(0, 2) == "//") {
			continue;
		}

		// Si la línea es "end", terminamos la sección de vértices
		if (linea == "end") {
			std::cout << "[leerVertices] Sección de vértices finalizada.\n";
			break;
		}

		// Trazar la línea leída
		std::cout << "[leerVertices] " << linea << std::endl;

		// Usar stringstream para separar identificador y posiciones
		std::stringstream l(linea);
		std::string identificador, posiciones;
		l >> identificador >> posiciones;

		// Mostrar cómo ha quedado el parseo
		std::cout << "  identificador: " << identificador
			<< " | posiciones: " << posiciones << std::endl;

		// Convertir las posiciones a floats
		std::vector<float> pos = splitString<float>(posiciones, ',');

		// Verificar que tengamos al menos 3 valores para x,y,z
		if (pos.size() >= 3) {
			Vertex v;
			v.position.x = pos[0];
			v.position.y = pos[1];
			v.position.z = pos[2];
			v.position.w = 1.0f; // Coordenada homogénea por defecto

			// Trazar el vértice que acabamos de crear
			std::cout << "  => V[" << vertexList.size() << "]: ("
				<< v.position.x << ", "
				<< v.position.y << ", "
				<< v.position.z << ")\n";

			this->vertexList.push_back(v);
		}
		else {
			// Línea mal formateada o datos insuficientes
			std::cerr << "Error: línea mal formateada o datos insuficientes: "
				<< linea << std::endl;
		}
	}

	// Al final, imprimimos la lista completa de vértices
	std::cout << "\n--- Lista final de vértices ---\n";
	for (size_t i = 0; i < vertexList.size(); i++) {
		auto& v = vertexList[i];
		std::cout << "Vertice " << i
			<< ": pos(" << v.position.x << ","
			<< v.position.y << ","
			<< v.position.z << ") "
			<< " color(" << v.color.r << ","
			<< v.color.g << ","
			<< v.color.b << ","
			<< v.color.a << ")\n";
	}
	std::cout << "--------------------------------\n";
}

void Object3D::leerColores(ifstream& f) { //fichero abierto en modo lectura

	//repetir
	string linea = "";
	do {
		//leer linea
		getline(f, linea);
		//si no es un comentario
		if ((linea[0] != '/' && linea[1] != '/') && (linea != "end")) {

			// // separar linea en identificador y colores
			stringstream l(linea);
			string identificador;
			string colores;
			l >> identificador;
			l >> colores;
			vector<float> color = splitString<float>(colores, ',');
			// // asignar colores a nuevo vertice
			//identificador.pop_back();
			int vertexId = splitString<int>(identificador, ',')[0];
			this->vertexList[vertexId - 1].color = { color[0], color[1], color[2], color[3] };
		}
		//hasta end
	} while (linea != "end");
}

void Object3D::leerNormales(ifstream& f) {
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

void Object3D::leerTexturas(ifstream& f) {
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
		this->mat = new Material(linea, 0.2f, 0.8f, 0.5f);
		//fichero de textura en variable linea
		this->mat->texture = new Texture(linea);
	}
	//hasta "end"
	} while (linea != "end");
}

void Object3D::leerCaras(ifstream& f) {
	string linea;
	do {
		getline(f, linea);
		if (linea.empty() || linea == "end" || (linea.size() >= 2 && linea.substr(0, 2) == "//"))
			continue;

		stringstream ss(linea);
		string identificador, vertexIds;
		ss >> identificador >> vertexIds;

		// Muestra qué estás leyendo
		cout << "[leerCaras] identificador: " << identificador
			<< " | vertexIds: " << vertexIds << endl;

		vector<int> vIds = splitString<int>(vertexIds, ',');

		// Resta 1 (1-based -> 0-based) y muéstralo
		cout << "Indices (ya restados 1): ";
		for (auto& vid : vIds) {
			vid -= 1;
			cout << vid << " ";
		}
		cout << endl;

		// Triangulación
		if (vIds.size() == 3) {
			idList.push_back(vIds[0]);
			idList.push_back(vIds[1]);
			idList.push_back(vIds[2]);
		}
		else if (vIds.size() == 4) {
			// Triángulo 1
			idList.push_back(vIds[0]);
			idList.push_back(vIds[1]);
			idList.push_back(vIds[2]);
			// Triángulo 2
			idList.push_back(vIds[0]);
			idList.push_back(vIds[2]);
			idList.push_back(vIds[3]);
		}
		else {
			cerr << "Error: la cara tiene " << vIds.size()
				<< " índices (ni 3 ni 4)\n";
		}
	} while (linea != "end");
}

void Object3D::leerProgramas(std::ifstream& f) {
	std::vector<string> shaderList;
	string linea = "";
	do {
		getline(f, linea);
		cout << "Línea leída: " << linea << endl;

		if ((linea[0] != '/' && linea[1] != '/') && (linea != "end")) {
			shaderList.push_back(linea);
		} 
	} while (linea != "end");
	this->prg = new Program(shaderList);
}

void Object3D::leerAtributos(std::ifstream& f) {
	this->prg->readShaderDesc(f);
}

void Object3D::loadFromFile(string file)
{
	this->vertexList.clear();
	this->idList.clear();
	
	ifstream f(file);
	if (f.is_open()) {
		leerVertices(f);
		leerColores(f);
		leerNormales(f);
		leerTexturas(f);
		leerCaras(f);
		leerProgramas(f);
		leerAtributos(f);
	}
	else {
		cout << "Error al abrir el archivo: " << file << "no existe" << endl;
	}
}

Matrix4x4f Object3D::computeModelMatrix()
{
	Matrix4x4f model = make_identity();
	model = make_translate(position.x, position.y, position.z);
	model = model * make_rotate(rotation.x, rotation.y, rotation.z);
	model = model * make_scale(scale.x, scale.y, scale.z);
	return model;
}

void Object3D::updateCollider()
{
	updateModelMatrix();
	
	coll->clearParticles();
	const float half = 0.005f;

	for (const Vertex& v : vertexList) {
		Collider::Particle p;
		
		p.min = make_vector4f(
			v.position.x - half,
			v.position.y - half,
			v.position.z - half,
			1.0f);
		p.max = make_vector4f(
			v.position.x + half,
			v.position.y + half,
			v.position.z + half,
			1.0f);

		coll->addParticle(p);
	}
	// Solo subdivide si hay muchas partículas
	if (coll->getParticleCount() > 4) {
		coll->subdivide();
	}

	coll->update(modelMatrix);
}
