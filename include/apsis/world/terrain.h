#ifndef APSIS_WORLD_TERRAIN_H
#define APSIS_WORLD_TERRAIN_H

#include "apsis/model/material.h"
#include "apsis/model/light.h"

#include "apsis/world/camera.h"

#include "apsis/primitive/vertex_array.h"
#include "apsis/primitive/vertex_buffer.h"

#include "apsis/world/tile.h"

#include <vector>

namespace Apsis {
  namespace World {
    class Terrain {
    public:
      Terrain(unsigned int width,
              unsigned int height);
      ~Terrain();

      void draw(const Primitive::Matrix& projection,
                const World::Camera& camera) const;

      Apsis::World::Tile tileAt(unsigned int x, unsigned int z);

    private:
      unsigned int _width;
      unsigned int _height;

      Primitive::VertexArray _vao;

      float* _vertices;
      Primitive::VertexBuffer _vbo;

      unsigned short* _elements;
      Primitive::VertexBuffer _ebo;

      std::vector<Apsis::Model::Material> _materials;
      std::vector<Apsis::Model::Light> _lights;
    };
  }
}

#endif
