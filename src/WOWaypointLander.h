#pragma once
#include "WOWayPointAbstract.h"
#include "WO.h"
namespace Aftr
{

	class GLView;
	
	class WO;

	

	class WOWaypointLander : public WOWayPointAbstract
	{
	public:
		WOWaypointLander(const WayPointParametersBase& params, float radius);
		static WOWaypointLander* New(WayPointParametersBase& base, float radius, bool& playing);
		void onTrigger();
		bool activate(Aftr::WO* activator);
		void onCreate(bool& playing);
		void changePlaying() { this->playing = !this->playing; }

	protected:
		float radius;
		bool playing = false;

	};

}


