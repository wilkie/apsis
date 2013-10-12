#ifndef APSIS_MODEL_MESH_H
#define APSIS_MODEL_MESH_H

#include "apsis/model/material.h"
#include "apsis/model/light.h"

#include "apsis/world/camera.h"

#include "apsis/primitive/math.h"
#include "apsis/primitive/vertex_array.h"
#include "apsis/primitive/vertex_buffer.h"

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

      void draw(const Primitive::Matrix& projection, const World::Camera& camera, const Primitive::Matrix& model) const;

    private:
      Primitive::VertexArray _vao;

      Primitive::VertexBuffer _vbo;
      Primitive::VertexBuffer _ebo;

      std::vector<Material> _materials;
      std::vector<Light> _lights;
    };
  }
}

#endif
