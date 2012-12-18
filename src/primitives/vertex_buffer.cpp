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

  _countRef = new unsigned int[1];
  *_countRef = 1;
}

IsoTasty::Primitives::VertexBuffer::VertexBuffer(const VertexBuffer& b) :
  _vbo(b._vbo),
  _countRef(b._countRef) {

  // TODO: Atomic Increment
  (*this->_countRef)++;
}

IsoTasty::Primitives::VertexBuffer& IsoTasty::Primitives::VertexBuffer::operator=(const VertexBuffer& b) {
  return IsoTasty::Primitives::VertexBuffer(b);
}

IsoTasty::Primitives::VertexBuffer::~VertexBuffer() {
  // TODO: This needs to make use of a compare and exchange
  (*this->_countRef)--;
  if (*this->_countRef == 0) {
    glDeleteBuffers(1, &this->_vbo);
    delete this->_countRef;
  }
}

unsigned int IsoTasty::Primitives::VertexBuffer::identifer() const {
  return this->_vbo;
}