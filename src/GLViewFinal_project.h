#pragma once

#include "GLView.h"
#include "AftrImGui_MenuBar.h"
#include "AftrImGui_WO_Editor.h"
#include "AftrImGui_Final_project.h"

#include "NetMsg.h"
#include "NetMessengerClient.h"
#include "NetMsgMovePotion.h"
#include "NetMsgCreateWO.h"

#include "PxPhysicsAPI.h"
#include <chrono>
#include "WOPhysx.h"
#include "TerrainGen.h"

 



//Irklang files for sound
#include "irrKlang.h"


using namespace irrklang;
using namespace physx;

namespace Aftr
{
   class Camera;
   class WOImGui;

/**
   \class GLViewFinal_project
   \author Scott Nykl 
   \brief A child of an abstract GLView. This class is the top-most manager of the module.

   Read \see GLView for important constructor and init information.

   \see GLView

    \{
*/

class GLViewFinal_project : public GLView
{
public:
   static GLViewFinal_project* New( const std::vector< std::string >& outArgs );
   virtual ~GLViewFinal_project();
   virtual void updateWorld() override; ///< Called once per frame
   virtual void loadMap() override; ///< Called once at startup to build this module's scene
   virtual void createFinal_projectWayPoints();
   virtual void onResizeWindow( GLsizei width, GLsizei height ) override;
   virtual void onMouseDown( const SDL_MouseButtonEvent& e ) override;
   virtual void onMouseUp( const SDL_MouseButtonEvent& e ) override;
   virtual void onMouseMove( const SDL_MouseMotionEvent& e ) override;
   virtual void onKeyDown( const SDL_KeyboardEvent& key ) override;
   virtual void onKeyUp( const SDL_KeyboardEvent& key ) override;

   void physxUpdate();

   

protected:
   GLViewFinal_project( const std::vector< std::string >& args );
   virtual void onCreate();

   WOImGui* gui = nullptr; //The GUI which contains all ImGui widgets
   AftrImGui_MenuBar menu;      //The Menu bar at the top of the GUI window
   AftrImGui_WO_Editor wo_editor;//The WO Editor to mutate a selected WO
   AftrImGui_Final_project orbit_gui;

   AftrImGui_Final_project gun_gui; //Gui used to spawn the gun
   WO* moon = nullptr;
   WO* gulfstream = nullptr;

  
   WOPhysx* lander = nullptr;
  

   
   WOPhysx* cube = nullptr;
   WOPhysx* cube2 = nullptr;
   
   

   ISoundEngine* engine = createIrrKlangDevice();
   bool toggle = false;
   ISound* bg = nullptr;
   ISound* moon_noise = nullptr;

   irrklang::vec3df moon_pos;
   irrklang::vec3df listen_pos, listen_look;

  // std::shared_ptr<NetMessengerClient> client = NetMessengerClient::New("192.168.1.182");
   NetMsgMovePotion msg;
   NetMsgCreateWO msgWO;

   PxDefaultAllocator a;
   PxDefaultErrorCallback e;
   PxFoundation* f = PxCreateFoundation(PX_PHYSICS_VERSION, a, e);

   physx::PxPvd* gPvd = physx::PxCreatePvd(*f);
   physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
   

   PxPhysics* p = PxCreatePhysics(PX_PHYSICS_VERSION, *f, PxTolerancesScale(), true, gPvd);


   PxScene* scene = nullptr;
   physx::PxPvdSceneClient* pvdClient = nullptr;

   ManagerSDLTime dt;


   std::chrono::time_point<std::chrono::system_clock> start, end;
   std::chrono::duration<float> elapsed_seconds = std::chrono::milliseconds(0);
   

   TerrainGen* grid = nullptr;

   
  
};

/** \} */

} //namespace Aftr
