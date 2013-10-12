#ifndef APSIS_SPRITE_LAYOUT_H
#define APSIS_SPRITE_LAYOUT_H

#include "apsis/sprite/font.h"
#include "apsis/primitive/math.h"

#include <string>
#include <vector>

namespace Apsis {
  namespace Sprite {
    class Layout {
    public:
      /*
       *  Constructs a representation of a text layout.
       */
      Layout();

      /*
       *  Adds a chunk of text.
       */
      void add(const char* text,
               const Apsis::Sprite::Font& font,
               const Apsis::Primitive::Vector4& color);
    private:
      struct Node {
        std::string data;
        const Apsis::Sprite::Font* font;
        Apsis::Primitive::Vector4 color;
      };

      std::vector<Node> _nodes;

      // Graphics primitive for storing on gpu and drawing.
      mutable Primitive::VertexArray _vao;

      float* _vertices;
      unsigned int _vertexCount;
      unsigned int _vertexCapacity;
      mutable Primitive::VertexBuffer _vbo;

      unsigned int* _elements;
      unsigned int _elementCount;
      unsigned int _elementCapacity;
      mutable Primitive::VertexBuffer _ebo;

      mutable bool _dirty;
    };
  }
}

#endif