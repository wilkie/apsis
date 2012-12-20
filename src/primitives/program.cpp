#include "iso-tasty/primitives/program.h"

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

IsoTasty::Primitives::Program::Program(unsigned int program,
                                       std::vector<VertexShader> vertexShaders,
                                       std::vector<FragmentShader> fragmentShaders) :
  _program(program),
  _vertexShaders(vertexShaders),
  _fragmentShaders(fragmentShaders) {
}

IsoTasty::Primitives::Program::~Program() {
  if (_counter.isAlone() && this->_program != 0) {
    glDeleteProgram(this->_program);
  }
}

void IsoTasty::Primitives::Program::defineInput(const char* name,
                                                VertexBuffer& buffer,
                                                unsigned int  numberOfComponents,
                                                Type          type,
                                                bool          normalized,
                                                unsigned int  stride,
                                                unsigned int  offset) const {
  glUseProgram(this->_program);
  GLint posAttrib = glGetAttribLocation(this->_program, name);
  glBindBuffer(GL_ARRAY_BUFFER, buffer.identifier());
  glEnableVertexAttribArray(posAttrib);

  GLuint gltype;
  GLuint glsize;

  if (type == Type::Float) {
    gltype = GL_FLOAT;
    glsize = sizeof(float);
  }

  glVertexAttribPointer(posAttrib, numberOfComponents, gltype, normalized,
                        stride * glsize, (GLvoid*)(offset * glsize));
}

void IsoTasty::Primitives::Program::uploadUniform(const char* name,
                                                  glm::mat4&  mat) const {
  GLint uniform = glGetUniformLocation(this->_program, name);
  glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(mat));
}

void IsoTasty::Primitives::Program::uploadUniform(const char* name,
                                                  int         value) const {
  GLint uniform = glGetUniformLocation(this->_program, name);
  glUniform1i(uniform, value);
}

unsigned int IsoTasty::Primitives::Program::identifier() const {
  return this->_program;
}