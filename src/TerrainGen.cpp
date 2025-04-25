
#include "TerrainGen.h"
#include "Vector.h"
#include "VectorFwd.h"
#include "WO.h"
#include "PxPhysicsAPI.h"
#include "Model.h"
#include "WorldList.h"
#include "WOPhysXTriangularMesh.h"

using namespace Aftr;

TerrainGen::TerrainGen() {

    top = 0.0f;
    bottom = 0.0f;

    left = 0.0f;
    right = 0.0f;
    
    vert = 0;
    horz = 0;

}

void TerrainGen::createGrid(PxPhysics* p, PxScene* scene, Vector scale, std::string path) {

  std::vector grid = ManagerGDAL::parseGDALTifAssumingGeodesicSpacing((ManagerEnvironmentConfiguration::getLMM() + "/models/Moon_Plane/scene.gltf").c_str());

  top = grid[0][0].x;
  bottom = grid[1][0].x;
  vert = top - bottom;

  left = grid[0][0].y;
  right = grid[0][1].y;
  horz = right - left;

  Vector temp((top + bottom) / 2, (left + right) / 2, 0);
      offset = temp;
  centerOfWorld = offset.toVecS().toECEFfromWGS84();
  gravityDirection = centerOfWorld;
  gravityDirection.normalize();

  scale = Vector(1.0f, 1.0f, 1.0f);
  upperLeft = Vector(top, left, 0);
  lowerRight = Vector(bottom, right, 0);

  WOGridECEFElevationPhysx* gridEleva = WOGridECEFElevationPhysx::New(p,scene,upperLeft,lowerRight,0,offset, scale ,path, grid);

  gridEleva->setLabel("grid");

  ManagerGLView::getGLView()->getWorldContainer()->push_back(gridEleva);
  gridEleva->upon_async_model_loaded([gridEleva]()
      {
          std::string texPath = ManagerEnvironmentConfiguration::getVariableValue("texpath");
          std::string texture = texPath + ManagerEnvironmentConfiguration::getVariableValue("texture");
          auto tex = ManagerTex::loadTexAsync(ManagerEnvironmentConfiguration::getLMM() + texture);
          for (size_t i = 0; i < gridEleva->getModel()->getModelDataShared()->getModelMeshes().size(); i++)
          {

              ModelMesh* mesh = gridEleva->getModel()->getModelDataShared()->getModelMeshes().at(i);
              ModelMeshSkin skin(*ManagerTex::loadTexAsync(texture));
              skin.setMeshShadingType(MESH_SHADING_TYPE::mstSMOOTH);
              gridEleva->getModel() -> getModelDataShared()->getModelMeshes().at(i)->getSkins().at(0) = std::move(skin);
              gridEleva->getModel()->isUsingBlending(false);

          }
      }
  );

}

WOGridECEFElevationPhysx::WOGridECEFElevationPhysx() : IFace(this) {

    WO::initInstanceData();

}

WOGridECEFElevationPhysx* WOGridECEFElevationPhysx::New(PxPhysics* p, PxScene* scene, Vector upperLeft, Vector lowerRight, int granular, Vector offset, Vector scale, std::string& path, std::vector<std::vector<VectorD>>& grid) {

    WOGridECEFElevationPhysx* gridEl = new WOGridECEFElevationPhysx();
    gridEl->onCreate(p, scene, upperLeft, lowerRight, granular, offset, scale, path, grid);
    return gridEl;

}

void WOGridECEFElevationPhysx::onCreate(PxPhysics* p, PxScene* scene, Vector upperLeft, Vector lowerRight, int granular, Vector offset, Vector &scale, std::string path, std::vector<std::vector<VectorD>>& grid) {

    trimesh = Aftr::WOPhysXTriangularMesh::New(path,scale,MESH_SHADING_TYPE::mstAUTO,p,scene);

    size_t size = grid.size() * grid[0].size();
    PxHeightFieldSample* samples = new PxHeightFieldSample[size];

    int counter = 0;
    for (int i = 0; i < grid.size();i++) {
        for (int j = 0; j < grid[i].size(); j++) {

            samples[counter].height = grid[i][j].z;
            samples[counter].clearTessFlag();
            counter++;

        }
    }

    PxHeightFieldDesc hfDesc;
    hfDesc.format = PxHeightFieldFormat::eS16_TM;
    hfDesc.nbColumns = grid[0].size();
    hfDesc.nbRows = grid.size();
    hfDesc.samples.data = samples;
    hfDesc.samples.stride = sizeof(PxHeightFieldSample);

    PxHeightField* aHeightField = PxCreateHeightField(hfDesc, p->getPhysicsInsertionCallback());

    float rowScale = -(grid.begin()->at(0).x - grid.rbegin()->at(0).x) / grid[0].size();
    float colScale = -(grid[0].begin()->y - grid[0].rbegin()->y) / grid.size();
    float heightScale = 1;

    PxHeightFieldGeometry hfGeom(aHeightField, PxMeshGeometryFlags(), heightScale, rowScale, colScale);

    Aftr::Mat4 mat;
    mat = mat.rotate(Vector(1, 0, 0), Aftr::PI / 2);
    mat = mat.rotate(Vector(0, 0, 1), Aftr::PI / 2);
    PxMat44 m;
    m.column0.x = mat.getX().x;
    m.column0.y = mat.getX().y;
    m.column0.z = mat.getX().z;

    m.column1.x = mat.getY().x;
    m.column1.y = mat.getY().y;
    m.column1.z = mat.getY().z;

    m.column2.x = mat.getZ().x;
    m.column2.y = mat.getZ().y;
    m.column2.z = mat.getZ().z;

    PxTransform t2(m);



}

