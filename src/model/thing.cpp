#include "iso-tasty/model/thing.h"

#include "assimp/AssimpPCH.h"

#include <glm/glm.hpp>

#include <vector>

IsoTasty::Model::Thing::Thing(const char* path) {
  Assimp::Importer Importer;

  const aiScene* pScene = Importer.ReadFile(path, aiProcess_Triangulate |
                                                  aiProcess_GenSmoothNormals |
                                                  aiProcess_FlipUVs);

  if (pScene == NULL) {
    throw "Cannot load 3d object. No suitable importer.";
  }

  unsigned int numberOfMeshes    = pScene->mNumMeshes;
  unsigned int numberOfMaterials = pScene->mNumMaterials;

  for (unsigned int i = 0; i < numberOfMeshes; i++) {
    _addMesh(pScene->mMeshes[i]);
  }

  for (unsigned int i = 0; i < numberOfMaterials; i++) {
    _addMaterial(pScene->mMaterials[i]);
  }
}

IsoTasty::Model::Thing::~Thing() {
}

void IsoTasty::Model::Thing::_addMesh(const void* mesh_ptr) {
  const aiMesh* mesh = (const aiMesh*)mesh_ptr;

  unsigned int materialIndex = mesh->mMaterialIndex;
  unsigned int numberOfVertices = mesh->mNumVertices;
  unsigned int numberOfFaces = mesh->mNumFaces;

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> textureCoords;
  std::vector<unsigned int> elements;

  aiVector3D zero = aiVector3D(0.0f, 0.0f, 0.0f);

  for (unsigned int i = 0; i < numberOfVertices; i++) {
    const aiVector3D* pos     = &mesh->mVertices[i];
    const aiVector3D* normal  = &zero;
    const aiVector3D* texture = &zero;
    if (mesh->HasNormals()) {
      normal = &mesh->mNormals[i];
    }
    if (mesh->HasTextureCoords(0)) {
      texture = &mesh->mTextureCoords[0][i];
    }

    vertices.push_back(glm::vec3(pos->x, pos->y, pos->z));
    normals.push_back(glm::vec3(normal->x, normal->y, normal->z));
    textureCoords.push_back(glm::vec2(texture->x, texture->y));
  }

  for (unsigned int i = 0; i < numberOfFaces; i++) {
    const aiFace& face = mesh->mFaces[i];
    if (face.mNumIndices == 3) {
      elements.push_back(face.mIndices[0]);
      elements.push_back(face.mIndices[1]);
      elements.push_back(face.mIndices[2]);
    }
  }

  _meshes.push_back(Mesh(vertices, normals, textureCoords, elements));
}

void IsoTasty::Model::Thing::_addMaterial(const void* mat_ptr) {
  const aiMaterial* mat = (const aiMaterial*)mat_ptr;

  unsigned int textureCount = mat->GetTextureCount(aiTextureType_DIFFUSE);
  if (textureCount > 0) {
    aiString path;
    aiReturn ret = mat->GetTexture(aiTextureType_DIFFUSE, 0, &path,
                                   NULL, NULL, NULL, NULL, NULL);

    if (ret == AI_SUCCESS) {
      throw path.C_Str();
    }
  }
}

void IsoTasty::Model::Thing::draw(glm::mat4& projection, glm::mat4& view, glm::mat4& model) {
  for (unsigned int i = 0; i < _meshes.size(); i++) {
    _meshes[i].draw(projection, view, model);
  }
}