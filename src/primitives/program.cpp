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

IsoTasty::Primitives::Program::Program(unsigned int program) :
  _program(program) {
}

IsoTasty::Primitives::Program::~Program() {
  if (_counter.isAlone()) {
    glDeleteProgram(this->_program);
  }
}

unsigned int IsoTasty::Primitives::Program::identifer() const {
  return this->_program;
}