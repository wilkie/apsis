#include "apsis/primitives/vertex_buffer.h"

#include "apsis/backend/sdl.h"

Apsis::Primitives::VertexBuffer::VertexBuffer() {
  glGenBuffers(1, &this->_vbo);
}

Apsis::Primitives::VertexBuffer::~VertexBuffer() {
  if (_counter.isAlone()) {
    //glDeleteBuffers(1, &this->_vbo);
  }
}

void Apsis::Primitives::VertexBuffer::transfer(const float* elements, unsigned int count) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_vbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * 4, elements, GL_STATIC_DRAW);

  _count = count;
}

void Apsis::Primitives::VertexBuffer::transfer(const unsigned int* elements, unsigned int count) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_vbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * 4, elements, GL_STATIC_DRAW);

  _count = count;
}

void Apsis::Primitives::VertexBuffer::transfer(const unsigned int* elements, unsigned int count, unsigned int at) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_vbo);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (GLintptr)(at * sizeof(GLint)), count, elements);
}

unsigned int Apsis::Primitives::VertexBuffer::count() const {
  return _count;
}

unsigned int Apsis::Primitives::VertexBuffer::identifier() const {
  return this->_vbo;
}

void Apsis::Primitives::VertexBuffer::bind() const {
}
