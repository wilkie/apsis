#ifndef ISOTASTY_PRIMITIVES_VERTEX_ARRAY_H
#define ISOTASTY_PRIMITIVES_VERTEX_ARRAY_H

namespace IsoTasty {
  namespace Primitives {
    class VertexArray {
    public:
      /*
       *  Constructs a vertex array object.
       */
      VertexArray();
      VertexArray(const VertexArray&);
      VertexArray& operator= (const VertexArray&);
      ~VertexArray();

      /*
       *  Gives the identifier for this vertex array.
       */
      unsigned int identifer() const;

    private:
      unsigned int _vao;
      unsigned int* _countRef;
    };
  }
}

#endif