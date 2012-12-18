#include "iso-tasty/primitives/vertex_buffer.h"

// Include GLEW
#include <GL/glew.h>

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

IsoTasty::Primitives::VertexBuffer::VertexBuffer() {
  glGenBuffers(1, &this->_vbo);
}

IsoTasty::Primitives::VertexBuffer::~VertexBuffer() {
  glDeleteBuffers(1, &this->_vbo);
}

unsigned int IsoTasty::Primitives::VertexBuffer::identifer() const {
  return this->_vbo;
}