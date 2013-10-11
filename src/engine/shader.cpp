#include "apsis/engine/shader.h"

#include <algorithm>
#include <fstream>

using namespace Apsis;

std::vector<Engine::Shader*> Engine::Shader::_shader_engines;

Engine::Shader& Engine::Shader::basic(const Engine::Graphics& graphics) {
  Engine::Shader* se = new Engine::Shader(graphics);
  _shader_engines.push_back(se);
  return *se;
}

Engine::Shader::Shader(const Engine::Graphics& graphics)
  : _graphics(graphics),
    _videoSettings(graphics.settings()) {
}

const Registry::Shader& Engine::Shader::load(const char* json,
                                             const Engine::Object& loader) {
  return Registry::Shader::load(json, loader);
}