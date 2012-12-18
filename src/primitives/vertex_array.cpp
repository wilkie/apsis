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

  _countRef = new unsigned int[1];
  *_countRef = 1;
}

IsoTasty::Primitives::VertexArray::VertexArray(const VertexArray& b) :
  _vao(b._vao),
  _countRef(b._countRef) {

  // TODO: Atomic Increment
  (*this->_countRef)++;
}

IsoTasty::Primitives::VertexArray& IsoTasty::Primitives::VertexArray::operator=(const VertexArray& b) {
  return IsoTasty::Primitives::VertexArray(b);
}

IsoTasty::Primitives::VertexArray::~VertexArray() {
  // TODO: This needs to make use of a compare and exchange
  (*this->_countRef)--;
  if (*this->_countRef == 0) {
    glDeleteVertexArrays(1, &this->_vao);
    delete this->_countRef;
  }
}

unsigned int IsoTasty::Primitives::VertexArray::identifer() const {
  return this->_vao;
}