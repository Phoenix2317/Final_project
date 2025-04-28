#pragma once
#include "AftrConfig.h"
#ifdef  AFTR_CONFIG_USE_IMGUI

#include "Vector.h"
#include "Mat4.h"
#include <functional>
#include <chrono>

//added by Kevin Belock
#include "WorldContainer.h"
#include "WorldList.h"
#include "GLView.h"
#include "Model.h"
#include "ModelDataShared.h"
#include "ModelMesh.h"
#include "ModelMeshDataShared.h"
#include "ModelMeshSkin.h"

namespace Aftr
{

   class WO;

class AftrImGui_Final_project
{
public:
   //main draw method called from GLView
   void draw();

   //Calls the draw function for the gun gui
   // /requests the class holding the world list of objects
   // /requests the camera actor of the object
   void draw_gun(WorldContainer*& worldLst, Camera** myCam); 
   //Given an origin, this returns a radial orbit in the XY plan specified by the
   //origin orientation and position. Each revolution takes the specified time.
   Mat4 compute_pose( Mat4 const& origin_pose );

   /*Mat4 gun_rotation(Mat4 const& origin);*/

   bool startRunning();

private:
   //draws the gui widgets that let the user manipulate orbit parameters
   void draw_orbit_controls();
   //draws the gun gui and will spawn the gun in using the world list and camera object given to it
   void draw_gun_spawn(WorldContainer*& worldLst, Camera** myCam);

	bool running();

   float radius_m = 100.0f;   //adjusted by gui slider
   float current_Relangle_x = 0.0f;
   float current_Relangle_y = 0.0f;
   float current_Relangle_z = 0.0f;
   float v_Relradx_g = 0.0f;
   float v_Relrady_g = 0.0f;
   float v_Relradz_g = 0.0f;

   float current_Globangle_x = 0.0f;
   float current_Globangle_y = 0.0f;
   float current_Globangle_z = 0.0f;
   float v_Globradx_g = 0.0f;
   float v_Globrady_g = 0.0f;
   float v_Globradz_g = 0.0f;

   bool isRunning = false;
   bool isPlaying = false;

   WO* hold_pointer;
   bool isSpawned = false;
   bool isPaused = false;     //adjusted by gui button in draw_orbit_controls
   int orbitTime_msec = 5000; //adjusted by gui slider in draw_orbit_controls
   std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now(); //used by draw_orbit_controls
   std::chrono::system_clock::time_point pause_time; //used by draw_orbit_controls
   std::chrono::system_clock::time_point now_time;   //used by compute_pose()
};

}

#endif //  AFTR_CONFIG_USE_IMGUI
