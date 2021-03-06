#ifndef APSIS_PRIMITIVE_UNLINKED_PROGRAM_H
#define APSIS_PRIMITIVE_UNLINKED_PROGRAM_H

#include "apsis/sync/reference_counter.h"

#include "apsis/primitive/vertex_shader.h"
#include "apsis/primitive/fragment_shader.h"

#include "apsis/primitive/program.h"

#include <vector>

namespace Apsis {
  namespace Primitive {
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
      void attach(const VertexShader& vertexShader);

      /*
       *  Attach the given fragment shader.
       */
      void attach(const FragmentShader& fragmentShader);

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
      vector<const VertexShader*> _vertexShaders;
      vector<const FragmentShader*> _fragmentShaders;
      Sync::ReferenceCounter _counter;
    };
  }
}

#endif