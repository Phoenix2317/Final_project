#pragma

#include "ManagerSerializableNetMsgMap.h"
#include "NetMessengerStreamBuffer.h"
#include "NetMsgMovePotion.h"
#include "GLViewFinal_project.h"
#include "WO.h"
#include "GLView.h"
#include "ManagerGLView.h"


using namespace Aftr;

NetMsgMacroDefinition( NetMsgMovePotion );


NetMsgMovePotion::NetMsgMovePotion()
{

	this->xPos = 0;
	this->yPos = 0;
	this->zPos = 0;

	this->xRot = 0;
	this->yRot = 0;
	this->zRot = 0;
	
	this->id = 0;

}

NetMsgMovePotion::~NetMsgMovePotion() {

}

bool NetMsgMovePotion::toStream(NetMessengerStreamBuffer& os) const
{

	os << this->xPos;
	os << this->yPos;
	os << this->zPos;

	os << this->xRot;
	os << this->yRot;
	os << this->zRot;

	os << this->id;

	return true;

}

bool NetMsgMovePotion::fromStream(NetMessengerStreamBuffer& is) {

	is >> this->xPos;
	is >> this->yPos;
	is >> this->zPos;

	is >> this->xRot;
	is >> this->yRot;
	is >> this->zRot;

	is >> this->id;

	return true;

}

void NetMsgMovePotion::onMessageArrived() {

	std::cout << "Message Recieved\n";

	ManagerGLView::getGLView()->getWorldContainer()->getWOByID(this->id)->moveRelative( Vector(xPos, yPos, zPos) );
	ManagerGLView::getGLView()->getWorldContainer()->getWOByID(this->id)->rotateAboutRelX(xRot);

	

}

std::string NetMsgMovePotion::toString() const {

	return "Payload\n";

}

