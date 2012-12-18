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
      ~VertexBuffer();

      /*
       *  Gives the identifier for this vertex buffer.
       */
      unsigned int identifer() const;

    private:
      unsigned int _vbo;
    };
  }
}

#endif