#ifndef APSIS_PRIMITIVE_VERTEX_ARRAY_H
#define APSIS_PRIMITIVE_VERTEX_ARRAY_H

#include "apsis/sync/reference_counter.h"

#include "apsis/primitive/math.h"

#include "apsis/registry/program.h"

#include "apsis/primitive/vertex_buffer.h"
#include "apsis/primitive/program.h"
#include "apsis/primitive/texture.h"

#include <vector>
#include <map>
#include <string>

#include <glm/glm.hpp>

namespace Apsis {
  namespace Primitive {
    class VertexArray {
    public:
      /*
       *  Constructs a vertex array object.
       */
      VertexArray();
      ~VertexArray();

      /*
       *  Bind a vertex buffer to this vertex array object.
       */
      void bind(VertexBuffer& buffer);

      /*
       *  Use the given program.
       */
      void useProgram(const Program& program);
      void useProgram(const Registry::Program& program);

      /*
       *  Define a uniform.
       */
      int defineUniform(const char* name);

      /*
       *  Transfer a matrix to a uniform.
       */
      void uploadUniform(const char* name, const Matrix& matrix) const;
      void uploadUniform(int identifier,   const Matrix& matrix) const;

      /*
       *  Transfer an integer to a uniform.
       */
      void uploadUniform(const char* name, int value) const;
      void uploadUniform(int identifier,   int value) const;

      /*
       *  Transfer a float to a uniform.
       */
      void uploadUniform(const char* name, float value) const;
      void uploadUniform(int identifier,   float value) const;

      /*
       *  Transfer a 3d vector to a uniform.
       */
      void uploadUniform(const char* name, const Vector3& value) const;
      void uploadUniform(int identifier,   const Vector3& value) const;

      /*
       *  Transfer a 4d vector to a uniform.
       */
      void uploadUniform(const char* name, const Vector4& value) const;
      void uploadUniform(int identifier,   const Vector4& value) const;

      /*
       *  Bind a known texture.
       */
      void bindTexture(unsigned int slot, const Texture& texture);

      void use();

      /*
       *  Render as triangles.
       */
      void draw() const;

      /*
       *  Render the range of the elements array as triangles.
       */
      void drawRange(unsigned int start, unsigned int count) const;

      /*
       *  Render as quads.
       */
      void drawQuads() const;

      /*
       *  Render the range of the elements array as quads.
       */
      void drawQuadsRange(unsigned int start, unsigned int count) const;

      void bindProgram() const;

      /*
       *  Gives the identifier for this vertex array.
       */
      unsigned int identifier() const;

    private:
      void _bindTextures() const;
      void _bind() const;

      unsigned int _vao;
      static unsigned int _current_vao;

      Sync::ReferenceCounter _counter;

      std::vector<const Program>      _programs;
      std::map<unsigned int, Texture> _textures;
      std::map<std::string, int>      _uniforms;

      std::vector<VertexBuffer> _elementBuffer;
      std::vector<VertexBuffer> _dataBuffer;
    };
  }
}

#endif
