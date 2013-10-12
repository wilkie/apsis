#ifndef APSIS_PRIMITIVE_FRAGMENT_SHADER_H
#define APSIS_PRIMITIVE_FRAGMENT_SHADER_H

#include "apsis/sync/reference_counter.h"

namespace Apsis {
  namespace Primitive {
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