#ifndef ISOTASTY_PRIMITIVES_VERTEX_BUFFER_H
#define ISOTASTY_PRIMITIVES_VERTEX_BUFFER_H

#include "iso-tasty/sync/reference_counter.h"

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
      Sync::ReferenceCounter _counter;
    };
  }
}

#endif