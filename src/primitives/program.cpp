#include "iso-tasty/primitives/program.h"

// Include GLEW
#include <GL/glew.h>

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

IsoTasty::Primitives::Program::Program(VertexShader& vertexShader) {
  this->_program = glCreateProgram();
  glAttachShader(this->_program, vertexShader.identifer());
}

IsoTasty::Primitives::Program::Program(VertexShader& vertexShader, FragmentShader& fragmentShader) {
  this->_program = glCreateProgram();
  glAttachShader(this->_program, vertexShader.identifer());
  glAttachShader(this->_program, fragmentShader.identifer());
}

IsoTasty::Primitives::Program::Program(FragmentShader& fragmentShader) {
  this->_program = glCreateProgram();
  glAttachShader(this->_program, fragmentShader.identifer());
}

IsoTasty::Primitives::Program::~Program() {
  glDeleteProgram(this->_program);
}

unsigned int IsoTasty::Primitives::Program::identifer() const {
  return this->_program;
}