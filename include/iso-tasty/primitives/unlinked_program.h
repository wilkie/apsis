#ifndef ISOTASTY_PRIMITIVES_UNLINKED_PROGRAM_H
#define ISOTASTY_PRIMITIVES_UNLINKED_PROGRAM_H

#include "iso-tasty/primitives/vertex_shader.h"
#include "iso-tasty/primitives/fragment_shader.h"

#include "iso-tasty/primitives/program.h"

namespace IsoTasty {
  namespace Primitives {
    class UnlinkedProgram {
    public:
      /*
       *  Constructs a program from a vertex shader.
       */
      UnlinkedProgram();
      UnlinkedProgram(const UnlinkedProgram&);
      UnlinkedProgram& operator= (const UnlinkedProgram&);
      ~UnlinkedProgram();

      /*
       *  Attach the given vertex shader.
       */
      void attach(VertexShader& vertexShader) const;

      /*
       *  Attach the given fragment shader.
       */
      void attach(FragmentShader& fragmentShader) const;

      /*
       *  Links the program.
       */
      Program link();

    private:
      bool _linked;
      unsigned int _program;
      unsigned int* _countRef;
    };
  }
}

#endif