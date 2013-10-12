#include "apsis/registry/shader.h"

#include "apsis/engine/log.h"

#include <json/json.h>

#include <algorithm>
#include <fstream>

#include <sstream>

using namespace Apsis;

std::vector<std::string> Registry::Shader::_ids;
std::vector<Apsis::Registry::Shader*> Registry::Shader::_shaders;

const Registry::Shader& Registry::Shader::load(const char* path,
                                               const Engine::Object& loader) {
  std::string str = std::string(path);

  std::vector<std::string>::iterator it = std::find(_ids.begin(), _ids.end(), str);
  if (it != _ids.end()) {
    // already exists
    return *_shaders[std::distance(_ids.begin(), it)];
  }

  Engine::Log::printf("Loading shader %s\n", path);

  _shaders.push_back(new Registry::Shader(path, loader));
  _ids.push_back(str);
  return *_shaders[_ids.size() - 1];
}

Registry::Shader::Shader(const char* path,
                         const Engine::Object& loader)
  : _vertex_shader(NULL),
    _fragment_shader(NULL) {
  Json::Reader reader;
  Json::Value  value;

  std::ifstream file(path);
  reader.parse(file, value);
  file.close();

  if (value.isMember("type") &&
      value["type"].compare(Json::Value("shader")) == 0) {
  }
  else {
    throw "Shader file given is not of type 'shader'.";
  }

  if (value.isMember("inherit")) {
    // TODO: inherit shaders?? Does that make sense?
  }

  // name: %string%
  if (value.isMember("name")) {
    _name = value["name"].asCString();
  }

  // process: {'vertex', 'fragment'}
  if (!value.isMember("process")) {
    throw "No process";
  }
  std::string process = value["process"].asCString();
  if (process.compare("vertex") == 0) {
    _process = Process::Vertex;
  }
  else if (process.compare("fragment") == 0) {
    _process = Process::Fragment;
  }
  else {
    throw "Shader description's process field is not valid.";
  }

  // inputs: array[{"name": %string%, "type": %string%}+]
  if (value.isMember("inputs")) {
    if (!value["inputs"].isArray()) {
      throw "Shader description's 'inputs' is not an array";
    }

    for (Json::Value::iterator it = value["inputs"].begin();
          it != value["inputs"].end();
          ++it) {
      if (!(*it)["name"].isString()) {
        throw "Shader description has an input that does not list a string for the name.";
      }
      const char* name = (*it)["name"].asCString();

      _inputs.push_back(name);
    }
  }

  // uniforms: array[{"name": %string%, "type": %string%}+]
  if (value.isMember("uniforms")) {
    if (!value["uniforms"].isArray()) {
      throw "Shader description's 'uniforms' is not an array";
    }

    for (Json::Value::iterator it = value["uniforms"].begin();
          it != value["uniforms"].end();
          ++it) {
      if (!(*it)["name"].isString()) {
        throw "Shader description has a uniform that does not list a string for the name.";
      }
      if (!(*it)["type"].isString()) {
        throw "Shader description has a uniform that does not list a string for the type.";
      }
      const char* name = (*it)["name"].asCString();
      const char* type = (*it)["type"].asCString();

      _uniform_names.push_back(name);
      _uniform_types.push_back(type);
    }
  }

  if (value.isMember("code")) {
    _implementation = value["code"].asCString();
  }

  if (value.isMember("output")) {
    _output = value["output"].asCString();
  }
  else {
    throw "Shader description does not list an output type.";
  }

  // Must load inputs
  for (unsigned int i = 0; i < _inputs.size(); i++) {
    _input_shaders.push_back(&loader.loadShader(_inputs[i].c_str()));
  }
}

const char* Registry::Shader::code() const {
  if (_code.empty()) {
    // Generate code
    _generateCode();
  }

  return _code.c_str();
}

const char* Registry::Shader::name() const {
  return _name.c_str();
}

const char* Registry::Shader::output() const {
  return _output.c_str();
}

void Registry::Shader::_generateCode() const {
  std::ostringstream os;

  os << "#version 100" << std::endl << std::endl;

  if (_process == Process::Fragment) {
    os << "#ifdef GL_ES" << std::endl;
    os << "precision highp float;" << std::endl;
    os << "#endif" << std::endl;
    os << std::endl;
  }
  else {
  }

  for (unsigned int i = 0; i < _uniform_names.size(); i++) {
    os << "uniform " << _uniform_types[i] << " " << _uniform_names[i] << ";" << std::endl;
  }

  if (_uniform_names.size() > 0) {
    os << std::endl;
  }

  os << _output << " " << _name << "(";

  for (unsigned int i = 0; i < _inputs.size(); i++) {
    // Get type from that input
    os << _input_shaders[i]->output() << " " << _input_shaders[i]->name();
    if (i < _inputs.size() - 1) {
      os << ", ";
    }
  }

  if (_inputs.size() == 0) {
    os << "vec3 position, vec3 normal, vec2 texcoord";
  }

  os << ") {" << std::endl;

  os << _implementation << std::endl;

  os << "}" << std::endl;

  _code = os.str();
}

const Primitive::FragmentShader& Registry::Shader::fragmentShader() const {
  if (_process != Process::Fragment) {
    throw "Not a fragment shader.";
  }

  if (_fragment_shader == NULL) {
    _fragment_shader = new Primitive::FragmentShader(this->code());
  }

  return *_fragment_shader;
}

const Primitive::VertexShader& Registry::Shader::vertexShader() const {
  if (_process != Process::Vertex) {
    throw "Not a fragment shader.";
  }

  if (_vertex_shader == NULL) {
    _vertex_shader = new Primitive::VertexShader(this->code());
  }

  return *_vertex_shader;
}

unsigned int Registry::Shader::inputCount() const {
  return _inputs.size();
}

const Registry::Shader& Registry::Shader::input(unsigned int index) const {
  return *_input_shaders[index];
}

unsigned int Registry::Shader::uniformCount() const {
  return _uniform_names.size();
}

const char* Registry::Shader::uniformName(unsigned int index) const {
  return _uniform_names[index].c_str();
}

const char* Registry::Shader::uniformType(unsigned int index) const {
  return _uniform_types[index].c_str();
}