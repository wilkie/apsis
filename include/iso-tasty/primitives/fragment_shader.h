#ifndef ISOTASTY_PRIMITIVES_FRAGMENT_SHADER_H
#define ISOTASTY_PRIMITIVES_FRAGMENT_SHADER_H

#include "iso-tasty/sync/reference_counter.h"

namespace IsoTasty {
  namespace Primitives {
    class FragmentShader {
    public:
      /*
       *  Constructs a fragment shader object from the given source.
       */
      FragmentShader(const char* source);
      ~FragmentShader();

      /*
       *  Constructs a fragment shader object from the source contained within
       *    the given file.
       */
      static FragmentShader fromFile(const char* path);

      /*
       *  Gives the identifier for this fragment shader.
       */
      unsigned int identifier() const;

    private:
      unsigned int _fragmentShader;
      Sync::ReferenceCounter _counter;
    };
  }
}

#endif