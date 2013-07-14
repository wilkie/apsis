#ifndef APSIS_WORLD_MAP_H
#define APSIS_WORLD_MAP_H

#include "apsis/world/tile.h"
#include "apsis/primitives/sprite_sheet.h"

#include <vector>

namespace Apsis {
  namespace World {
    /*
     *  Describes a world made out of tiles drawn from a single SpriteSheet.
     */
    class Map {
      public:
        /*
         *  Constructs a Badger::Map of the given width and height and drawn
         *    using the given Badger::SpriteSheet
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
    };
  }
}

#endif
