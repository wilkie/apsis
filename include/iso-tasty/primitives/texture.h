#ifndef ISOTASTY_PRIMITIVES_TEXTURE_H
#define ISOTASTY_PRIMITIVES_TEXTURE_H

#include "iso-tasty/sync/reference_counter.h"

#include <vector>

namespace IsoTasty {
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