#ifndef APSIS_WORLD_TILE_H
#define APSIS_WORLD_TILE_H

namespace Apsis {
  enum Corner {
    TOP_LEFT = 0,
    TOP_RIGHT,
    BOT_RIGHT,
    BOT_LEFT
  };

  namespace World {
    class Tile {
    public:
      Tile(float cornerHeight[4]);

      // Properties
      float hover();
      float hover(float value);

      float cornerHeight(unsigned int index);
      float cornerHeight(unsigned int index, float value);

      float firstControl(unsigned int index);
      float firstControl(unsigned int index, float value);

      float secondControl(unsigned int index);
      float secondControl(unsigned int index, float value);

      // Methods
      float minHeight();
      float maxHeight();
      float minHeight(unsigned int index);
      float maxHeight(unsigned int index);

    private:
      // Tile parameters
      float _height;
      float _hover;
      float _cornerHeight[4];
      float _firstControl[4];
      float _secondControl[4];
    };
  };
}

#endif