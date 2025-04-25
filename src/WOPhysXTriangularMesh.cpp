#include "WOPhysXTriangularMesh.h"
#include "WO.h"

#include "Model.h"
#include "PxPhysicsAPI.h"

using namespace Aftr;
using namespace physx;


WOPhysXTriangularMesh::WOPhysXTriangularMesh() : IFace(this)
{
	WO::initInstanceData();

}

WOPhysXTriangularMesh* WOPhysXTriangularMesh::New(const std::string& path, Vector scale, MESH_SHADING_TYPE mst, PxPhysics* p, PxScene* scene)
{
	WOPhysXTriangularMesh* tri = new WOPhysXTriangularMesh();
	tri->onCreate(path, scale, mst, p, scene);
	return tri;
}


void WOPhysXTriangularMesh::onCreate(const std::string& path, const Vector& scale, Aftr::MESH_SHADING_TYPE mst, PxPhysics* p, PxScene* scene) {

	WO::onCreate(path, scale, mst);

	size_t vertexListSize = this->getModel()->getModelDataShared()->getCompositeVertexList().size();
	size_t indexListSize = this->getModel()->getModelDataShared()->getCompositeIndexList().size();

	this->vertexListCopy = new float[vertexListSize * 3];
	this->indicesCopy = new unsigned int[indexListSize];

	for (size_t i = 0; i < vertexListSize; i++) {

		this->vertexListCopy[i*3+0] = this->getModel()->getModelDataShared()->getCompositeVertexList().at(i).x;
		this->vertexListCopy[i*3+1] = this->getModel()->getModelDataShared()->getCompositeVertexList().at(i).y;
		this->vertexListCopy[i*3+2] = this->getModel()->getModelDataShared()->getCompositeVertexList().at(i).z;

	}

	for (size_t i = 0; i < indexListSize; i++) {
		this->indicesCopy[i] = this->getModel()->getModelDataShared()->getCompositeIndexList().at(i);

	}

	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = vertexListSize;
	meshDesc.points.stride = sizeof(float) * 3;
	meshDesc.points.data = this->vertexListCopy;

	meshDesc.triangles.count = indexListSize / 3;
	meshDesc.triangles.stride = 3 * sizeof(unsigned int);
	meshDesc.triangles.data = this->indicesCopy;

	PxDefaultMemoryOutputStream writeBuffer;
	PxTriangleMeshCookingResult::Enum result;
	PxTolerancesScale tScale;
	PxCookingParams params(tScale);
	bool status = PxCookTriangleMesh(params, meshDesc, writeBuffer, &result);

	if (!status) {

		std::cout << "Failed to create Triangular mesh" << std::endl;
		std::cin.get();

	}

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	PxTriangleMesh* mesh = p->createTriangleMesh(readBuffer);

	PxMaterial* gMaterial = p->createMaterial(0.5f, 0.5f, 0.6f);
	PxShape* shape = p->createShape(PxTriangleMeshGeometry(mesh), *gMaterial, true);
	PxTransform t({ 0,0,0 });

	actor = p->createRigidStatic(t);
	bool b = actor->attachShape(*shape);

	actor->userData = this;
	scene->addActor(*actor);

}
