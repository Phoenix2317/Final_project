

#include "WOPhysx.h"
#include "WO.h"
#include "PxPhysicsAPI.h"

using namespace Aftr;
using namespace physx;



WOPhysx* WOPhysx::New(const std::string& modelFileName, Vector scale, MESH_SHADING_TYPE shadingType, PxScene* scene, PxPhysics* p, const char* body)
{
    WOPhysx* wophysx = new WOPhysx();
    wophysx->onCreate(modelFileName, scale, shadingType, scene, p, body);
    return wophysx;
}

WOPhysx::WOPhysx() : IFace(this)
{
    WO::initInstanceData();
}


WOPhysx::~WOPhysx() {

    this->WO::~WO();
   
    actor->release();
    actor = nullptr;

}

void WOPhysx::onCreate(const std::string& path, const Vector& scale, Aftr::MESH_SHADING_TYPE mst, PxScene* scene, PxPhysics* p, const char* body) {

    WO::onCreate(path, scale, mst);

    PxShape* shape = nullptr;
    PxTransform t({});

    PxMaterial* gMaterial = p->createMaterial(0.5f, 0.5f, 0.6f);

    if (body = "cube") {

         shape = p->createShape(PxBoxGeometry(2,2,2), *gMaterial, true);
         t = { 20,20,20 };
    }
    else if (body = "lander") {

        shape = p->createShape(PxBoxGeometry(2,4, 2), *gMaterial, true);
        t = { 20,20,20 };
    
    }
    else if (body = "cylinder") {

        shape = p->createShape(PxCapsuleGeometry(2, 3), *gMaterial, true);

        t = { 0, 20, 20 };

    }

    this->actor = p->createRigidDynamic(t);

    actor->attachShape(*shape);

    actor->userData = this;

    scene->addActor(*actor);

}

void WOPhysx::updatePoseFromPhysicsEngine() {

    PxTransform t = actor->getGlobalPose();
    PxMat44 m = PxMat44(t);
    Aftr::Mat4 m2;

    for (int i = 0; i < 16; i++) {
        m2[i] = m(i % 4, i / 4);
    }
    
  

    this->setDisplayMatrix(m2);
    this->WO::setPosition(m2.getPosition());


}

void WOPhysx::setPosition(float x, float y, float z) {

    PxTransform t(x, y, z);

    this->actor->setGlobalPose(t);

}