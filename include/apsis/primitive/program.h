#ifndef APSIS_PRIMITIVE_PROGRAM_H
#define APSIS_PRIMITIVE_PROGRAM_H

#include "apsis/sync/reference_counter.h"

#include "apsis/primitive/vertex_shader.h"
#include "apsis/primitive/fragment_shader.h"

#include <glm/glm.hpp>

#include <vector>

namespace Apsis {
  namespace Primitive {
    class Program {
    public:
      /*
       *  Links the given unlinked program.
       */
      Program(unsigned int program,
              std::vector<const VertexShader*> vertexShaders,
              std::vector<const FragmentShader*> fragmentShaders);
      ~Program();

      /*
       *  Gives the identifier for this program.
       */
      unsigned int identifier() const;

    private:
      unsigned int _program;
      std::vector<const VertexShader*> _vertexShaders;
      std::vector<const FragmentShader*> _fragmentShaders;
      Sync::ReferenceCounter _counter;
    };
  }
}

#endif
