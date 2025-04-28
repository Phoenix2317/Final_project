#include "GLViewFinal_project.h"

#include "WorldList.h" //This is where we place all of our WOs
#include "ManagerOpenGLState.h" //We can change OpenGL State attributes with this
#include "Axes.h" //We can set Axes to on/off with this
#include "PhysicsEngineODE.h"

//Different WO used by this module
#include "WO.h"
#include "WOStatic.h"
#include "WOStaticPlane.h"
#include "WOStaticTrimesh.h"
#include "WOTrimesh.h"
#include "WOHumanCyborg.h"
#include "WOHumanCal3DPaladin.h"
#include "WOWayPointSpherical.h"
#include "WOLight.h"
#include "WOSkyBox.h"
#include "WOCar1970sBeater.h"
#include "Camera.h"
#include "CameraStandard.h"
#include "CameraChaseActorSmooth.h"
#include "CameraChaseActorAbsNormal.h" 
#include "CameraChaseActorRelNormal.h"
#include "Model.h"
#include "ModelDataShared.h"
#include "ModelMesh.h"
#include "ModelMeshDataShared.h"
#include "ModelMeshSkin.h"
#include "WONVStaticPlane.h"
#include "WONVPhysX.h"
#include "WONVDynSphere.h"
#include "WOImGui.h" //GUI Demos also need to #include "AftrImGuiIncludes.h"
#include "AftrImGuiIncludes.h"
#include "AftrGLRendererBase.h"
#include "MGLIndexedGeometry.h"
#include "IndexedGeometrySphereTriStrip.h"
#include "WOAxesTubes.h"
#include "AftrTimer.h"
#include <chrono>

#include "NetMsgMovePotion.h"
#include "NetMsgCreateWO.h"
#include "WOPhysx.h"

#include "PxPhysicsAPI.h"

//Irklang files for sound
#include "irrKlang.h"




using namespace Aftr;

GLViewFinal_project* GLViewFinal_project::New( const std::vector< std::string >& args )
{
   GLViewFinal_project* glv = new GLViewFinal_project( args );
   glv->init( Aftr::GRAVITY, Vector( 0, 0, -1.0f ), "aftr.conf", PHYSICS_ENGINE_TYPE::petODE );
   glv->onCreate();
   return glv;
 

}


GLViewFinal_project::GLViewFinal_project( const std::vector< std::string >& args ) : GLView( args )
{
   //Initialize any member variables that need to be used inside of LoadMap() here.
   //Note: At this point, the Managers are not yet initialized. The Engine initialization
   //occurs immediately after this method returns (see GLViewFinal_project::New() for
   //reference). Then the engine invoke's GLView::loadMap() for this module.
   //After loadMap() returns, GLView::onCreate is finally invoked.

   //The order of execution of a module startup:
   //GLView::New() is invoked:
   //    calls GLView::init()
   //       calls GLView::loadMap() (as well as initializing the engine's Managers)
   //    calls GLView::onCreate()

   //GLViewFinal_project::onCreate() is invoked after this module's LoadMap() is completed.
}


void GLViewFinal_project::onCreate()
{
   //GLViewFinal_project::onCreate() is invoked after this module's LoadMap() is completed.
   //At this point, all the managers are initialized. That is, the engine is fully initialized.

   if( this->pe != NULL )
   {
      //optionally, change gravity direction and magnitude here
      //The user could load these values from the module's aftr.conf
      this->pe->setGravityNormalizedVector( Vector( 0,0,-1.0f ) );
      this->pe->setGravityScalar( Aftr::GRAVITY );
   }
   this->setActorChaseType( STANDARDEZNAV ); //Default is STANDARDEZNAV mode
   //this->setNumPhysicsStepsPerRender( 1 ); //pause physics engine on start up; will remain paused till set to 1
   
}


GLViewFinal_project::~GLViewFinal_project()
{
   //Implicitly calls GLView::~GLView()
    GLView::~GLView();
   

    this->cube->~WOPhysx();
    this->cube = nullptr;
    this->cube2->~WOPhysx();
    this->cube2 = nullptr;
    this->engine->removeAllSoundSources();
    this->bg->drop();
    this->bg = nullptr;
    this->moon_noise->drop();
    this->moon_noise = nullptr;
    this->engine->irrklang::ISoundEngine::drop();
    this->engine = nullptr;


    this->gPvd->release();
    this->transport->release();
    this->p->release();
    this->scene->release();
    this->f->release();
    gPvd = nullptr;
    transport = nullptr;
    p = nullptr;
    scene = nullptr;
    f = nullptr;

}


void GLViewFinal_project::updateWorld()
{
   GLView::updateWorld(); //Just call the parent's update world first.
                          //If you want to add additional functionality, do it after
                          //this call.


   //Customize the world's behavior. Typically one would call a function to keep this short
   //but for the sake of a minimial example, we'll do something mildly interesting here

   if( this->gulfstream != nullptr && this->moon != nullptr )
      this->moon->setPose( 
         this->orbit_gui.compute_pose( this->gulfstream->getModel()->getPose() ) );

   if (this->moon != nullptr) { //moving emitter

       this->moon_pos.set(this->moon->getPosition().x, this->moon->getPosition().y, this->moon->getPosition().z);
       this->moon_noise->setPosition(moon_pos);


   }

   //Updating listener
   if (this->cam != nullptr) {

       this->listen_pos.set(this->cam->getPosition().x, this->cam->getPosition().y, this->cam->getPosition().z);
       this->listen_look.set(this->cam->getLookDirection().x, this->cam->getLookDirection().y, this->cam->getLookDirection().z);

        this->engine->setListenerPosition(this->listen_pos, this->listen_look);

   }

   if (playing) {


    physxUpdate();

   }

   start = std::chrono::system_clock::now();

  // std::cout << "Time: " << ManagerSDLTime::getTimeSinceLastMainLoopIteration() << " Secs\n";

  

}


void GLViewFinal_project::onResizeWindow( GLsizei width, GLsizei height )
{
   GLView::onResizeWindow( width, height ); //call parent's resize method.
}


void GLViewFinal_project::onMouseDown( const SDL_MouseButtonEvent& e )
{
   GLView::onMouseDown( e );
}


void GLViewFinal_project::onMouseUp( const SDL_MouseButtonEvent& e )
{
   GLView::onMouseUp( e );
}


void GLViewFinal_project::onMouseMove( const SDL_MouseMotionEvent& e )
{
   GLView::onMouseMove( e );
}


void GLViewFinal_project::onKeyDown(const SDL_KeyboardEvent& key)
{
    GLView::onKeyDown(key);
    if (key.keysym.sym == SDLK_0)
        this->setNumPhysicsStepsPerRender(1);

    

    if (key.keysym.sym == SDLK_w) { //Allows player to move forward
        this->cam->moveInLookDirection();
    }

    if (key.keysym.sym == SDLK_d) { //allows player to move to the right
        this->cam->moveRight();
    }

    if (key.keysym.sym == SDLK_s) { //allows player to move backwards
        this->cam->moveOppositeLookDirection();
    }

    if (key.keysym.sym == SDLK_a) { //allows the player to move left
        this->cam->moveLeft();
    }

    if (key.keysym.sym == SDLK_p) { //plays music
        
        if (this->bg->getIsPaused()) {

            this->bg->setIsPaused(false);

        }
        else {
            
            this->bg->setIsPaused(true);

        }

    }

    if (key.keysym.sym == SDLK_MINUS) { //decreases volume of background music



        if (this->bg->getVolume() > 0) {

            this->bg->setVolume(bg->getVolume() - 0.1);

        } 

    }

    if (key.keysym.sym == SDLK_EQUALS) { //Increases volume of background music; Used Equals since its in the same spot as PLUS and PLUS wasn't workng

        std::cout << "Plus was hit\n";

        if (this->bg->getVolume() < 1) {

            this->bg->setVolume(bg->getVolume() + 0.1);

        }

    }

    if (key.keysym.sym == SDLK_SPACE) {

        msgWO.xPos = 50;
        msgWO.yPos = 50;
        msgWO.zPos = 25;
       // client->sendNetMsgSynchronousTCP(msgWO);

        msgWO.xPos = 0;
        msgWO.yPos = 0;
        msgWO.zPos = 0;
       

    }

}


void GLViewFinal_project::onKeyUp( const SDL_KeyboardEvent& key )
{
   GLView::onKeyUp( key );
}


void Aftr::GLViewFinal_project::loadMap()
{
    
   this->worldLst = new WorldList(); //WorldList is a 'smart' vector that is used to store WO*'s
   this->actorLst = new WorldList();
   this->netLst = new WorldList();

   

   { //Making physx scene
       PxSceneDesc sc(p->getTolerancesScale());
       sc.filterShader = PxDefaultSimulationFilterShader;
       sc.cpuDispatcher = PxDefaultCpuDispatcherCreate(2);
       this->scene = p->createScene(sc);
       this->scene->setFlag(physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS, true);

       this->scene->setGravity(PxVec3(0.0f, 0.0f, -9.8f));

       this->gPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

       this->pvdClient = scene->getScenePvdClient();
       if (pvdClient) {
           this->pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
           this->pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
           this->pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);

           
       }

       

   }

   {//adding to the scene

       PxMaterial* gMaterial = p->createMaterial(0.5f, 0.5f, 0.6f);
       PxRigidStatic* groundPlane = PxCreatePlane(*p, PxPlane(0, 0, 1, -0.75), *gMaterial);
       this->scene->addActor(*groundPlane);

   }

   //Background music
   
   this->bg = this->engine->play2D((ManagerEnvironmentConfiguration::getLMM() + "/sounds/getout.ogg").c_str(), true, true, true);

   ManagerOpenGLState::GL_CLIPPING_PLANE( 1000.0 );
   ManagerOpenGLState::GL_NEAR_PLANE( 0.1f );
   ManagerOpenGLState::enableFrustumCulling( false );
   Axes::isVisible = true;
   this->glRenderer->isUsingShadowMapping( false ); //set to TRUE to enable shadow mapping, must be using GL 3.2+

   this->cam->setPosition( 15,15,10 );

   std::string shinyRedPlasticCube( ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl" );
   std::string wheeledCar( ManagerEnvironmentConfiguration::getSMM() + "/models/rcx_treads.wrl" );
   std::string grass( ManagerEnvironmentConfiguration::getSMM() + "/models/grassFloor400x400_pp.wrl" );
   std::string human( ManagerEnvironmentConfiguration::getSMM() + "/models/human_chest.wrl" );

   std::string moon_map(ManagerEnvironmentConfiguration::getLMM() + "models/Moon_Plane/scene.gltf");
   
   //SkyBox Textures readily available
   std::vector< std::string > skyBoxImageNames; //vector to store texture paths
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_water+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_dust+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_mountains+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_winter+6.jpg" );
  // skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/early_morning+6.jpg" );
  // skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_afternoon+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_cloudy+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_cloudy3+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_day+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_day2+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_deepsun+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_evening+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_morning+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_morning2+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_noon+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_warp+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_Hubble_Nebula+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_gray_matter+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_easter+6.jpg" );
   skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_hot_nebula+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_ice_field+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_lemon_lime+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_milk_chocolate+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_solar_bloom+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_thick_rb+6.jpg" );

   {
      //Create a light
      float ga = 0.1f; //Global Ambient Light level for this module
      ManagerLight::setGlobalAmbientLight( aftrColor4f( ga, ga, ga, 1.0f ) );
      WOLight* light = WOLight::New();
      light->isDirectionalLight( true );
      light->setPosition( Vector( 0, 0, 100 ) );
      //Set the light's display matrix such that it casts light in a direction parallel to the -z axis (ie, downwards as though it was "high noon")
      //for shadow mapping to work, this->glRenderer->isUsingShadowMapping( true ), must be invoked.
      light->getModel()->setDisplayMatrix( Mat4::rotateIdentityMat( { 0, 1, 0 }, 90.0f * Aftr::DEGtoRAD ) );
      light->setLabel( "Light" );
      worldLst->push_back( light );
   }

   {
      //Create the SkyBox

       

      WO* wo = WOSkyBox::New( skyBoxImageNames.at( 0 ), this->getCameraPtrPtr() );
      wo->setPosition( Vector( 0, 0, 0 ) );
      wo->setLabel( "Sky Box" );
      wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
      worldLst->push_back( wo );

     

   }

   { 
   //   ////Create the infinite grass plane (the floor)
   //   WO* wo = WO::New( grass, Vector( 1, 1, 1 ), MESH_SHADING_TYPE::mstFLAT );
   //   wo->setPosition( Vector( 0, 0, 0 ) );
   //   wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   //   wo->upon_async_model_loaded( [wo]()
   //      {
   //         ModelMeshSkin& grassSkin = wo->getModel()->getModelDataShared()->getModelMeshes().at( 0 )->getSkins().at( 0 );
   //         grassSkin.getMultiTextureSet().at( 0 ).setTexRepeats( 5.0f );
   //         grassSkin.setAmbient( aftrColor4f( 0.4f, 0.4f, 0.4f, 1.0f ) ); //Color of object when it is not in any light
   //         grassSkin.setDiffuse( aftrColor4f( 1.0f, 1.0f, 1.0f, 1.0f ) ); //Diffuse color components (ie, matte shading color of this object)
   //         grassSkin.setSpecular( aftrColor4f( 0.4f, 0.4f, 0.4f, 1.0f ) ); //Specular color component (ie, how "shiney" it is)
   //         grassSkin.setSpecularCoefficient( 10 ); // How "sharp" are the specular highlights (bigger is sharper, 1000 is very sharp, 10 is very dull)
   //      } );
   //   wo->setLabel( "Grass" );
   //   worldLst->push_back( wo );

   //   
   }

   { //create moon terrain

       
       WO* floor = WO::New(moon_map, Vector(5, 5, 1), MESH_SHADING_TYPE::mstFLAT);
       floor->setPosition(Vector(0, 0, 0));
       floor->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
       floor->upon_async_model_loaded([floor]() {

           ModelMeshSkin& moonSkin = floor->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0);
           moonSkin.getMultiTextureSet().at(0).setTexRepeats(5.0f);
           moonSkin.setAmbient(aftrColor4f(0.4f, 0.4f, 0.4f, 1.0f));
           moonSkin.setDiffuse(aftrColor4f(1.0f, 1.0f, 1.0f, 1.0f));
           moonSkin.setSpecular(aftrColor4f(0.4f, 0.4f, 0.4f, 1.0f));
           moonSkin.setSpecularCoefficient(50);

           });

       floor->setLabel("moon floor");
       worldLst->push_back(floor);

   }

   {
   //   this->gulfstream = WO::New( ManagerEnvironmentConfiguration::getSMM() + "/models/Aircraft/Gulfstream3/G3.obj", Vector(1.0f, 1.0f, 1.0f ), MESH_SHADING_TYPE::mstAUTO );
   //   this->gulfstream->setPosition( Vector( 0, 0, 10 ) );
   //   this->gulfstream->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   //   this->gulfstream->upon_async_model_loaded( [this]()
   //      {
   //         ModelMeshSkin& skin = this->gulfstream->getModel()->getModelDataShared()->getModelMeshes().at( 0 )->getSkins().at( 0 );
   //         skin.setAmbient( aftrColor4f( 0.1f, 0.1f, 0.1f, 1.0f ) ); //Color of object when it is not in any light
   //         //skin.setDiffuse( aftrColor4f( .1f, .1f, .5f, 1.0f ) ); //Diffuse color components (ie, matte shading color of this object) // Make it blue? Why not?
   //         skin.setSpecular( aftrColor4f( 0.4f, 0.4f, 0.4f, 1.0f ) ); //Specular color component (ie, how "shiney" it is)
   //         skin.setSpecularCoefficient( 10 ); // How "sharp" are the specular highlights (bigger is sharper, 1000 is very sharp, 10 is very dull)
   //      } );
   //   gulfstream->setLabel( "Gulfstream GIII" );
   //   worldLst->push_back( this->gulfstream );
   }

   {
   //   //Make a sphere
   //   this->moon = WO::New();
   //   MGLIndexedGeometry* mglSphere = MGLIndexedGeometry::New( this->moon );
   //   IndexedGeometrySphereTriStrip* geoSphere = IndexedGeometrySphereTriStrip::New( 3.0f, 12, 12, true, true );
   //   mglSphere->setIndexedGeometry( geoSphere );
   //   this->moon->setModel( mglSphere );
   //   this->moon->setLabel( "Moon" );
   //   this->moon->setPosition( { 15,15,15 } );
   //   this->moon->renderOrderType = RENDER_ORDER_TYPE::roTRANSPARENT;
   //   this->worldLst->push_back( this->moon );


   //  


   //   //Place a texture on the sphere, now its a moon
   //   fmt::print( "To the moon...\n" );
   //   Tex tex = *ManagerTex::loadTexAsync( ManagerEnvironmentConfiguration::getSMM() + "/images/moonMap.jpg" );
   //   this->moon->getModel()->getSkin().getMultiTextureSet().at( 0 ) = tex;
   //   this->moon->setPosition( {15,2,10});


   //   //setting up 3D sound for Moon
   //   if (this->moon != nullptr) {

   //       this->moon_pos.set(this->moon->getPosition().x, this->moon->getPosition().y, this->moon->getPosition().z);

   //       this->moon_noise = this->engine->play3D((ManagerEnvironmentConfiguration::getLMM() + "/sounds/Space.mp3").c_str(), this->moon_pos, true, false, true);

   //   }

   //   this->listen_pos.set(this->cam->getPosition().x, this->cam->getPosition().y, this->cam->getPosition().z);
   //   this->listen_look.set(this->cam->getLookDirection().x, this->cam->getLookDirection().y, this->cam->getLookDirection().z);
   //   
   // 

   //   this->engine->setListenerPosition(this->listen_pos, this->listen_look);


      //We always want some axes, too!
   //   WO* axes = WOAxesTubes::New( { 15.0f,15.0f,15.0f }, .2f );
   //   axes->setParentWorldObject( this->moon );
   //   axes->setPosition( this->moon->getPosition() ); //match parent position before locking
   //   axes->lockWRTparent(); //makes a joint that "welds" this child rigidly to parent
   //   this->moon->getChildren().push_back( axes );     
   }

   {//setting up moon lander
       
       this->lander = WOPhysx::New((ManagerEnvironmentConfiguration::getLMM() + "/models/Apollo_Lunar_Module.glb").c_str(), Vector(1.0f, 1.0f, 1.0f), MESH_SHADING_TYPE::mstAUTO, this->scene,this->p, "lander");
       this->lander->setPosition(35.0f, 30.0f, 10.0f);
       this->lander->renderOrderType = RENDER_ORDER_TYPE::roOVERLAY;
       this->lander->upon_async_model_loaded([this]()
           {
               ModelMeshSkin& skin = this->lander->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0);
               skin.setAmbient(aftrColor4f(0.1f, 0.1f, 0.1f, 1.0f)); //Color of object when it is not in any light
               skin.setDiffuse(aftrColor4f(.1f, .1f, .5f, 1.0f)); //Diffuse color components (ie, matte shading color of this object) // Make it blue? Why not?
               skin.setSpecular(aftrColor4f(0.4f, 0.4f, 0.4f, 1.0f)); //Specular color component (ie, how "shiney" it is)
               skin.setSpecularCoefficient(10); // How "sharp" are the specular highlights (bigger is sharper, 1000 is very sharp, 10 is very dull)

           });
       lander->setLabel("Moon_Lander");
       worldLst->push_back(lander);
   }

  

   
   { //making a physics cube


   //   this->cube = WOPhysx::New((ManagerEnvironmentConfiguration::getLMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl").c_str(), Vector(1.0f, 1.0f, 1.0f), MESH_SHADING_TYPE::mstAUTO, this->scene,this->p, "cube");
   //    
   //    this->cube->setPosition(35.0f,15.0f,4.0f);
   //    this->cube->renderOrderType = RENDER_ORDER_TYPE::roOVERLAY;
   //    this->cube->upon_async_model_loaded([this]() 
   //         {

   //            ModelMeshSkin& skin = this->cube->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0);
   //            skin.setAmbient(aftrColor4f(0.1f, 0.1f, 0.1f, 1.0f));
   //            skin.setDiffuse(aftrColor4f(0.1f, 0.1f, 0.5f, 1.0f));
   //            skin.setSpecular(aftrColor4f(0.4f, 0.4f, 0.4f, 1.0f));
   //            skin.setSpecularCoefficient(100);

   //         });
   //    this->cube->setLabel("Shiny");

   //    worldLst->push_back(this->cube);

   //    this->cube2 = WOPhysx::New((ManagerEnvironmentConfiguration::getLMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl").c_str(), Vector(1.0f, 1.0f, 1.0f), MESH_SHADING_TYPE::mstAUTO, this->scene, this->p, "cube");

   //    this->cube2->setPosition(35.0f, 15.0f, 20.0f);
   //    this->cube2->renderOrderType = RENDER_ORDER_TYPE::roOVERLAY;
   //    this->cube2->upon_async_model_loaded([this]()
   //        {

   //            ModelMeshSkin& skin = this->cube2->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0);
   //            skin.setAmbient(aftrColor4f(0.1f, 0.1f, 0.1f, 1.0f));
   //            skin.setDiffuse(aftrColor4f(0.1f, 0.1f, 0.5f, 1.0f));
   //            skin.setSpecular(aftrColor4f(0.4f, 0.4f, 0.4f, 1.0f));
   //            skin.setSpecularCoefficient(100);

   //        });
   //    this->cube2->setLabel("Shiny2");
   //    worldLst->push_back(this->cube2);

   }


   // Let's make a GUI. We create a WOImGui instance, and then use the strategy pattern to
   // submit/subscribe lambdas/std::functions/callbacks (~same thing) to draw our desired widgets.
   // We nestle the callbacks inside a menu to keep everything organized. The menu uses an on/off
   // toggle -- when on, that menu item's corresponding callback will be invoked each frame, when
   // off that callback is not invoked and therefore the corresponding window is not drawn.

   {
      this->gui = WOImGui::New( nullptr );
      gui->setLabel( "My Gui" );
      //callbacks -- When the user toggles the checkbox from the menu, call these callbacks:
      
      //This callback shows the WOEditor window. It will be visible when the user
      //selected Menu -> Edit -> Show WO Editor (as linked up below).
      auto woEditFunc = [this]() { this->wo_editor.draw( this->getLastSelectionQuery(), *this->getWorldContainer(), this->getCamera_functor() ); };

      //We will put these demo items under the "Demo" menu
      //auto showDemoWindow_ImGui     = [this]() { ImGui::ShowDemoWindow(); };
      //auto showDemoWindow_AftrDemo  = [this]() { WOImGui::draw_AftrImGui_Demo(this->gui); };
      //auto showDemoWindow_ImGuiPlot = [this]() { ImPlot::ShowDemoWindow(); };
      //auto show_moon_orbit_params   = [this]() { this->orbit_gui.draw(); };
      //auto show_gun_spawn           = [this]() { this->gun_gui.draw_gun(this->worldLst, this->getCameraPtrPtr() ); }; //draws the gun gui
      auto run_game                   = [this]() {playing = this->running.startRunning();};

      this->gui->subscribe_drawImGuiWidget(
         [=,this]() //this is a lambda, the capture clause is in [], the input argument list is in (), and the body is in {}
         {
            //We defined the callbacks above, now hook them into the menu labels
           /* menu.attach( "Edit", "Show WO Editor", woEditFunc );
            menu.attach( "Demos", "Show Default ImGui Demo", showDemoWindow_ImGui );
            menu.attach( "Demos", "Show Default ImPlot Demo", showDemoWindow_ImGuiPlot );
            menu.attach( "Demos", "Show Aftr ImGui w/ Markdown & File Dialogs", showDemoWindow_AftrDemo );
            menu.attach( "Orbit Gui", "Show Orbit", show_moon_orbit_params, true );*/
            menu.attach("Game status", "Playing game", run_game, true); 
            menu.draw(); //The menu.draw() is the entry point for your gui. It is called once per frame to draw the GUI.
         } );
      this->worldLst->push_back( this->gui );
   }

   



   createFinal_projectWayPoints();
}

void GLViewFinal_project::physxUpdate() {

    this->end = std::chrono::system_clock::now();

    this->elapsed_seconds = this->end - this->start;

   /* this->start = std::chrono::system_clock::now();*/

    this->scene->simulate(elapsed_seconds.count());

    PxU32 errorState = 0;
    this->scene->fetchResults(true);

    {
        physx::PxU32 numActors = 0;
        physx::PxActor** actors = this->scene->getActiveActors(numActors);

        for (physx::PxU32 i = 0; i < numActors; ++i) {
            physx::PxActor* actor = actors[i];

            WOPhysx* wo = static_cast<WOPhysx*>(actor->userData);
            wo->updatePoseFromPhysicsEngine();
        }
    }

    
}

void GLViewFinal_project::createFinal_projectWayPoints()
{
   // Create a waypoint with a radius of 3, a frequency of 5 seconds, activated by GLView's camera, and is visible.
   WayPointParametersBase params(this);
   params.frequency = 5000;
   params.useCamera = true;
   params.visible = true;
   WOWayPointSpherical* wayPt = WOWayPointSpherical::New( params, 3 );
   wayPt->setPosition( Vector( 50, 0, 3 ) );
   worldLst->push_back( wayPt );
}

