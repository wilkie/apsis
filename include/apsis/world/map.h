#ifndef APSIS_WORLD_MAP_H
#define APSIS_WORLD_MAP_H

#include "apsis/renderer.h"
#include "apsis/world/tile.h"
#include "apsis/primitives/sprite_sheet.h"

#include "apsis/primitives/camera.h"
#include "apsis/primitives/vertex_array.h"
#include "apsis/primitives/vertex_buffer.h"

#include <glm/glm.hpp>

#include <vector>

namespace Apsis {
  namespace World {
    /*
     *  Describes a world made out of tiles drawn from a single SpriteSheet.
     */
    class Map {
    public:
      /*
       *  Constructs a Apsis::World::Map of the given width and height and
       *    drawn using the given Apsis::Primitives::SpriteSheet.
       */
      Map(unsigned int width,
          unsigned int height,
          Apsis::Primitives::SpriteSheet* spriteSheet);

      /*
       *  Queries the tile at the world coordinates (x,y)
       */
      Tile* tile(unsigned int x, unsigned int y);

      /*
       *  Returns the width of the map.
       */
      unsigned int width();

      /*
       *  Returns the height of the map
       */
      unsigned int height();

      /*
       *  Returns the Badger::SpriteSheet being used to draw the map.
       */
      Apsis::Primitives::SpriteSheet* spriteSheet();

      /*
       *  Renders the map.
       */
      void draw(glm::mat4& projection,
                Primitives::Camera& camera,
                glm::mat4& model);

    private:

      // Internal storage
      unsigned int _width;
      unsigned int _height;
      Apsis::Primitives::SpriteSheet* _spriteSheet;

      // The tiles that compose the map
      std::vector<Tile> _tiles;

      /*
        * Randomly assigns tiles from _tiles to be walls. Wall tiles use
        * sprites in range 16-19 and are impassable.
        */
      void _generateWalls();

      Primitives::VertexArray _vao;

      float* _vertices;
      Primitives::VertexBuffer _vbo;

      unsigned int* _elements;
      Primitives::VertexBuffer _ebo;
    };
  }
}

#endif
