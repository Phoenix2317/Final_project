#include "AftrImGui_Final_project.h"
#include "AftrImGuiIncludes.h"
#include <fmt/core.h>
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

void Aftr::AftrImGui_Final_project::draw()
{
   this->draw_orbit_controls();
}


//calls the gun gui
void Aftr::AftrImGui_Final_project::draw_gun(WorldContainer*& worldLst, Camera** myCam) {

    this->draw_gun_spawn(worldLst, myCam);

}

void Aftr::AftrImGui_Final_project::draw_orbit_controls()
{
   if( ImGui::Begin( "Moon Spy" ) )
   {
      if( ImGui::Button( ( this->isPaused ? "Resume Motion" : "Pause Motion" ) ) )
      {
         this->isPaused = !this->isPaused;
         if( this->isPaused )
            this->pause_time = std::chrono::system_clock::now();
         else
            this->start_time = (std::chrono::system_clock::now() - pause_time) + start_time;
         fmt::print( "Moon Orbiter Status: {:b}...\n", this->isPaused );
      }

      if( ImGui::SliderFloat( "Radius (m)", &this->radius_m, 1.0f, 100.0f ) )
         fmt::print( "Adjusted Moon Orbiter radius to {:f} meters... Good job...\n", this->radius_m );

      if( ImGui::SliderInt( "Orbit time (msec)", &this->orbitTime_msec, 50, 10000 ) )
         fmt::print( "Adjusted Moon Orbiter time to {:d} milliseconds... Better job...\n", this->orbitTime_msec );

      ImGui::End();
   }
}


//creates the gun gui
void Aftr::AftrImGui_Final_project::draw_gun_spawn(WorldContainer*& worldLst, Camera** myCam) {
    

    if (ImGui::Begin("Gun drop")) //starts
    {

        if (ImGui::Button( (this->isSpawned ? "Despawn" : "Spawn") )) //button to spawn in the gun
        {
            if (!this->isSpawned) {

                //if true it generates a gun at the camera's position
                WO* gun = WO::New(ManagerEnvironmentConfiguration::getLMM() + "/models/gun/gun.3DS", Vector(2.0f, 3.0f, 1.0f), MESH_SHADING_TYPE::mstAUTO);
                gun->setPosition((*myCam)->getPosition());
            
                gun->renderOrderType = RENDER_ORDER_TYPE::roOVERLAY;
                gun->upon_async_model_loaded([gun]()
                {

                    ModelMeshSkin& coating = gun->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0);
                    coating.setAmbient(aftrColor4f(0.1f, 0.1f, 0.1f, 1.0f));
                    coating.setSpecular(aftrColor4f(0.4f, 0.6f, 0.8f, 2.0f));
                    coating.setSpecularCoefficient(100);

                });
                gun->setLabel("MK47");
                this->hold_pointer = gun;
                worldLst->push_back(gun);
                this->isSpawned = !this->isSpawned;
                
                

            }
            else {

                worldLst->eraseViaWOptr(this->hold_pointer);
                this->hold_pointer->~WO();
                this->isSpawned = !this->isSpawned;

            }
           
        }
        if (this->isSpawned) {

            if (ImGui::BeginChild(ImGui::GetID("Gun drop"))) {
               
                int index = worldLst->getIndexOfWO(this->hold_pointer);

                   if (index != -1) {

                       if (ImGui::SliderAngle("relative X-axis", &this->v_Relradx_g, 0.0f, 360.0f))
                       {
                           if ((this->v_Relradx_g - this->current_Relangle_x) != 0.0f) {

                               worldLst->at(index)->rotateAboutRelX(this->v_Relradx_g - this->current_Relangle_x);
                               this->current_Relangle_x = this->v_Relradx_g;

                           }
                  
                       }

                       if (ImGui::SliderAngle("relative Y-axis", &this->v_Relrady_g, 0.0f, 360.0f))
                       {
                           if ( (this->v_Relrady_g - this->current_Relangle_y) != 0.0f) {

                               worldLst->at(index)->rotateAboutRelY(this->v_Relrady_g - this->current_Relangle_y);
                               this->current_Relangle_y = this->v_Relrady_g;

                           }

                       }

                       if (ImGui::SliderAngle("relative Z-axis", &this->v_Relradz_g, 0.0f, 360.0f))
                       {
                           if ((this->v_Relradz_g - this->current_Relangle_z) != 0.0f) {

                               worldLst->at(index)->rotateAboutRelZ(this->v_Relradz_g - this->current_Relangle_z);
                               this->current_Relangle_z = this->v_Relradz_g;

                           }

                       }

                       if (ImGui::SliderAngle("Global X-axis", &this->v_Globradx_g, 0.0f, 360.0f))
                       {
                           if ((this->v_Globradx_g - this->current_Globangle_x) != 0.0f) {

                               worldLst->at(index)->rotateAboutGlobalX(this->v_Globradx_g - this->current_Globangle_x);
                               this->current_Globangle_x = this->v_Globradx_g;

                           }

                       }

                       if (ImGui::SliderAngle("Global Y-axis", &this->v_Globrady_g, 0.0f, 360.0f))
                       {
                           if ((this->v_Globrady_g - this->current_Globangle_y) != 0.0f) {

                               worldLst->at(index)->rotateAboutGlobalY(this->v_Globrady_g - this->current_Globangle_y);
                               this->current_Globangle_y = this->v_Globrady_g;

                           }

                       }

                       if (ImGui::SliderAngle("Global Z-axis", &this->v_Globradz_g, 0.0f, 360.0f))
                       {
                           if ((this->v_Globradz_g - this->current_Globangle_z) != 0.0f) {

                               worldLst->at(index)->rotateAboutGlobalZ(this->v_Globradz_g - this->current_Globangle_z);
                               this->current_Globangle_z = this->v_Globradz_g;

                           }

                       }


                       
                   }

                ImGui::EndChild();
            }

        }
        else {

            this->current_Relangle_x = 0.0f;
            this->current_Relangle_y = 0.0f;
            this->current_Relangle_z = 0.0f;
            this->v_Relradx_g = 0.0f;
            this->v_Relrady_g = 0.0f;
            this->v_Relradz_g = 0.0f;

            this->current_Globangle_x = 0.0f;
            this->current_Globangle_y = 0.0f;
            this->current_Globangle_z = 0.0f;
            this->v_Globradx_g = 0.0f;
            this->v_Globrady_g = 0.0f;
            this->v_Globradz_g = 0.0f;

        }

        
        //the end of the gui
        ImGui::End();
    }


}

Aftr::Mat4 Aftr::AftrImGui_Final_project::compute_pose( Mat4 const& origin )
{
   if( !this->isPaused )
      this->now_time = std::chrono::system_clock::now();

   //one revolution completes in exactly REV_TIME, compute parametric offset based on the current time
   std::chrono::duration REV_TIME = std::chrono::milliseconds( this->orbitTime_msec );

   auto delta_t = std::chrono::duration<float>( now_time - start_time );
   float t = float( delta_t / REV_TIME ); //parametric distance [0,1], between start time and now
   //fmt::print( "Num revolutions is {:f}\n", t);

   //Compute the pose of the orbiting object -- the position and the orientation
   Vector pos = origin.getPosition();
   Vector fwdVec = origin.getX();
   Vector upVec = origin.getZ();
   Vector orbit_pos = pos + (fwdVec * radius_m).rotate( upVec, t * Aftr::PI * 2.0f );
   Vector relX = (pos - orbit_pos).normalizeMe();
   Vector relY = upVec.crossProduct( relX );
   Vector relZ = relX.crossProduct( relY );
   Mat4 pose( relX, relY, relZ );
   pose.setPosition( orbit_pos );
   return pose;
}

