#ifndef ISOTASTY_PRIMITIVES_VERTEX_SHADER_H
#define ISOTASTY_PRIMITIVES_VERTEX_SHADER_H

#include "iso-tasty/sync/reference_counter.h"

namespace IsoTasty {
  namespace Primitives {
    class VertexShader {
    public:
      /*
       *  Constructs a vertex shader object from the given source.
       */
      VertexShader(const char* source);
      ~VertexShader();

      /*
       *  Constructs a vertex shader object from the source contained within
       *    the given file.
       */
      static VertexShader fromFile(const char* path);

      /*
       *  Gives the identifier for this vertex shader.
       */
      unsigned int identifer() const;

    private:
      unsigned int _vertexShader;
      Sync::ReferenceCounter _counter;
    };
  }
}

#endif