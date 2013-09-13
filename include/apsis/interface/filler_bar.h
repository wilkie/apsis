#ifndef APSIS_HUD_FILLER_BAR_H
#define APSIS_HUD_FILLER_BAR_H

#include <apsis/sprite/sheet.h>
#include <apsis/primitives/camera.h>
#include <apsis/primitives/vertex_array.h>
#include <apsis/primitives/vertex_buffer.h>
#include <apsis/geometry/rectangle.h>

namespace Apsis {
  namespace Hud {
    class FillerBar {
      public:
        /*
         *  Constructs a Apsis::Sprite using the actor files in the
         *    given filepath. The sprite used is defined by the given
         *    Apsis::Sprite::Sheet.
         */
        FillerBar(Apsis::Sprite::Sheet* spriteSheet,
                  unsigned int startIndex,
                  unsigned int stages,
                  unsigned int count,
                  float x, float y);

        /*
         *  Return: the Apsis::Sprite::Sheet for the Apsis::Sprite.
         */
        Apsis::Sprite::Sheet* spriteSheet();

        /*
         *  Return: the Apsis::Geometry::Rectangle for the Apsis::Sprite.
         */
        Apsis::Geometry::Rectangle position();

        // Draws the actor
        void draw(const Primitives::Matrix& projection,
                  const Primitives::Camera& camera);

        unsigned int value();
        void value(unsigned int updated);

      private:
        unsigned int _value;
        unsigned int _count;

        unsigned int _startIndex;
        unsigned int _stages;

        float _itemWidth;

        // The set of sprites for the Numbers.
        Apsis::Sprite::Sheet* _spriteSheet;

        // The current position of the Numbers in the world.
        Apsis::Geometry::Rectangle _position;
    };
  }
}

#endif
