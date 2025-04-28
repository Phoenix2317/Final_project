#pragma once
#include "Vector.h"
#include "VectorFwd.h"
#include "WO.h"
#include "PxPhysicsAPI.h"
#include "GLView.h"
#include "WOPhysXTriangularMesh.h"

using namespace physx;

namespace Aftr
{

	

	class WOGridECEFElevationPhysx : public WO {

	public:

		WOGridECEFElevationPhysx();
		~WOGridECEFElevationPhysx();

		static WOGridECEFElevationPhysx* New(PxPhysics* p, PxScene* scene, Vector upperLeft, Vector lowerRight, int granular, Vector offset, Vector scale, std::string& path, std::vector<std::vector<VectorD>>& grid);
		
		void onCreate(PxPhysics* p, PxScene* scene, Vector upperLeft, Vector lowerRight, int granular, Vector offset, Vector &scale, std::string path, std::vector<std::vector<VectorD>> &grid);

		WOPhysXTriangularMesh* trimesh = nullptr;
		


	};

	class TerrainGen {
	public:

		//Genrates a local Tangent plane using a float or double value,
		 template <typename T>
		 void getLocalBodySpaceToLTPSpace(const VectorD& lla, T localBodySpacetoLTP[16]) {

			 VectorD ll = lla;
			 ll.z = 0;
			 VectorD zdir = ll.toECEFfromWGS84().normalizeMe();
			 VectorD northPoleECEF = VectorD(90.0, 0, 0).toECEFfromWGS84();
			 VectorD xdir = northPoleECEF - ll.toECEFfromWGS84();
			 xdir = xdir.vectorProjectOnToPlane(zdir);
			 xdir.normalize();
			 VectorD ydir = zdir.crossProduct(xdir);
			 ydir.normalize();

			 //LTP

			 localBodySpacetoLTP[0] = (T)xdir.x;
			 localBodySpacetoLTP[1] = (T)xdir.y;
			 localBodySpacetoLTP[2] = (T)xdir.z;
			 localBodySpacetoLTP[3] = (T)0;

			 localBodySpacetoLTP[4] = (T)ydir.x;
			 localBodySpacetoLTP[5] = (T)ydir.y;
			 localBodySpacetoLTP[6] = (T)ydir.z;
			 localBodySpacetoLTP[7] = (T)0;

			 localBodySpacetoLTP[8] = (T)zdir.x;
			 localBodySpacetoLTP[9] = (T)zdir.y;
			 localBodySpacetoLTP[10] = (T)zdir.z;
			 localBodySpacetoLTP[11] = (T)0;

			 localBodySpacetoLTP[12] = (T)0;
			 localBodySpacetoLTP[13] = (T)0;
			 localBodySpacetoLTP[14] = (T)0;
			 localBodySpacetoLTP[15] = (T)1.0f;


		 }

		 TerrainGen();
		 ~TerrainGen();

		 void createGrid(PxPhysics* p, PxScene* scene, Vector scale, std::string path);

		 Aftr::WOGridECEFElevationPhysx* gridEleva = nullptr;

	protected:

		float top = 0;
		float bottom = 0;
		
		float left = 0;
		float right = 0;

		float vert = 0;
		float horz = 0;

		Vector offset;
		
		Vector centerOfWorld;
		Vector gravityDirection;

		Vector scale = Vector(1.0f, 1.0f, 1.0f);
		Vector upperLeft;
		Vector lowerRight;


		


	};
}


