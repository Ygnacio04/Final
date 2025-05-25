#pragma once
#include "Object3D.h"

typedef enum {
    SPACESHIP=0,
    ENEMY = 1,
    BULLET =2 
};


class SpaceShip : public Object {

public:
    SpaceShip() : Object("data/spaceShip.fiis")
    {
        this->objectType = SPACESHIP;
    }

    virtual void update(float timeStep) override;

};



class Enemy : public Object {

public:
    Enemy() : Object("data/spaceShip.fiis")
    {
        this->objectType = ENEMY;
        rot.z = 180;
    }

    virtual void update(float timeStep) override;

};


class Bullet : public Object {

public:
    Bullet() : Object("data/Bullet.fiis")
    {
        this->objectType = BULLET;
        rot.z = 0;
        this->size = { 0.25,0.25, 0.25, 1 };

    }

    virtual void update(float timeStep) override;

};