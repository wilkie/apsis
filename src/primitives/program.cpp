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

IsoTasty::Primitives::Program::Program(unsigned int program) {
  this->_program = program;

  _countRef = new unsigned int[1];
  *_countRef = 1;
}

IsoTasty::Primitives::Program::Program(const Program& b) :
  _program(b._program),
  _countRef(b._countRef) {

  // TODO: Atomic Increment
  (*this->_countRef)++;
}

IsoTasty::Primitives::Program& IsoTasty::Primitives::Program::operator=(const Program& b) {
  return IsoTasty::Primitives::Program(b);
}

IsoTasty::Primitives::Program::~Program() {
  // TODO: This needs to make use of a compare and exchange
  (*this->_countRef)--;
  if (*this->_countRef == 0) {
    glDeleteProgram(this->_program);
    delete this->_countRef;
  }
}

unsigned int IsoTasty::Primitives::Program::identifer() const {
  return this->_program;
}