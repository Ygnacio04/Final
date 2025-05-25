#pragma once
#include "common.h"
#include "vectorMath.h"

typedef enum {
    sphere, AABB
}BVType;

//particulas, unidades mínimas de colisión
class BV {
public:
    BVType type;
    BV() {};
    virtual bool collision(BV* bv2)=0;
    virtual void update(matrix4x4f& mat, vector4f size)=0;
    virtual vector4f getSize() = 0;
    virtual vector4f getCenter() = 0;
};

class SphereBV : public BV {

public:
    vector4f center = { 0,0,0,1 };
    vector4f centerOrigin = { 0,0,0,1 };


    float radiusOrigin = 0;
    float radius=0;



    SphereBV() { type = sphere; };

    SphereBV(vector4f center, float radious);
    bool collision(BV* bv2);
    void update(matrix4x4f& mat,vector4f size);

    vector4f getSize() { return { radius * 2.0f,radius * 2.0f, radius * 2.0f, 0 }; };
    vector4f getCenter() {return center;};

};

class VBHNode
{
public:
    vector4f max = { -numeric_limits<float>::max(),-numeric_limits<float>::max(),-numeric_limits<float>::max(),1 };
    vector4f min = { numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max(),1 };

    VBHNode* parent=nullptr;
    BV* volumeTest=nullptr;
    vector<BV*> particles;
    VBHNode* sons[2] = { nullptr,nullptr };

    VBHNode() { }
    void subdivide();//generar dos hijos a partir del volumeTest, repartir particulas entre hijos, ajustar volumen

    bool collision(VBHNode* node);//dos nodos colisionan si alguno de sus hijos colisionan
    
    void update(matrix4x4f& mat, vector4f size);

    //añade un BV y recalcula volumeTest
    void addParticle(BV* newParticle);

};

class VBH { //jerarquía

public:


    VBHNode* root=nullptr;
    VBH() { root = new VBHNode(); };
    void addParticle(BV* newParticle) { root->addParticle(newParticle); };
    void subdivide() { root->subdivide(); };
    void update(matrix4x4f& mat, vector4f size) { root->update(mat, size); };

};
