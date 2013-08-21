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
      Tile();
      Tile(float cornerHeight[4]);

      // Properties
      float hover() const;
      float hover(float value);

      float cornerHeight(unsigned int index) const;
      float cornerHeight(unsigned int index, float value);

      float firstControl(unsigned int index) const;
      float firstControl(unsigned int index, float value);

      float secondControl(unsigned int index) const;
      float secondControl(unsigned int index, float value);

      bool  passable() const;
      bool  passable(bool value);

      unsigned int spriteIndex() const;
      unsigned int spriteIndex(unsigned int value);

      // Methods
      float minHeight() const;
      float minHeight(unsigned int index);
      float maxHeight() const;
      float maxHeight(unsigned int index);

    private:
      // Tile parameters
      float _height;

      float _hover;

      float _cornerHeight[4];
      float _firstControl[4];
      float _secondControl[4];

      bool         _passable;
      unsigned int _spriteIndex;
    };
  };
}

#endif