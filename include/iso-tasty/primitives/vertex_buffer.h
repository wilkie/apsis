#ifndef APSIS_PRIMITIVES_VERTEX_BUFFER_H
#define APSIS_PRIMITIVES_VERTEX_BUFFER_H

#include "iso-tasty/sync/reference_counter.h"

namespace Apsis {
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
      void transfer(const float* elements, unsigned int count);
      void transfer(const unsigned int* elements, unsigned int count);

      /*
       *  Returns the number of elements in the buffer.
       */
      unsigned int count() const;

      /*
       *  Gives the identifier for this vertex buffer.
       */
      unsigned int identifier() const;

    private:
      unsigned int _vbo;
      unsigned int _count;
      Sync::ReferenceCounter _counter;
    };
  }
}

#endif