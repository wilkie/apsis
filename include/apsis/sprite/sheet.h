#ifndef APSIS_SPRITE_SHEET_H
#define APSIS_SPRITE_SHEET_H

#include "apsis/engine/object.h"

#include "apsis/primitive/texture.h"

#include "apsis/primitive/vertex_array.h"
#include "apsis/primitive/vertex_buffer.h"

#include "apsis/world/camera.h"

#include <string>
#include <vector>

namespace Apsis {
  namespace Sprite {
    /*
     *  Loads an image comprised of many sprites and handles calculations of
     *    texture coordinates.
     */
    class Sheet {
    public:
      /*
       *  Constructs a sprite sheet by loading the image located in
       *    the given filename. Will also parse a description file if one
       *    exists by looking for a file with the same name with a 'txt'
       *    extension.
       */
      Sheet(const char* filename, const Engine::Object& loader);

      static const Sprite::Sheet& load(const char* filename,
                                       const Engine::Object& loader);

      static const Sprite::Sheet& loaded(unsigned int id);

      /*
       *  Returns: The Texture for this SpriteSheet.
       */
      const Primitive::Texture& texture() const;

      /*
       *  Fills the given coordinate array with the texture coordinates of the
       *    given sprite. The sprite index is assigned by the order in which
       *    sprites are listed in the description file.
       */
      void textureCoordinates(unsigned int index, float coords[4]) const;

      /*
       *  Fills the given coordinate array with the texture coordinates of the
       *    given sprite. The sprite is located by comparing the name given
       *    with the one described by the description file.
       *  Returns: true when the sprite is found, false otherwise.
       */
      bool textureCoordinates(const char* name, float coords[4]) const;

      /*
        *  Returns: the number of sprites contained in the sheet.
        */
      unsigned int count() const;

      /*
       *  Returns the x position within the sheet of the sprite at the given
       *  index.
       */
      float left(unsigned int index) const;

      /*
       *  Returns the y position within the sheet of the sprite at the given
       *  index.
       */
      float top(unsigned int index) const;

      /*
       *  Returns the width of the sprite at the given index.
       */
      float width(unsigned int index) const;

      /*
       *  Returns the height of the sprite at the given index.
       */
      float height(unsigned int index) const;

      /*
       *  Returns the id of this sprite sheet in the system.
       */
      unsigned int id() const;

      /*
       *  Returns: -1 when no more sprites match, or the index
       *    of the next available sprite that matches the wildcard
       *    given by name. For instance wildcard = "down_*" will yield
       *    the next sprite after or including the index at last
       *    that begins with "down_". Use last = 0 to start, and
       *    then pass the return value + 1 in the next invocation.
       */
      int enumerateSprites(const char* wildcard, unsigned int last) const;

      /*
       *  Draws the given sprite at the given location.
       */
      void draw(unsigned int              index,
                const Primitive::Matrix& projection,
                const World::Camera& camera,
                const Primitive::Matrix& model) const;
    private:
      // Registry of sprite sheets.
      static std::vector<std::string> _ids;
      static std::vector<Apsis::Sprite::Sheet*> _sheets;

      std::string _graphic_path;

      unsigned int _id;

      Sync::ReferenceCounter _counter;

      // The texture.
      Apsis::Primitive::Texture* _texture;

      // The width and height of the sprite texture.
      unsigned int _width;
      unsigned int _height;

      // Describes an individual sprite.
      struct _Sprite {
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

      // An array of all sprites in the sprite sheet.
      std::vector<_Sprite> _sprites;

      // Parse the description file from the given json filename.
      void  _loadStatSheet(const char* filename);

      // Graphics primitive for storing on gpu and drawing.
      Primitive::VertexArray _vao;
      float* _vertices;
      Primitive::VertexBuffer _vbo;
      unsigned short* _elements;
      Primitive::VertexBuffer _ebo;
    };
  }
}

#endif
