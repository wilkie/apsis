#ifndef APSIS_PRIMITIVE_VERTEX_BUFFER_H
#define APSIS_PRIMITIVE_VERTEX_BUFFER_H

#include "apsis/sync/reference_counter.h"

#include "apsis/primitive/program.h"

#include <vector>

namespace Apsis {
  namespace Primitive {
    class VertexBuffer {
    public:
      struct Type {
        enum Types {
          Float
        };
      };

      struct Target {
        enum Targets {
          Data,
          Elements
        };
      };

      /*
       *  Constructs a vertex buffer object.
       */
      VertexBuffer(Target::Targets target);
      ~VertexBuffer();

      /*
       *  Returns the target.
       */
      Target::Targets target() const;

      /*
       *  Defines the representation of the input attribute with the given
       *    name used by the given Program.
       */
      void defineInput(const char*    name,
                       const Program& program,
                       unsigned int   numberOfComponents,
                       Type::Types    type,
                       bool           normalized,
                       unsigned int   stride,
                       unsigned int   offset);

      /*
       *  Copies the given data to the buffer.
       */
      void transfer(const float* elements, unsigned int count);
      void transfer(const unsigned short* elements, unsigned int count);

      /*
       *  Updates the given data to the buffer.
       */
      void transfer(const unsigned short* elements, unsigned int count, unsigned int at);

      /*
       *  Returns the number of elements in the buffer.
       */
      unsigned int count() const;

      /*
       *  Gives the identifier for this vertex buffer.
       */
      unsigned int identifier() const;

      /*
       *  Binds as data array.
       */
      void bind() const;

    private:
      struct AttributeInfo {
        unsigned int attribute;
        unsigned int numberOfComponents;
        Type::Types  type;
        bool         normalized;
        unsigned int stride;
        unsigned int offset;
      };

      // TODO: map with attribute name?
      std::vector<AttributeInfo> _attributes;

      unsigned int _vbo;
      unsigned int _count;
      Sync::ReferenceCounter _counter;

      Target::Targets _target;
    };
  }
}

#endif
