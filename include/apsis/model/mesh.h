#ifndef APSIS_MODEL_MESH_H
#define APSIS_MODEL_MESH_H

#include "apsis/model/material.h"
#include "apsis/model/light.h"

#include "apsis/primitives/camera.h"
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
           std::vector<unsigned int>& elements);

      void draw(glm::mat4& projection, Primitives::Camera& camera, glm::mat4& model);

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