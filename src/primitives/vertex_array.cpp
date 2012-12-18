#include "iso-tasty/primitives/vertex_array.h"

// Include GLEW
#include <GL/glew.h>

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

IsoTasty::Primitives::VertexArray::VertexArray() {
  glGenVertexArrays(1, &this->_vao);
}

IsoTasty::Primitives::VertexArray::~VertexArray() {
  glDeleteVertexArrays(1, &this->_vao);
}

unsigned int IsoTasty::Primitives::VertexArray::identifer() const {
  return this->_vao;
}