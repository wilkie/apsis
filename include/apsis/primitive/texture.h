#ifndef APSIS_PRIMITIVE_TEXTURE_H
#define APSIS_PRIMITIVE_TEXTURE_H

#include "apsis/sync/reference_counter.h"

#include <vector>

namespace Apsis {
  namespace Primitive {
    class Texture {
    public:

      // TODO: Texture formats.

      /*
       *  Constructs a texture from the given file.
       */
      Texture(const char* path);
      ~Texture();

      /*
       *  Constructs a texture with the given size.
       */
      Texture(unsigned int width, unsigned int height);

      /*
       *  Constructs a texture with the given size and data.
       */
      Texture(unsigned int width, unsigned int height, void* data);

      /*
       *  Copy the given pixel data to this texture.
       */
      void blit(unsigned int x, unsigned int y, void* data, unsigned int src_width, unsigned int src_height);

      /*
       *  Gives the width for this texture.
       */
      unsigned int width() const;

      /*
       *  Gives the height for this texture.
       */
      unsigned int height() const;

      /*
       *  Gives the identifier for this texture.
       */
      unsigned int identifier() const;

    private:
      unsigned int _texture;
      unsigned int _width;
      unsigned int _height;
      Sync::ReferenceCounter _counter;
    };
  }
}

#endif