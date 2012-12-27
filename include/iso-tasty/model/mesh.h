#ifndef ISOTASTY_MODEL_MESH_H
#define ISOTASTY_MODEL_MESH_H

#include "iso-tasty/model/material.h"

#include "iso-tasty/primitives/camera.h"
#include "iso-tasty/primitives/vertex_array.h"
#include "iso-tasty/primitives/vertex_buffer.h"

#include <glm/glm.hpp>

#include <vector>

namespace IsoTasty {
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
    };
  }
}

#endif