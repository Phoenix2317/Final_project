#New Module CMake Template written by Scott Nykl. This requires the engine is already built.
#Running "cmake ../src -LAH" from command line will list all CMake Variables. This is useful
#for extending and debugging CMake scripts.
cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)
PROJECT( "Final_project" ) #This command populates CMAKE_SIZEOF_VOID_P and should be one of the first commands in the file
set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}" )
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

#These are the files that will appear in the output project (ie, in MSVC, this will be in the vcxproj)
#All cpp files that ought to be compiled in this project must be included here
FILE( GLOB sources ${CMAKE_SOURCE_DIR}/*.cpp )
FILE( GLOB headers ${AFTR_USERLAND_LIB_PATH}/*.h )
message( STATUS "HEADERS: ${headers}" ) 
message( STATUS "SOURCES: ${sources}" )

SET( aftrModuleCMakeHelperCmakeIncludePath "../../../include/cmake/aftrModuleCMakeHelper.cmake" )
include( ${aftrModuleCMakeHelperCmakeIncludePath} ) #sets ${AFTR_PATH_TO_CMAKE_SCRIPTS}, cmake_vars, and compiler_flags



#This will use the AftrConfig.h to determine which libraries AftrBurner was build against and
#then include those libraries to link against which compiling this modules exe.
#Let's include the default libraries and header include paths for libraries that were not found
#using the CMake FIND_PACKAGE method used inside of the file aftrModuleCMakeHelper.cmake.
#These other libraries and includes are added inside of aftrModuleCommonProjectIncludesAndLibs.cmake
SET( aftrModuleCommonProjectIncludesAndLibs "${AFTR_PATH_TO_CMAKE_SCRIPTS}/aftrModuleCommonProjectIncludesAndLibs.cmake" )
include( ${aftrModuleCommonProjectIncludesAndLibs} )

print_aftr_cmake_vars()

#Set CMAKE_CXX_FLAGS if any default flags need modification
#SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings} ${cppFlags}" )
#SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${warnings}" )
MESSAGE( STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}" )
MESSAGE( STATUS "CMAKE_C_FLAGS  : ${CMAKE_C_FLAGS}" )

#Set any additional compiler flags -- by Default, usr/include/cmake/aftrModuleCMakeHelper.cmake sets AftrBurnerEngine-wide settings
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
   #The following two variables (warnings and cppFlags) are set inside aftrModuleCMakeHelper.cmake).
   #This is where GNU/Clang specific compiler flags specifically for building this module are located.
   ###SET( warnings "${warnings} -Wall -Wextra -myWarningFlags" )
   ###SET( cppFlags "${cppFlags} -fpermissive -std=c++11 -myGNUCompilerFlag" )    #GNU/Clang specific CPP compiler flags
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
   #The following two variables (warnings and cppFlags) are set inside aftrModuleCMakeHelper.cmake).
   #This is where MSVC specific compiler flags specifically for building the engine are located.
   ###SET( warnings "${warnings} /W4 /EHsc" )
   ###SET( warnings "${warnings} /wd4100 /myWarningFlag" ) #disable warning: 'identifier' : unreferenced formal parameter   
   ###SET( cppFlags "${cppFlags} /std:c++latest" ) #MSVC Specific CPP compiler flags, MP=Multi Processor compilations 
endif()


#By default, modules use find_package for SDL and boost, every platform includes the headers and libraries the same way,
#but for packages that don't have the Find<MyLib>.cmake in the /engine/src/cmake folder, we have to manually add
#them per platform. The TARGET_LINK_LIBRARIES and TARGET_INCLUDE_DIRECTORIES below perform this function:

#This section is already populated with default values from: ../../../include/cmake/aftrModuleCommonProjectIncludesAndLibs.cmake
IF( WIN32 ) #MSVC Compiler library paths
   TARGET_LINK_LIBRARIES( ${PROJECT_NAME} PRIVATE
                #optimized "${AFTR_USERLAND_LIB_PATH}/mySampleLibraryNeededInThisModule.lib"
                #    debug "${AFTR_USERLAND_LIB_PATH}/mySampleLibraryNeededInThisModule_debug.lib"
                #optimized "${CMAKE_SOURCE_DIR}/lib${AFTR_NBITS}/myLocalLib.lib" #Located in ../lib64/ or ../lib32/
                #    debug "${CMAKE_SOURCE_DIR}/lib${AFTR_NBITS}/myLocalLib.lib"
                #optimized
                            "${AFTR_USERLAND_LIB_PATH}/irrklang/lib/Winx64-visualStudio/irrKlang.lib"
                #   debug
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/debug/LowLevel_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/debug/LowLevelAABB_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/debug/LowLevelDynamics_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysX_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysXCharacterKinematic_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysXCommon_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysXCooking_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysXExtensions_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysXFoundation_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysXPvdSDK_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysXTask_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysXVehicle_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysXVehicle2_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/debug/PVDRuntime_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/debug/SceneQuery_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/debug/SimulationController_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/debug/SnippetRender_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/debug/SnippetUtils_static_64.lib"
                #optimized
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/release/LowLevel_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/release/LowLevelAABB_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/release/LowLevelDynamics_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysX_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysXCharacterKinematic_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysXCommon_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysXCooking_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysXExtensions_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysXFoundation_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysXPvdSDK_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysXTask_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysXVehicle_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysXVehicle2_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/release/PVDRuntime_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/release/SceneQuery_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/release/SimulationController_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/release/SnippetRender_static_64.lib"
                            "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/bin/win.x86_64.vc143.md/release/SnippetUtils_static_64.lib"
                #debug
                           # "${AFTR_USERLAND_LIB_PATH}/PhysX Visual Debugger 3/PVDNext.lib"
                         )
ENDIF()

#This section is already populated with default values from: ../../../include/cmake/aftrModuleCommonProjectIncludesAndLibs.cmake
IF( UNIX ) #Linux, Apple, *NIX platforms (see ../../../include/cmake/aftrModuleCommonProjectIncludesAndLibs.cmake for more details)
   TARGET_LINK_LIBRARIES( ${PROJECT_NAME} PRIVATE
                #optimized "${AFTR_USERLAND_LIB_PATH}/mySampleLibraryNeededInThisModule.lib"
                #    debug "${AFTR_USERLAND_LIB_PATH}/mySampleLibraryNeededInThisModule_debug.lib"
                #optimized "${CMAKE_SOURCE_DIR}/lib${AFTR_NBITS}/myLocalLib.lib" #Located in ../lib64/ or ../lib32/
                #    debug "${CMAKE_SOURCE_DIR}/lib${AFTR_NBITS}/myLocalLib.lib"
                #          "mySystemInstalledlib" #called libMySystemInstalledlib.a (perhaps in /usr/lib64/)
                        )
ENDIF()

#This section is already populated with default values from: ../../../include/cmake/aftrModuleCommonProjectIncludesAndLibs.cmake
#This can be made WIN32 or UNIX specific, depending on the platform, if desired.
TARGET_INCLUDE_DIRECTORIES( ${PROJECT_NAME} PRIVATE 
                           #"${AFTR_USR_INCLUDE_DIR}"
                           #"${CMAKE_SOURCE_DIR}/../my3rdPartyLib/include/"
                           "${AFTR_USERLAND_LIB_PATH}/irrklang/include/"
                           "${AFTR_USERLAND_LIB_PATH}/Physx/PhysX/physx/include"
                           "${AFTR_USERLAND_LIB_PATH}/include"
                          )



SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings} ${cppFlags}" )  #These two lines should be removed sln 3 aug 2022
SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${warnings}" )                  #These two lines should be removed sln 3 aug 2022
MESSAGE( STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}" )
MESSAGE( STATUS "CMAKE_C_FLAGS  : ${CMAKE_C_FLAGS}" )                          