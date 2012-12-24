#include "iso-tasty/primitives/unlinked_program.h"

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

IsoTasty::Primitives::UnlinkedProgram::UnlinkedProgram() :
  _linked(false) {
  this->_program = glCreateProgram();
}

IsoTasty::Primitives::UnlinkedProgram::~UnlinkedProgram() {
  if (_counter.isAlone()) {
    if (!(this->_linked)) {
      glDeleteProgram(this->_program);
    }
  }
}

void IsoTasty::Primitives::UnlinkedProgram::attach(VertexShader& vertexShader) {
  if (_linked) throw "Program already linked. Cannot attach vertex shader.";
  _vertexShaders.push_back(vertexShader);
  glAttachShader(this->_program, vertexShader.identifier());
}

void IsoTasty::Primitives::UnlinkedProgram::attach(FragmentShader& fragmentShader) {
  if (_linked) throw "Program already linked. Cannot attach fragment shader.";
  _fragmentShaders.push_back(fragmentShader);
  glAttachShader(this->_program, fragmentShader.identifier());
}

void IsoTasty::Primitives::UnlinkedProgram::defineFragmentOutput(const char* name) {
  glBindFragDataLocation(this->_program, 0, name);
}

IsoTasty::Primitives::Program IsoTasty::Primitives::UnlinkedProgram::link() {
  if (_linked) throw "Program already linked. Cannot link again.";
  glLinkProgram(_program);
  GLint status;
  glGetProgramiv(_program, GL_LINK_STATUS, &status);
  if (status != GL_TRUE) {
    throw "Cannot link program";
  }
  _linked = true;
  return Program(_program, _vertexShaders, _fragmentShaders);
}