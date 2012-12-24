#ifndef ISOTASTY_MODEL_MESH_H
#define ISOTASTY_MODEL_MESH_H

#include "iso-tasty/sync/reference_counter.h"

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
      ~Mesh();

      void draw(glm::mat4& projection, glm::mat4& view, glm::mat4& model);

    private:
      Sync::ReferenceCounter _counter;

      Primitives::VertexArray _vao;

      Primitives::VertexBuffer _vbo;
      Primitives::VertexBuffer _ebo;
    };
  }
}

#endif