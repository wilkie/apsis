#ifndef APSIS_SPRITE_LAYOUT_H
#define APSIS_SPRITE_LAYOUT_H

#include "apsis/sprite/font.h"
#include "apsis/primitives/math.h"

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
               const Apsis::Primitives::Vector4& color);
    private:
      struct Node {
        std::string data;
        const Apsis::Sprite::Font* font;
        Apsis::Primitives::Vector4 color;
      };

      std::vector<Node> _nodes;

      // Graphics primitives for storing on gpu and drawing.
      mutable Primitives::VertexArray _vao;

      float* _vertices;
      unsigned int _vertexCount;
      unsigned int _vertexCapacity;
      mutable Primitives::VertexBuffer _vbo;

      unsigned int* _elements;
      unsigned int _elementCount;
      unsigned int _elementCapacity;
      mutable Primitives::VertexBuffer _ebo;

      mutable bool _dirty;
    };
  }
}

#endif