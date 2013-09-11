#ifndef APSIS_ENGINE_GRAPHICS_H
#define APSIS_ENGINE_GRAPHICS_H

namespace Apsis {
  namespace Engine {
    /*
     *  This class represents the collective state of the renderer.
     */
    class Graphics {
    public:
      /*
       *  Draws the given string to the given 2d coordinates.
       */
      void draw(float x, float y, const char* string);

      /*
       *  Sets the current font.
       */
      void useFont(unsigned int font_id);

      /*
       *  Retrieves the current font.
       */
      //const Apsis::Drawing::Font& font();

    private:
      // Keep track of Drawing::Font's and which are loaded...
      // Keep track of Sprite::Sheet's and which are loaded...
    };
  }
}

#endif