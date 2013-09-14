#ifndef APSIS_ENGINE_GRAPHICS_H
#define APSIS_ENGINE_GRAPHICS_H

#include "apsis/sprite/font.h"
#include "apsis/settings/video.h"

#include <vector>

namespace Apsis {
  namespace Engine {
    /*
     *  This class represents the collective state of the renderer.
     */
    class Graphics {
    public:
      static Apsis::Engine::Graphics& basic(const Apsis::Settings::Video& settings);

      /*
       *  Retrieves the current video settings.
       */
      const Settings::Video& settings() const;

      /*
       *  Sets the current font.
       */
      void font(unsigned int font_id);

      /*
       *  Retrieves the current font.
       */
      const Apsis::Sprite::Font& font() const;

      /*
       *  Clears buffer.
       */
      void clear() const;

      /*
       *  Establish a perspective projection.
       */
      void perspective();

      /*
       *  Establish an orthographic projection.
       */
      void orthographic();

      /*
       *  Retrieves the current camera.
       */
      const Primitives::Camera& camera() const;

      /*
       *  Sets the current camera.
       */
      void camera(Primitives::Camera& camera);

      /*
       *  Returns the active projection matrix.
       */
      const Primitives::Matrix& projection() const;

      /*
       *  Returns the active view matrix.
       */
      const Primitives::Matrix& view() const;

      /*
       *  Draws the given string to the given 2d coordinates.
       */
      void draw(float x, float y, const char* string) const;

    private:
      // Constructor
      Graphics(const Apsis::Settings::Video& settings);

      // Keep global track of all Graphics Engines
      static std::vector<Apsis::Engine::Graphics*> _graphics_engines;

      // Video Settings
      Apsis::Settings::Video _videoSettings;

      // Keep track of Sprite::Font's and which are loaded...
      // Keep track of Sprite::Sheet's and which are loaded...
      mutable const Apsis::Sprite::Font* _default;
      const Apsis::Sprite::Font* _font;

      // Camera
      Primitives::Camera _camera;

      // Matrices
      Primitives::Matrix _projection;
    };
  }
}

#endif