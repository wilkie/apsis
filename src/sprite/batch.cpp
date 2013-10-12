#include "apsis/sprite/batch.h"

#include "apsis/primitive/fragment_shader.h"
#include "apsis/primitive/vertex_shader.h"

#include "apsis/primitive/unlinked_program.h"
#include "apsis/primitive/program.h"

#include <algorithm>
#include <string.h>
#include <stdlib.h>

std::vector<Apsis::Sprite::Batch*> Apsis::Sprite::Batch::_batches;

Apsis::Sprite::Batch& Apsis::Sprite::Batch::load(const Apsis::Sprite::Sheet& sheet,
                                                 const Engine::Object& loader) {
  _batches.push_back(new Apsis::Sprite::Batch(sheet, loader));
  return *_batches[_batches.size() - 1];
}

Apsis::Sprite::Batch& Apsis::Sprite::Batch::loaded(unsigned int index) {
  return *_batches[index];
}

Apsis::Sprite::Batch::Batch(const Apsis::Sprite::Sheet& sheet,
                            const Engine::Object& loader)
  : _sheet(sheet),
    _vertexCount(0),
    _vertices(NULL),
    _elementCount(0),
    _elements(NULL),
    _vbo(Primitive::VertexBuffer::Target::Data),
    _ebo(Primitive::VertexBuffer::Target::Elements),
    _dirty(false) {

  _id = _batches.size();

  printf("Building a new batch\n");

  const Registry::Program& program = loader.loadProgram("basic");

  _vao.useProgram(program);

  _vbo.defineInput("position", program.program(), 3, Primitive::VertexBuffer::Type::Float, false, 5, 0);
  _vbo.defineInput("texcoord", program.program(), 2, Primitive::VertexBuffer::Type::Float, false, 5, 3);

  _vao.bindTexture(0, _sheet.texture());
  _vao.uploadUniform("texture", 0);
}

unsigned int Apsis::Sprite::Batch::id() const {
  return _id;
}

const Apsis::Sprite::Sheet& Apsis::Sprite::Batch::sheet() const {
  return _sheet;
}

void Apsis::Sprite::Batch::add(unsigned int index,
                               float x,
                               float y,
                               float width,
                               float height,
                               float src_x,
                               float src_y,
                               float src_width,
                               float src_height) {
  if (width == 0 || height == 0) {
    return;
  }

  if (_vertexCount == 0) {
    _vertexCapacity = 5*4*10;
    _vertices = new float[_vertexCapacity];

    _elementCapacity = 6*10;
    _elements = new unsigned short[_elementCapacity];
  }

  if (_vertexCount == _vertexCapacity) {
    float* oldVertices = _vertices;
    _vertexCapacity *= 2;
    _vertices = new float[_vertexCapacity];
    memcpy(_vertices, oldVertices, _vertexCount * sizeof(float));
    delete [] oldVertices;

    unsigned short* oldElements = _elements;
    _elementCapacity *= 2;
    _elements = new unsigned short[_elementCapacity];
    memcpy(_elements, oldElements, _elementCount * sizeof(unsigned short));
    delete [] oldElements;
  }

  // Texture Coordinates
  float coords[4];
  coords[0] = (_sheet.left(index) + src_x)              / _sheet.texture().width();
  coords[1] = (_sheet.top(index)  + src_y)              / _sheet.texture().height();
  coords[2] = (_sheet.left(index) + src_x + src_width)  / _sheet.texture().width();
  coords[3] = (_sheet.top(index)  + src_y + src_height) / _sheet.texture().height();

  unsigned int vi = _vertexCount / 5;
  _vertices[vi * 5 + 0] = x;
  _vertices[vi * 5 + 1] = 0.0f;
  _vertices[vi * 5 + 2] = y;
  _vertices[vi * 5 + 3] = coords[0];
  _vertices[vi * 5 + 4] = coords[1];

  vi++;

  _vertices[vi * 5 + 0] = x + width;
  _vertices[vi * 5 + 1] = 0.0f;
  _vertices[vi * 5 + 2] = y;
  _vertices[vi * 5 + 3] = coords[2];
  _vertices[vi * 5 + 4] = coords[1];

  vi++;

  _vertices[vi * 5 + 0] = x + width;
  _vertices[vi * 5 + 1] = 0.0f;
  _vertices[vi * 5 + 2] = y + height;
  _vertices[vi * 5 + 3] = coords[2];
  _vertices[vi * 5 + 4] = coords[3];

  vi++;

  _vertices[vi * 5 + 0] = x;
  _vertices[vi * 5 + 1] = 0.0f;
  _vertices[vi * 5 + 2] = y + height;
  _vertices[vi * 5 + 3] = coords[0];
  _vertices[vi * 5 + 4] = coords[3];

  vi++;

  _vertexCount = vi * 5;

  unsigned int ei = _elementCount;

  _elements[ei] = vi-4; ei++;
  _elements[ei] = vi-3; ei++;
  _elements[ei] = vi-1; ei++;

  _elements[ei] = vi-3; ei++;
  _elements[ei] = vi-2; ei++;
  _elements[ei] = vi-1; ei++;

  _elementCount = ei;

  _dirty = true;
}
void Apsis::Sprite::Batch::add(unsigned int index,
                               float x,
                               float y,
                               float src_x,
                               float src_y,
                               float width,
                               float height) {
  add(index, x, y, width, height, src_x, src_y, width, height);
}

void Apsis::Sprite::Batch::add(unsigned int index,
                               float x,
                               float y,
                               float width,
                               float height) {
  add(index, x, y, width, height, 0.0f, 0.0f, width, height);
}

void Apsis::Sprite::Batch::add(unsigned int index,
                               float x,
                               float y) {
  add(index, x, y, _sheet.width(index), _sheet.height(index), 0.0f, 0.0f, _sheet.width(index), _sheet.height(index));
}

void Apsis::Sprite::Batch::draw(const Primitive::Matrix& projection,
                                const World::Camera& camera,
                                const Primitive::Matrix& model) const {
  _vao.bindProgram();

  if (_dirty) {
    // TODO: sync
    _vbo.transfer(_vertices, _vertexCount);
    _ebo.transfer(_elements, _elementCount);

    _vao.bind(_vbo);
    _vao.bind(_ebo);

    // TODO: Worry about texture slots
    _vao.uploadUniform("texture", 0);
    _dirty = false;
  }

  _vao.uploadUniform("projection", projection);
  _vao.uploadUniform("view", camera.view());
  _vao.uploadUniform("model", model);

  _vao.draw();
}
