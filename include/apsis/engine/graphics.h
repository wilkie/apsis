#ifndef APSIS_ENGINE_GRAPHICS_H
#define APSIS_ENGINE_GRAPHICS_H

#include "apsis/sprite/font.h"

#include <vector>

namespace Apsis {
  namespace Engine {
    /*
     *  This class represents the collective state of the renderer.
     */
    class Graphics {
    public:
      static Apsis::Engine::Graphics& basic();

      /*
       *  Draws the given string to the given 2d coordinates.
       */
      void draw(float x, float y, const char* string) const;

      /*
       *  Sets the current font.
       */
      void font(unsigned int font_id);

      /*
       *  Retrieves the current font.
       */
      const Apsis::Sprite::Font& font() const;

    private:
      // Constructor
      Graphics();

      // Keep global track of all Graphics Engines
      static std::vector<Apsis::Engine::Graphics*> _graphics_engines;

      // Keep track of Sprite::Font's and which are loaded...
      // Keep track of Sprite::Sheet's and which are loaded...
      mutable const Apsis::Sprite::Font* _default;
      const Apsis::Sprite::Font* _font;
    };
  }
}

#endif