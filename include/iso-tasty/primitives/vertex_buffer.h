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
       *  Copies the given data to the buffer.
       */
      void transfer(float elements[], unsigned int count);
      void transfer(unsigned int elements[], unsigned int count);

      /*
       *  Gives the identifier for this vertex buffer.
       */
      unsigned int identifier() const;

    private:
      unsigned int _vbo;
      Sync::ReferenceCounter _counter;
    };
  }
}

#endif