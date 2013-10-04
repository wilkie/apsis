#ifndef APSIS_WORLD_BACKGROUND_H
#define APSIS_WORLD_BACKGROUND_H

#include "apsis/world/camera.h"

#include "apsis/primitives/texture.h"
#include "apsis/primitives/vertex_array.h"
#include "apsis/primitives/vertex_buffer.h"

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
       *    drawn using the given Apsis::Primitives::Texture.
       */
      Background(Apsis::Primitives::Texture* texture);

      /*
       *  Returns the width of the background.
       */
      unsigned int width() const;

      /*
       *  Returns the height of the background.
       */
      unsigned int height() const;

      /*
       *  Returns the Apsis::Primitives::Texture being used to draw the background.
       */
      const Apsis::Primitives::Texture& texture() const;

      /*
       *  Renders the background.
       */
      void draw(const Primitives::Matrix& projection,
                const World::Camera& camera) const;

    private:

      // Internal storage
      unsigned int _width;
      unsigned int _height;
      Apsis::Primitives::Texture* _texture;

      Primitives::VertexArray _vao;

      float* _vertices;
      Primitives::VertexBuffer _vbo;

      unsigned int* _elements;
      Primitives::VertexBuffer _ebo;
    };
  }
}

#endif
