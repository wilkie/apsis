#include "iso-tasty/primitives/vertex_buffer.h"

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

IsoTasty::Primitives::VertexBuffer::VertexBuffer() {
  glGenBuffers(1, &this->_vbo);
}

IsoTasty::Primitives::VertexBuffer::~VertexBuffer() {
  if (_counter.isAlone()) {
    //glDeleteBuffers(1, &this->_vbo);
  }
}

void IsoTasty::Primitives::VertexBuffer::transfer(float elements[], unsigned int count) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_vbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * 4, elements, GL_STATIC_DRAW);
}

void IsoTasty::Primitives::VertexBuffer::transfer(unsigned int elements[], unsigned int count) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_vbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * 4, elements, GL_STATIC_DRAW);
}

unsigned int IsoTasty::Primitives::VertexBuffer::identifier() const {
  return this->_vbo;
}