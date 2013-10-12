#ifndef APSIS_PRIMITIVE_VERTEX_SHADER_H
#define APSIS_PRIMITIVE_VERTEX_SHADER_H

#include "apsis/sync/reference_counter.h"

namespace Apsis {
  namespace Primitive {
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
      unsigned int identifier() const;

    private:
      unsigned int _vertexShader;
      Sync::ReferenceCounter _counter;
    };
  }
}

#endif