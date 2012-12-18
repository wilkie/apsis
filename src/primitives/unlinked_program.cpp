#include "iso-tasty/primitives/unlinked_program.h"

// Include GLEW
#include <GL/glew.h>

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

IsoTasty::Primitives::UnlinkedProgram::UnlinkedProgram() {
  this->_program = glCreateProgram();
}

IsoTasty::Primitives::UnlinkedProgram::~UnlinkedProgram() {
  if (!(this->_linked)) {
    glDeleteProgram(this->_program);
  }
}

void IsoTasty::Primitives::UnlinkedProgram::attach(VertexShader& vertexShader) const {
  glAttachShader(this->_program, vertexShader.identifer());
}

void IsoTasty::Primitives::UnlinkedProgram::attach(FragmentShader& fragmentShader) const {
  glAttachShader(this->_program, fragmentShader.identifer());
}

IsoTasty::Primitives::Program IsoTasty::Primitives::UnlinkedProgram::link() {
  _linked = true;
  return Program(_program);
}