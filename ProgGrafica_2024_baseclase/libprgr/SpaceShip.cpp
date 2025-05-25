#include "SpaceShip.h"
#include "EventManager.h"
#include "Render.h"

void SpaceShip::update(float timeStep)
{
    float speed = 2.0f;
    //si aprieto derecha o izquierda
    if (EventManager::keyState[GLFW_KEY_LEFT])
        this->pos.x -= speed*timeStep;

    if (EventManager::keyState[GLFW_KEY_RIGHT])
        this->pos.x += speed * timeStep;

    if (EventManager::keyState[GLFW_KEY_UP])
        this->pos.y += speed * timeStep;
    if (EventManager::keyState[GLFW_KEY_DOWN])
        this->pos.y -= speed * timeStep;

    //si aprieto espacio
        //dispara
    if (EventManager::keyState[GLFW_KEY_SPACE])
    {
        Object* bullet = new Bullet(); //crear bala en mi posición
        bullet->pos = this->pos;
        Render::newObjectList.push_back(bullet);//añadir al sistema

    }


    auto results = Render::getCollisions(this, ENEMY);

    if (results.size() > 0)
    {
       // this->isAlive = false;
        cout << "Coll\n";
    }else
        cout << "No Coll\n";



    //búsqueda de objetos a colisionar 
    updateCollider();
}

void Enemy::update(float timeStep)
{
    float speed = 1.0f;

    if (pos.x > 5) //recolocar
        pos.x = -5;
    //actualizar movimiento
    this->pos.x += speed * timeStep;
    updateCollider();
}

void Bullet::update(float timeStep)
{

    float speed = 2.0f;

    //si colisiona con algo, evento....
    //dependiendo tipo de bullet : Amiga o enemiga

    auto results = Render::getCollisions(this, ENEMY);

    for (auto& enemy : results)
    {
        enemy->isAlive = false;
        this->isAlive = false;
    }

    if (pos.y > 10) //recolocar
        this->isAlive = false;
    //actualizar movimiento
    this->pos.y += speed * timeStep;

    updateCollider();
}
