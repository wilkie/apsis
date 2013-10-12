#ifndef APSIS_ENGINE_SHADER_H
#define APSIS_ENGINE_SHADER_H

#include "apsis/engine/object.h"
#include "apsis/engine/graphics.h"

#include "apsis/registry/program.h"
#include "apsis/registry/shader.h"

#include "apsis/primitive/fragment_shader.h"
#include "apsis/primitive/vertex_shader.h"

#include <string>
#include <vector>

namespace Apsis {
  namespace Engine {
    class Shader {
    public:
      static Apsis::Engine::Shader& basic(const Engine::Graphics& graphics);

      /*
       *  Loads the given shader.
       */
      const Registry::Shader& load(const char* json,
                                   const Engine::Object& loader);

      /*
       *  Loads the given program.
       */
      const Registry::Program& loadProgram(const char* json,
                                           const Engine::Object& loader);

    private:
      Shader(const Engine::Graphics& graphics);

      // Keeps track of Scene engines system-wide.
      static std::vector<Apsis::Engine::Shader*> _shader_engines;

      // Keep a graphics engine
      const Engine::Graphics& _graphics;

      // Keep track of video settings
      const Settings::Video& _videoSettings;

      std::vector<std::string> _paths;

      std::vector<std::string> _input_names;
      std::vector<std::string> _input_types;
    };
  }
}

#endif