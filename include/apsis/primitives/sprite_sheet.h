#ifndef APSIS_PRIMITIVES_SPRITE_SHEET_H
#define APSIS_PRIMITIVES_SPRITE_SHEET_H

#include <SDL/SDL_image.h>

#include <vector>

namespace Apsis {
  namespace Primitives {
    /*
     *  Describes an individual sprite on a Badger::SpriteSheet
     */
    struct Sprite {
      /*
       *  The x coordinate of the sprite on the sprite sheet in pixels.
       */
      unsigned int x;

      /*
       *  The y coordinate of the sprite on the sprite sheet in pixels.
       */
      unsigned int y;

      /*
       *  The width of the sprite on the sprite sheet in pixels.
       */
      unsigned int width;

      /*
       *  The height of the sprite on the sprite sheet in pixels.
       */
      unsigned int height;

      /*
       *  The x coordinate of the origin point of the sprite.
       */
      unsigned int center_x;

      /*
       *  The y coordinate of the origin point of the sprite.
       */
      unsigned int center_y;

      /*
       *  The name of the sprite. Maximum 64 characters.
       */
      char name[65];
    };

    /*
     *  Loads an image comprised of many sprites and handles calculations of
     *    texture coordinates.
     */
    class SpriteSheet {
      public:
        /*
         *  Constructs a Badger::SpriteSheet by loading the image located in
         *    the given filename. Will also parse a description file if one
         *    exists by looking for a file with the same name with a 'txt'
         *    extension.
         */
        SpriteSheet(const char* filename);

        /*
         *  Deconstructor
         */
        ~SpriteSheet();

        /*
         *  Returns: The texture id usable by the Renderer.
         */
        unsigned int texture();

        /*
         *  Fills the given coordinate array with the texture coordinates of the
         *    given sprite. The sprite index is assigned by the order in which
         *    sprites are listed in the description file.
         */
        void textureCoordinates(unsigned int index, double coords[4]);

        /*
         *  Fills the given coordinate array with the texture coordinates of the
         *    given sprite. The sprite is located by comparing the name given
         *    with the one described by the description file.
         *  Returns: true when the sprite is found, false otherwise.
         */
        bool textureCoordinates(const char* name, double coords[4]);

        /*
         *  Returns: the Sprite at the given index.
         */
        Sprite* sprite(unsigned int index);

        /*
         *  Returns: the Sprite by it's description.
         */
        Sprite* sprite(const char* name);
      
        /*
         *  Returns: -1 when no more sprites match, or the index
         *    of the next available sprite that matches the wildcard
         *    given by name. For instance wildcard = "down_*" will yield
         *    the next sprite after or including the index at last
         *    that begins with "down_". Use last = 0 to start, and
         *    then pass the return value + 1 in the next invocation.
         */
        int enumerateSprites(const char* wildcard, unsigned int last);

      private:

        // The OpenGL texture index.
        unsigned int _texture;

        // The width and height of the sprite texture.
        unsigned int _width;
        unsigned int _height;

        // An array of all sprites in the sprite sheet.
        std::vector<Sprite*> _sprites;

        // Yields the filename of the sprite description file from the given
        //   image filename.
        char* _determineStatSheetFilename(const char* filename);

        // Parse the description file from the given image filename.
        void  _loadStatSheet(const char* filename);
    };
  }
}

#endif
