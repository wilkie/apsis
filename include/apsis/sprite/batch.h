#ifndef APSIS_SPRITE_BUFFER_H
#define APSIS_SPRITE_BUFFER_H

#include "apsis/sprite/sheet.h"

namespace Apsis {
  namespace Sprite {
    class Batch {
    public:
      /*
       *  Constructs a new Batch object using the given Sprite::Sheet.
       */
      static Apsis::Sprite::Batch& load(const Apsis::Sprite::Sheet& sheet,
                                        const Engine::Object& loader);
      static Apsis::Sprite::Batch& loaded(unsigned int id);

      /*
       *  Returns the unique identifier for this Batch.
       */
      unsigned int id() const;

      /*
       *  Returns the Sprite::Sheet that this Batch object is using.
       */
      const Apsis::Sprite::Sheet& sheet() const;

      /*
       *  Adds the given sprite to this Batch cropped and scaled
       *  using the given dimensions.
       */
      void add(unsigned int sprite_id,
               float x,
               float y,
               float width,
               float height,
               float src_x,
               float src_y,
               float src_width,
               float src_height);

      /*
       *  Adds the given sprite to this Batch cropped using the given
       *  dimensions.
       */
      void add(unsigned int sprite_id,
               float x,
               float y,
               float src_x,
               float src_y,
               float width,
               float height);

      /*
       *  Adds the given sprite to this Batch cropped using the given
       *  dimensions.
       */
      void add(unsigned int sprite_id,
               float x,
               float y,
               float width,
               float height);

      /*
       *  Adds the given sprite to this Batch without cropping.
       */
      void add(unsigned int sprite_id,
               float x,
               float y);

      /*
       *  Draws the Batch object with the given matrices.
       */
      void draw(const Primitives::Matrix& projection,
                const World::Camera& camera,
                const Primitives::Matrix& model) const;
    private:
      // Constructor
      Batch(const Apsis::Sprite::Sheet& sheet,
            const Engine::Object& loader);

      // Registry of batches.
      static std::vector<Apsis::Sprite::Batch*> _batches;

      // Id
      unsigned int _id;

      const Apsis::Sprite::Sheet& _sheet;

      // Graphics primitives for storing on gpu and drawing.
      mutable Primitives::VertexArray _vao;

      float* _vertices;
      unsigned int _vertexCount;
      unsigned int _vertexCapacity;
      mutable Primitives::VertexBuffer _vbo;

      unsigned short* _elements;
      unsigned int _elementCount;
      unsigned int _elementCapacity;
      mutable Primitives::VertexBuffer _ebo;

      mutable bool _dirty;
    };
  }
}

#endif
