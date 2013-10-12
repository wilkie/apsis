#ifndef APSIS_REGISTRY_PROGRAM_H
#define APSIS_REGISTRY_PROGRAM_H

#include "apsis/engine/object.h"

#include "apsis/registry/shader.h"

#include "apsis/primitives/program.h"
#include "apsis/primitives/fragment_shader.h"
#include "apsis/primitives/vertex_shader.h"

#include <string>
#include <set>
#include <vector>

namespace Apsis {
  namespace Registry {
    class Program {
    public:
      /*
       *  Loads the given JSON described shader program into the system.
       */
      static const Registry::Program& load(const char* path,
                                           const Engine::Object& loader);

      /*
       *  Returns the name of this shader program.
       */
      const char* name() const;

      /*
       *  Returns the compiled shader program.
       */
      const Primitives::Program& program() const;

      /*
       *  Returns the number of uniforms in the program.
       */
      unsigned int uniformCount() const;

      /*
       *  Returns the name of the uniform of the given index.
       */
      const char* uniformName(unsigned int index) const;

      /*
       *  Returns the type of the uniform of the given index.
       */
      const char* uniformType(unsigned int index) const;
    private:
      // Constructors
      Program(const char* path,
              const Engine::Object& loader);

      void _generateCode() const;
      void _generateVertexCode() const;
      void _generateFragmentCode() const;

      void _pullUniforms(const Registry::Shader& shader);

      // Keeps track of Scenes system-wide.
      static std::vector<std::string> _ids;
      static std::vector<Registry::Program*> _programs;

      std::string _name;

      std::set<std::string> _uniform_temp;
      std::vector<std::string> _uniform_names;
      std::vector<std::string> _uniform_types;

      const Registry::Shader* _vertex_position;
      const Registry::Shader* _vertex_normal;
      const Registry::Shader* _vertex_texcoord;
      const Registry::Shader* _vertex_final;

      const Registry::Shader* _fragment_final;

      mutable const Primitives::VertexShader*   _vertex_main;
      mutable const Primitives::FragmentShader* _fragment_main;

      mutable const Primitives::Program* _program;

      mutable std::string _vertex_code;
      mutable std::string _fragment_code;
    };
  }
}

#endif