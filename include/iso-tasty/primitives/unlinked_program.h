#ifndef APSIS_PRIMITIVES_UNLINKED_PROGRAM_H
#define APSIS_PRIMITIVES_UNLINKED_PROGRAM_H

#include "iso-tasty/sync/reference_counter.h"

#include "iso-tasty/primitives/vertex_shader.h"
#include "iso-tasty/primitives/fragment_shader.h"

#include "iso-tasty/primitives/program.h"

#include <vector>

namespace Apsis {
  namespace Primitives {
    using namespace std;

    class UnlinkedProgram {
    public:
      /*
       *  Constructs a program from a vertex shader.
       */
      UnlinkedProgram();
      ~UnlinkedProgram();

      /*
       *  Attach the given vertex shader.
       */
      void attach(VertexShader& vertexShader);

      /*
       *  Attach the given fragment shader.
       */
      void attach(FragmentShader& fragmentShader);

      /*
       *  Define the fragment shader's output location.
       */
      void defineFragmentOutput(const char* name);

      /*
       *  Links the program.
       */
      Program link();

    private:
      bool _linked;
      unsigned int _program;
      vector<VertexShader> _vertexShaders;
      vector<FragmentShader> _fragmentShaders;
      Sync::ReferenceCounter _counter;
    };
  }
}

#endif