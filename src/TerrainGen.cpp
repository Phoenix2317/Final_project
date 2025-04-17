
#include "TerrainGen.h"
#include "Vector.h"
#include "VectorFwd.h"
#include "WO.h"
#include "PxPhysicsAPI.h"

using namespace Aftr;

TerrainGen::TerrainGen() {

	top = 8.0f;
	bottom = 8.0f;

	left = 1737.4f;
	right = 1737.4f;

	vert = top - bottom;
	horz = left - right;

	offset.x = (top + bottom) / 2;
	offset.y = (left + right) / 2;
	offset.z = 0;

	centerOfWorld = offset.toVecS().toECEFfromWGS84();
	gravityDirection = centerOfWorld;
	gravityDirection.normalize();

	upperLeft.x = top;
	upperLeft.y = left;
	upperLeft.z = 0;

	lowerRight.x = bottom;
	lowerRight.y = right;
	lowerRight.z = 0;

}

void TerrainGen::createGrid() {

	


}


//WOGridECEFElevationPhysx* WOGridECEFElevationPhysx::New(PxPhysics*& p, PxScene*& scene, VectorD upperLeft, VectorD lowerRight, int z = 0, VectorD offset, Vector scale, std::string& path) {
//
//
//
//	/*WO::New(path, scale, MESH_SHADING_TYPE::mstAUTO);*/
//
//
//
//
//}