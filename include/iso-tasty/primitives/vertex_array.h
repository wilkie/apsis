#ifndef ISOTASTY_PRIMITIVES_VERTEX_ARRAY_H
#define ISOTASTY_PRIMITIVES_VERTEX_ARRAY_H

#include "iso-tasty/sync/reference_counter.h"

#include "iso-tasty/primitives/vertex_buffer.h"
#include "iso-tasty/primitives/program.h"
#include "iso-tasty/primitives/texture.h"

#include <vector>
#include <map>
#include <string>

#include <glm/glm.hpp>

namespace IsoTasty {
  namespace Primitives {
    class VertexArray {
    public:
      typedef enum {
        Float,
      } Type;

      /*
       *  Constructs a vertex array object.
       */
      VertexArray();
      ~VertexArray();

      /*
       *  Bind a vertex buffer to the elements array to describe what to draw.
       */
      void bindElements(VertexBuffer& buffer);

      /*
       *  Use the given program.
       */
      void useProgram(Program& program);

      /*
       *  Define a uniform.
       */
      int defineUniform(const char* name, Program& program);

      /*
       *  Transfer a matrix to a uniform.
       */
      void uploadUniform(const char* name, glm::mat4& matrix) const;
      void uploadUniform(int identifier, glm::mat4& matrix) const;

      /*
       *  Transfer an integer to a uniform.
       */
      void uploadUniform(const char* name, int value) const;
      void uploadUniform(int identifier, int value) const;

      /*
       *  Bind a texture.
       */
      void bindTexture(unsigned int slot, Texture& texture);

      void use();

      /*
       *  Render.
       */
      void draw();

      /*
       *  Gives the identifier for this vertex array.
       */
      unsigned int identifier() const;

    private:
      unsigned int _vao;

      Sync::ReferenceCounter _counter;

      std::vector<Program>            _programs;
      std::map<unsigned int, Texture> _textures;
      std::map<std::string, int>      _uniforms;

      std::vector<VertexBuffer> _elementBuffer;
    };
  }
}

#endif