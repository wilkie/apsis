#ifndef ISOTASTY_PRIMITIVES_VERTEX_SHADER_H
#define ISOTASTY_PRIMITIVES_VERTEX_SHADER_H

namespace IsoTasty {
  namespace Primitives {
    class VertexShader {
    public:
      /*
       *  Constructs a vertex shader object from the given source.
       */
      VertexShader(const char* source);
      VertexShader(const VertexShader&);
      VertexShader& operator= (const VertexShader&);
      ~VertexShader();

      /*
       *  Constructs a vertex shader object from the source contained within
       *    the given file.
       */
      static VertexShader fromFile(const char* path);

      /*
       *  Gives the identifier for this vertex shader.
       */
      unsigned int identifer() const;

    private:
      unsigned int _vertexShader;
      unsigned int* _countRef;
    };
  }
}

#endif