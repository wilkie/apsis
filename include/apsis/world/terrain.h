#ifndef APSIS_WORLD_TERRAIN_H
#define APSIS_WORLD_TERRAIN_H

#include "apsis/model/material.h"
#include "apsis/model/light.h"

#include "apsis/world/camera.h"

#include "apsis/primitives/vertex_array.h"
#include "apsis/primitives/vertex_buffer.h"

#include "apsis/world/tile.h"

#include <vector>

namespace Apsis {
  namespace World {
    class Terrain {
    public:
      Terrain(unsigned int width,
              unsigned int height);
      ~Terrain();

      void draw(const Primitives::Matrix& projection,
                const World::Camera& camera) const;

      Apsis::World::Tile tileAt(unsigned int x, unsigned int z);

    private:
      unsigned int _width;
      unsigned int _height;

      Primitives::VertexArray _vao;

      float* _vertices;
      Primitives::VertexBuffer _vbo;

      unsigned short* _elements;
      Primitives::VertexBuffer _ebo;

      std::vector<Apsis::Model::Material> _materials;
      std::vector<Apsis::Model::Light> _lights;
    };
  }
}

#endif
