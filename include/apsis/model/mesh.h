#ifndef APSIS_MODEL_MESH_H
#define APSIS_MODEL_MESH_H

#include "apsis/model/material.h"
#include "apsis/model/light.h"

#include "apsis/world/camera.h"

#include "apsis/primitives/math.h"
#include "apsis/primitives/vertex_array.h"
#include "apsis/primitives/vertex_buffer.h"

#include <glm/glm.hpp>

#include <vector>

namespace Apsis {
  namespace Model {
    class Mesh {
    public:
      Mesh(std::vector<glm::vec3>& vertices,
           std::vector<glm::vec3>& normals,
           std::vector<glm::vec2>& textureCoords,
           std::vector<unsigned short>& elements);

      void draw(const Primitives::Matrix& projection, const World::Camera& camera, const Primitives::Matrix& model) const;

    private:
      Primitives::VertexArray _vao;

      Primitives::VertexBuffer _vbo;
      Primitives::VertexBuffer _ebo;

      std::vector<Material> _materials;
      std::vector<Light> _lights;
    };
  }
}

#endif
