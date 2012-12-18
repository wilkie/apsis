#ifndef ISOTASTY_PRIMITIVES_PROGRAM_H
#define ISOTASTY_PRIMITIVES_PROGRAM_H

#include "iso-tasty/primitives/vertex_shader.h"
#include "iso-tasty/primitives/fragment_shader.h"

namespace IsoTasty {
  namespace Primitives {
    class Program {
    public:
      /*
       *  Constructs a program from a vertex shader.
       */
      Program(VertexShader& vertexShader);

      /*
       *  Constructs a program from a fragment shader.
       */
      Program(FragmentShader& fragmentShader);

      /*
       *  Constructs a program from a vertex shader and a fragment shader.
       */
      Program(VertexShader& vertexShader, FragmentShader& fragmentShader);
      ~Program();

      /*
       *  Gives the identifier for this program.
       */
      unsigned int identifer() const;

    private:
      unsigned int _program;
    };
  }
}

#endif