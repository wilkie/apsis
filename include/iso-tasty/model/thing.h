#ifndef ISOTASTY_MODEL_THING_H
#define ISOTASTY_MODEL_THING_H

#include "iso-tasty/sync/reference_counter.h"

#include "iso-tasty/primitives/camera.h"

#include "iso-tasty/model/mesh.h"

namespace IsoTasty {
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