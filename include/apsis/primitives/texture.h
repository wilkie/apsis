#ifndef APSIS_PRIMITIVES_TEXTURE_H
#define APSIS_PRIMITIVES_TEXTURE_H

#include "apsis/sync/reference_counter.h"

#include <vector>

namespace Apsis {
  namespace Primitives {
    class Texture {
    public:
      /*
       *  Constructs a texture from the given file.
       */
      Texture(const char* path);
      ~Texture();

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