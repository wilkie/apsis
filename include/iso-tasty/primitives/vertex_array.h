#ifndef ISOTASTY_PRIMITIVES_VERTEX_ARRAY_H
#define ISOTASTY_PRIMITIVES_VERTEX_ARRAY_H

#include "iso-tasty/sync/reference_counter.h"

namespace IsoTasty {
  namespace Primitives {
    class VertexArray {
    public:
      /*
       *  Constructs a vertex array object.
       */
      VertexArray();
      ~VertexArray();

      /*
       *  Gives the identifier for this vertex array.
       */
      unsigned int identifer() const;

    private:
      unsigned int _vao;
      Sync::ReferenceCounter _counter;
    };
  }
}

#endif