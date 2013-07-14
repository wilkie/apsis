#ifndef APSIS_WORLD_TERRAIN_H
#define APSIS_WORLD_TERRAIN_H

#include "apsis/model/material.h"
#include "apsis/model/light.h"

#include "apsis/primitives/camera.h"
#include "apsis/primitives/vertex_array.h"
#include "apsis/primitives/vertex_buffer.h"

#include "apsis/world/tile.h"

#include <glm/glm.hpp>

#include <vector>

namespace Apsis {
  namespace World {
    class Terrain {
    public:
      Terrain(unsigned int width,
              unsigned int height);
      ~Terrain();

      void draw(glm::mat4& projection,
                Primitives::Camera& camera,
                glm::mat4& model);

      Apsis::World::Tile tileAt(unsigned int x, unsigned int z);

    private:
      unsigned int _width;
      unsigned int _height;

      Primitives::VertexArray _vao;

      float* _vertices;
      Primitives::VertexBuffer _vbo;

      unsigned int* _elements;
      Primitives::VertexBuffer _ebo;

      std::vector<Apsis::Model::Material> _materials;
      std::vector<Apsis::Model::Light> _lights;
    };
  }
}

#endif