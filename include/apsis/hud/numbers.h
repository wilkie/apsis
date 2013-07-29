#ifndef APSIS_HUD_NUMBERS_H
#define APSIS_HUD_NUMBERS_H

#include <apsis/primitives/sprite_sheet.h>
#include <apsis/primitives/camera.h>
#include <apsis/primitives/vertex_array.h>
#include <apsis/primitives/vertex_buffer.h>
#include <apsis/geometry/rectangle.h>

namespace Apsis {
  namespace Hud {
    class Numbers {
      public:
        /*
         *  Constructs a Apsis::Sprite using the actor files in the
         *    given filepath. The sprite used is defined by the given
         *    Apsis::Primitives::SpriteSheet.
         */
        Numbers(Apsis::Primitives::SpriteSheet* spriteSheet,
                unsigned int startIndex,
                float x, float y);

        /*
         *  Return: the Apsis::Primitives::SpriteSheet for the Apsis::Sprite.
         */
        Apsis::Primitives::SpriteSheet* spriteSheet();

        /*
         *  Return: the Apsis::Geometry::Rectangle for the Apsis::Sprite.
         */
        Apsis::Geometry::Rectangle position();

        // Draws the actor
        void draw(glm::mat4& projection,
                  Primitives::Camera& camera);

        unsigned int value();
        void value(unsigned int updated);

      private:
        void _calculateSize();

        unsigned int _value;
        unsigned int _startIndex;

        // The set of sprites for the Numbers.
        Apsis::Primitives::SpriteSheet* _spriteSheet;

        // The current position of the Numbers in the world.
        Apsis::Geometry::Rectangle _position;

        Primitives::VertexArray _vao;

        float* _vertices;
        Primitives::VertexBuffer _vbo;

        unsigned int* _elements;
        Primitives::VertexBuffer _ebo;
    };
  }
}

#endif
