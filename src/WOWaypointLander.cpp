#include "WOWaypointLander.h"
#include "GLViewFinal_project.h"
#include "ManagerGLView.h"
#include "MGLWayPointSpherical.h"

using namespace Aftr;

WOWaypointLander* WOWaypointLander::New(WayPointParametersBase& base, float radius, bool& playing) {

	WOWaypointLander* ptr = new WOWaypointLander(base, radius);
	ptr->onCreate(playing);
	return ptr;

}

bool WOWaypointLander::activate(Aftr::WO* activator) {

	if (activator == nullptr)
	{
		std::cout << "NULL activator!\n";
		return false;
	}
	Vector translateAmt = this->getPosition();
	Vector position = activator->getPosition();
	if (pow(translateAmt.x - position.x, 2.0f) + pow(translateAmt.y - position.y, 2.0f) + pow(translateAmt.z - position.z, 2.0f) < radius)
		return true;
	return false;

}

void WOWaypointLander::onCreate(bool& playing) {

	model = new MGLWayPointSpherical(this);
	this->renderOrderType = RENDER_ORDER_TYPE::roTRANSPARENT;
	this->playing = playing;

}

WOWaypointLander::WOWaypointLander(const WayPointParametersBase& params, float radius) : IFace(this), WOWayPointAbstract(params) {

	this->radius = radius * radius;

}

void WOWaypointLander::onTrigger() {
	

	std::cout << "\n\nCongragulations! You won!" << std::endl;

}