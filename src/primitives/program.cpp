#include "apsis/primitives/program.h"

#include "apsis/backend/sdl.h"

// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

Apsis::Primitives::Program::Program(unsigned int program,
                                    std::vector<VertexShader> vertexShaders,
                                    std::vector<FragmentShader> fragmentShaders) :
  _program(program),
  _vertexShaders(vertexShaders),
  _fragmentShaders(fragmentShaders) {
}

Apsis::Primitives::Program::~Program() {
  if (_counter.isAlone() && this->_program != 0) {
    glDeleteProgram(this->_program);
  }
}

void Apsis::Primitives::Program::uploadUniform(const char* name,
                                               glm::mat4&  mat) const {
  GLint uniform = glGetUniformLocation(this->_program, name);
  if (uniform < 0) {
    throw "Cannot upload uniform. Uniform not found.";
  }
  glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(mat));
}

void Apsis::Primitives::Program::uploadUniform(const char* name,
                                               int         value) const {
  GLint uniform = glGetUniformLocation(this->_program, name);
  if (uniform < 0) {
    throw "Cannot upload uniform. Uniform not found.";
  }
  glUniform1i(uniform, value);
}

unsigned int Apsis::Primitives::Program::identifier() const {
  return this->_program;
}
