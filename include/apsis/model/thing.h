#ifndef APSIS_MODEL_THING_H
#define APSIS_MODEL_THING_H

#include "apsis/sync/reference_counter.h"

#include "apsis/primitives/camera.h"

#include "apsis/model/mesh.h"

namespace Apsis {
  namespace Model {
    class Thing {
    public:
      Thing(const char* path);
      ~Thing();

      void draw(glm::mat4& projection, Primitives::Camera& camera, glm::mat4& model);

    private:
      void _addMesh(const void* mesh);
      void _addMaterial(const void* mat);

      Sync::ReferenceCounter _counter;
      std::vector<Mesh> _meshes;
    };
  }
}

#endif