#pragma once



#include "WO.h"
#include "PxPhysicsAPI.h"
#include "GLView.h"

using namespace physx;

namespace Aftr
{


	class WOPhysx : public WO
	{

	public:

		WOPhysx();
		
		/**
		* 
		* \param modelFilename = Path to file
		* \param scale = Size of model
		* \param shadingtype = how it is shaded
		* \param scene = storgae for physics actor
		* \param p = main physics operator
		* \param body = shape of object (e.g. cube, cylinder, etc.)
		* 
		*/
		static WOPhysx* New(const std::string& modelFileName, Vector scale, MESH_SHADING_TYPE shadingType, PxScene* scene, PxPhysics* p, const char* body);

		~WOPhysx();

		void onCreate(const std::string& path, const Vector& scale, Aftr::MESH_SHADING_TYPE mst, PxScene* scene, PxPhysics* p, const char* body);


		void updatePoseFromPhysicsEngine();

		void setPosition(float x, float y, float z);



	private:

		PxRigidDynamic* actor = nullptr;

	};
}