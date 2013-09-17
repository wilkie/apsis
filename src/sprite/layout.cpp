#include "apsis/sprite/layout.h"

using namespace Apsis;

Sprite::Layout::Layout()
  : _vertexCount(0),
    _vertices(NULL),
    _elementCount(0),
    _elements(NULL),
    _dirty(false)  {
}

void Sprite::Layout::add(const char* text,
                         const Sprite::Font& font,
                         const Primitives::Vector4& color) {
  Node node;

  node.color = color;
  node.font  = &font;
  node.data  = text;

  _nodes.push_back(node);
  _dirty = true;
}