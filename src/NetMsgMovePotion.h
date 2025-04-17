
#pragma once

#include "NetMsg.h"


#ifdef AFTR_CONFIG_USE_BOOST

namespace Aftr
{

	class NetMsgMovePotion : public NetMsg
	{
	public:
			NetMsgMacroDeclaration(NetMsgMovePotion);

			NetMsgMovePotion();
			virtual ~NetMsgMovePotion();
			virtual bool toStream(NetMessengerStreamBuffer& os) const;
			virtual bool fromStream(NetMessengerStreamBuffer& is);
			virtual void onMessageArrived();
			virtual std::string toString() const;

			int xPos;
			int yPos;
			int zPos;

			float xRot;
			float yRot;
			float zRot;

			unsigned int id;
		

	protected:

	};

	}  //namespace Aftr

#endif