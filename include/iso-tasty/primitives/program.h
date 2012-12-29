#ifndef APSIS_PRIMITIVES_PROGRAM_H
#define APSIS_PRIMITIVES_PROGRAM_H

#include "iso-tasty/sync/reference_counter.h"

#include "iso-tasty/primitives/vertex_buffer.h"

#include "iso-tasty/primitives/vertex_shader.h"
#include "iso-tasty/primitives/fragment_shader.h"

#include <glm/glm.hpp>

#include <vector>

namespace Apsis {
  namespace Primitives {
    class Program {
    public:
      typedef enum {
        Float,
      } Type;

      /*
       *  Links the given unlinked program.
       */
      Program(unsigned int program,
              std::vector<VertexShader> vertexShaders,
              std::vector<FragmentShader> fragmentShaders);
      ~Program();

      /*
       *  Defines the representation of the input attribute with the given
       *    name.
       */
      void defineInput(const char*   name,
                       VertexBuffer& buffer,
                       unsigned int  numberOfComponents,
                       Type          type,
                       bool          normalized,
                       unsigned int  stride,
                       unsigned int  offset) const;

      /*
       *  Uploads the given matrix to the given uniform variable.
       */
      void uploadUniform(const char* name,
                         glm::mat4&  mat) const;

      /*
       *  Uploads the given integer to the given uniform variable.
       */
      void uploadUniform(const char* name,
                         int         value) const;

      /*
       *  Gives the identifier for this program.
       */
      unsigned int identifier() const;

    private:
      unsigned int _program;
      std::vector<VertexShader> _vertexShaders;
      std::vector<FragmentShader> _fragmentShaders;
      Sync::ReferenceCounter _counter;
    };
  }
}

#endif