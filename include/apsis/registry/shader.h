#ifndef APSIS_REGISTRY_SHADER_H
#define APSIS_REGISTRY_SHADER_H

#include "apsis/engine/object.h"

#include "apsis/primitives/fragment_shader.h"
#include "apsis/primitives/vertex_shader.h"

#include <string>
#include <vector>

namespace Apsis {
  namespace Registry {
    class Shader {
    public:
      /*
       *  Lists the shader process types.
       */
      struct Process {
        enum Processes {
          /*
           *  This shader operates on vertices and positioning.
           */
          Vertex,

          /*
           *  This shader operates on final pixels and determines
           *  rendering output.
           */
          Fragment
        };
      };

      /*
       *  Loads the given JSON described shader into the system.
       */
      static const Registry::Shader& load(const char* path,
                                          const Engine::Object& loader);

      /*
       *  Returns the complete code for this shader function.
       */
      const char* code() const;

      /*
       *  Returns the name of this shader function.
       */
      const char* name() const;

      /*
       *  Returns the output type for this shader code.
       */
      const char* output() const;

      /*
       *  Returns the compiled fragment shader.
       */
      const Primitives::FragmentShader& fragmentShader() const;

      /*
       *  Returns the compiled vertex shader.
       */
      const Primitives::VertexShader& vertexShader() const;

      /*
       *  Returns the number of dependent inputs.
       */
      unsigned int inputCount() const;

      /*
       *  Returns the input through the given index.
       */
      const Registry::Shader& input(unsigned int index) const;
    private:
      // Constructors
      Shader(const char* path,
             const Engine::Object& loader);

      void _generateCode() const;

      // Keeps track of Scenes system-wide.
      static std::vector<std::string> _ids;
      static std::vector<Apsis::Registry::Shader*> _shaders;

      Process::Processes _process;
      std::string _name;
      std::string _output;

      std::vector<std::string> _inputs;
      std::vector<const Registry::Shader*> _input_shaders;

      std::vector<std::string> _uniform_names;
      std::vector<std::string> _uniform_types;

      mutable const Primitives::VertexShader* _vertex_shader;
      mutable const Primitives::FragmentShader* _fragment_shader;

      mutable std::string _code;
      std::string _implementation;
    };
  }
}

#endif