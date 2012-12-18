#ifndef ISOTASTY_PRIMITIVES_VERTEX_BUFFER_H
#define ISOTASTY_PRIMITIVES_VERTEX_BUFFER_H

namespace IsoTasty {
  namespace Primitives {
    class VertexBuffer {
    public:
      /*
       *  Constructs a vertex buffer object.
       */
      VertexBuffer();
      VertexBuffer(const VertexBuffer&);
      VertexBuffer& operator= (const VertexBuffer&);
      ~VertexBuffer();

      /*
       *  Gives the identifier for this vertex buffer.
       */
      unsigned int identifer() const;

    private:
      unsigned int _vbo;
      unsigned int* _countRef;
    };
  }
}

#endif