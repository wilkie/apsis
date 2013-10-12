#include "apsis/registry/program.h"

#include "apsis/primitive/unlinked_program.h"

#include <json/json.h>

#include <algorithm>
#include <fstream>

#include <sstream>

using namespace Apsis;

std::vector<std::string> Registry::Program::_ids;
std::vector<Apsis::Registry::Program*> Registry::Program::_programs;

#include <Windows.h>

const Registry::Program& Registry::Program::load(const char* path,
                                                 const Engine::Object& loader) {
  std::string str = std::string(path);

  std::vector<std::string>::iterator it = std::find(_ids.begin(), _ids.end(), str);
  if (it != _ids.end()) {
    // already exists
    return *_programs[std::distance(_ids.begin(), it)];
  }

  char foo[1024];
  sprintf(foo, "Loading program %s\n", path);
  OutputDebugStringA(foo);
  _programs.push_back(new Registry::Program(path, loader));
  _ids.push_back(str);
  return *_programs[_ids.size() - 1];
}

Registry::Program::Program(const char* path,
                           const Engine::Object& loader)
  : _program(NULL),
    _vertex_main(NULL),
    _fragment_main(NULL) {
  Json::Reader reader;
  Json::Value  value;

  std::ifstream file(path);
  reader.parse(file, value);
  file.close();

  if (value.isMember("type") &&
      value["type"].compare(Json::Value("program")) == 0) {
  }
  else {
    throw "Program file given is not of type 'program'.";
  }

  if (value.isMember("inherit")) {
    // TODO: inherit programs?? Does that make sense?
  }

  // name: %string%
  if (value.isMember("name")) {
    _name = value["name"].asCString();
  }

  // vertex: { "position": %string%, "normal": %string%, "texcoord": %string% }
  if (!value.isMember("vertex")) {
    throw "Program does not specify vertex computations.";
  }

  if (!value["vertex"].isObject()) {
    throw "Program's vertex description is not a hash of values.";
  }

  if (!value["vertex"].isMember("position")) {
    throw "Program does not specify a vertex position computation.";
  }

  if (!value["vertex"].isMember("normal")) {
    throw "Program does not specify a vertex normal computation.";
  }

  if (!value["vertex"].isMember("texcoord")) {
    throw "Program does not specify a vertex texcoord computation.";
  }

  if (!value["vertex"].isMember("final")) {
    throw "Program does not specify a final vertex computation.";
  }

  // fragment: { "final": %string% }
  if (!value.isMember("fragment")) {
    throw "Program does not specify fragment computations.";
  }

  if (!value["fragment"].isMember("final")) {
    throw "Program does not specify a final fragment computation.";
  }

  Json::Value& vertex = value["vertex"];

  // TODO: Load these in the constructor initializer as references
  _vertex_position = &loader.loadShader(vertex["position"].asCString());
  if (std::string(_vertex_position->output()).compare("vec3") != 0) {
    throw "Vertex shader position function does not return a vec3.";
  }
  _pullUniforms(*_vertex_position);
  _vertex_normal   = &loader.loadShader(vertex["normal"].asCString());
  if (std::string(_vertex_normal->output()).compare("vec3") != 0) {
    throw "Vertex shader normal function does not return a vec3.";
  }
  _pullUniforms(*_vertex_normal);
  _vertex_texcoord = &loader.loadShader(vertex["texcoord"].asCString());
  if (std::string(_vertex_texcoord->output()).compare("vec2") != 0) {
    throw "Vertex shader texcoord function does not return a vec2.";
  }
  _pullUniforms(*_vertex_texcoord);
  _vertex_final    = &loader.loadShader(vertex["final"].asCString());
  if (std::string(_vertex_final->output()).compare("vec4") != 0) {
    throw "Vertex shader function does not return a vec4.";
  }
  _pullUniforms(*_vertex_final);

  Json::Value& fragment = value["fragment"];

  _fragment_final  = &loader.loadShader(fragment["final"].asCString());
  if (std::string(_fragment_final->output()).compare("vec4") != 0) {
    throw "Fragment shader function does not return a vec4.";
  }
  _pullUniforms(*_fragment_final);

  _uniform_temp.clear();
}

const char* Registry::Program::name() const {
  return _name.c_str();
}

void Registry::Program::_generateCode() const {
  _generateVertexCode();
  _generateFragmentCode();
}

void Registry::Program::_generateVertexCode() const {
  std::ostringstream os;

  os << "#version 100" << std::endl << std::endl;

  const char* in_type = "attribute";
  const char* out_type = "varying";

  os << in_type << " vec3 normal;" << std::endl;
  os << in_type << " vec3 position;" << std::endl;
  os << in_type << " vec2 texcoord;" << std::endl;
  os << std::endl;
  os << out_type << " vec3 Normal;" << std::endl;
  os << out_type << " vec3 Position;" << std::endl;
  os << out_type << " vec2 Texcoord;" << std::endl;
  os << std::endl;

  os << "vec3 " << _vertex_position->name() << "(vec3,vec3,vec2);" << std::endl;
  os << "vec3 " << _vertex_normal->name()   << "(vec3,vec3,vec2);" << std::endl;
  os << "vec2 " << _vertex_texcoord->name() << "(vec3,vec3,vec2);" << std::endl;
  os << "vec4 " << _vertex_final->name()    << "(vec3,vec3,vec2);" << std::endl;
  os << std::endl;

  os << "void" << " " << "main" << "() {" << std::endl;

  os << "  Position = " << _vertex_position->name() << "(position, normal, texcoord);" << std::endl;
  os << "  Normal = " << _vertex_normal->name() << "(position, normal, texcoord);" << std::endl;
  os << "  Texcoord = " << _vertex_texcoord->name() << "(position, normal, texcoord);" << std::endl;
  os << std::endl;
  os << "  gl_Position = " << _vertex_final->name() << "(position, normal, texcoord);" << std::endl;

  os << "}" << std::endl;

  _vertex_code = os.str();
}

void Registry::Program::_generateFragmentCode() const {
  std::ostringstream os;

  os << "#version 100" << std::endl << std::endl;

  os << "#ifdef GL_ES" << std::endl;
  os << "precision highp float;" << std::endl;
  os << "#endif" << std::endl;
  os << std::endl;
  
  const char* in_type = "varying";
  const char* out_type = "";

  os << in_type << " vec3 Normal;" << std::endl;
  os << in_type << " vec3 Position;" << std::endl;
  os << in_type << " vec2 Texcoord;" << std::endl;
  os << std::endl;

  os << "vec4 " << _fragment_final->name() << "(vec3,vec3,vec2);" << std::endl;
  os << std::endl;

  os << "void" << " " << "main" << "() {" << std::endl;

  os << "  gl_FragColor = " << _fragment_final->name() << "(Position, Normal, Texcoord);" << std::endl;

  os << "}" << std::endl;

  _fragment_code = os.str();
}

const Primitive::Program& Registry::Program::program() const {
  if (_program == NULL) {
    if (_vertex_main == NULL) {
      if (_vertex_code.empty()) {
        _generateVertexCode();
      }
      _vertex_main = new Primitive::VertexShader(_vertex_code.c_str());
    }
    if (_fragment_main == NULL) {
      if (_fragment_code.empty()) {
        _generateFragmentCode();
      }
      _fragment_main = new Primitive::FragmentShader(_fragment_code.c_str());
    }

    Primitive::UnlinkedProgram& program = *(new Primitive::UnlinkedProgram());

    // Attach compiled shaders
    program.attach((*_vertex_position).vertexShader());
    program.attach((*_vertex_normal).vertexShader());
    program.attach((*_vertex_texcoord).vertexShader());
    program.attach((*_vertex_final).vertexShader());
    program.attach(*_vertex_main);

    program.attach((*_fragment_final).fragmentShader());
    program.attach(*_fragment_main);

    // Link
    _program = new Primitive::Program(program.link());
  }

  return *_program;
}

void Registry::Program::_pullUniforms(const Registry::Shader& shader) {
  for (unsigned int i = 0; i < shader.uniformCount(); i++) {
    if (_uniform_temp.count(shader.uniformName(i)) == 0) {
      _uniform_temp.insert(shader.uniformName(i));
      _uniform_names.push_back(shader.uniformName(i));
      _uniform_types.push_back(shader.uniformType(i));
    }
  }

  // Recursively pull in uniforms for functions called by this shader.
  for (unsigned int i = 0; i < shader.inputCount(); i++) {
    const Registry::Shader& sub_shader = shader.input(i);

    _pullUniforms(sub_shader);
  }
}

unsigned int Registry::Program::uniformCount() const {
  return _uniform_names.size();
}

const char* Registry::Program::uniformName(unsigned int index) const {
  return _uniform_names[index].c_str();
}

const char* Registry::Program::uniformType(unsigned int index) const {
  return _uniform_types[index].c_str();
}