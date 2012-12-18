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

  _countRef = new unsigned int[1];
  *_countRef = 1;
}

IsoTasty::Primitives::UnlinkedProgram::UnlinkedProgram(const UnlinkedProgram& b) :
  _program(b._program),
  _countRef(b._countRef) {

  // TODO: Atomic Increment
  (*this->_countRef)++;
}

IsoTasty::Primitives::UnlinkedProgram& IsoTasty::Primitives::UnlinkedProgram::operator=(const UnlinkedProgram& b) {
  return IsoTasty::Primitives::UnlinkedProgram(b);
}

IsoTasty::Primitives::UnlinkedProgram::~UnlinkedProgram() {
  // TODO: This needs to make use of a compare and exchange
  (*this->_countRef)--;
  if (*this->_countRef == 0) {
    if (!(this->_linked)) {
      glDeleteProgram(this->_program);
    }
    delete this->_countRef;
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