#ifndef APSIS_WORLD_BACKGROUND_H
#define APSIS_WORLD_BACKGROUND_H

#include "apsis/world/camera.h"

#include "apsis/primitive/texture.h"
#include "apsis/primitive/vertex_array.h"
#include "apsis/primitive/vertex_buffer.h"

#include <glm/glm.hpp>

#include <vector>

namespace Apsis {
  namespace World {
    /*
     *  Describes a scrolling background.
     */
    class Background {
    public:
      /*
       *  Constructs a Apsis::World::Background of the given width and height and
       *    drawn using the given Apsis::Primitive::Texture.
       */
      Background(Apsis::Primitive::Texture* texture);

      /*
       *  Returns the width of the background.
       */
      unsigned int width() const;

      /*
       *  Returns the height of the background.
       */
      unsigned int height() const;

      /*
       *  Returns the Apsis::Primitive::Texture being used to draw the background.
       */
      const Apsis::Primitive::Texture& texture() const;

      /*
       *  Renders the background.
       */
      void draw(const Primitive::Matrix& projection,
                const World::Camera& camera) const;

    private:

      // Internal storage
      unsigned int _width;
      unsigned int _height;
      Apsis::Primitive::Texture* _texture;

      Primitive::VertexArray _vao;

      float* _vertices;
      Primitive::VertexBuffer _vbo;

      unsigned short* _elements;
      Primitive::VertexBuffer _ebo;
    };
  }
}

#endif
