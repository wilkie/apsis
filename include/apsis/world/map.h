#ifndef APSIS_WORLD_MAP_H
#define APSIS_WORLD_MAP_H

#include "apsis/world/tile.h"
#include "apsis/sprite/sheet.h"

#include "apsis/primitives/camera.h"
#include "apsis/primitives/vertex_array.h"
#include "apsis/primitives/vertex_buffer.h"

#include "apsis/sync/reference_counter.h"

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
       *    drawn using the given Apsis::Sprite::Sheet.
       */
      Map(unsigned int width,
          unsigned int height,
          float tileWidth,
          float tileHeight,
          Apsis::Sprite::Sheet* spriteSheet);

      /*
       *  Queries the tile at the world coordinates (x,y)
       */
      Tile* tile(unsigned int x, unsigned int y);

      /*
       *  Returns the width of the map.
       */
      unsigned int width();

      /*
       *  Returns the height of the map.
       */
      unsigned int height();

      /*
       *  Returns the width of a tile.
       */
      float tileWidth();

      /*
       *  Returns the height of a tile.
       */
      float tileHeight();

      /*
       *  Returns the Apsis::Sprite::Sheet being used to draw the map.
       */
      Apsis::Sprite::Sheet* spriteSheet();

      /*
       *  Renders the map.
       */
      void draw(const glm::mat4& projection,
                Primitives::Camera& camera,
                const glm::mat4& model);

    private:
      Sync::ReferenceCounter _counter;

      // Internal storage
      unsigned int _width;
      unsigned int _height;
      Apsis::Sprite::Sheet* _spriteSheet;

      // The tiles that compose the map
      std::vector<Tile> _tiles;

      float _tileWidth;
      float _tileHeight;

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
