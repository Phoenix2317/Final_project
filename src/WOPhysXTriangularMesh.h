#pragma once
#include "WO.h"
#include "PxPhysicsAPI.h"

namespace Aftr
{
    class WOPhysXTriangularMesh : public WO
    {
    public:

        WOPhysXTriangularMesh();

        static WOPhysXTriangularMesh* New(const std::string& path, Vector scale, MESH_SHADING_TYPE mst, physx::PxPhysics* p, physx::PxScene* scene);

        void onCreate(const std::string& path, const Vector& scale, Aftr::MESH_SHADING_TYPE mst, physx::PxPhysics *p, physx::PxScene* scene);

    private:
        float* vertexListCopy;
        unsigned int* indicesCopy;
        physx::PxRigidStatic* actor = nullptr;

    };

}

