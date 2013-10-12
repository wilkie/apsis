#ifndef APSIS_ENGINE_GRAPHICS_H
#define APSIS_ENGINE_GRAPHICS_H

#include "apsis/engine/object.h"

#include "apsis/sprite/font.h"
#include "apsis/sprite/sheet.h"

#include "apsis/settings/video.h"

#include <vector>

namespace Apsis {
  namespace Engine {
    /*
     *  This class represents the collective state of the renderer.
     */
    class Graphics {
    public:
      static Apsis::Engine::Graphics& basic(const Apsis::Settings::Video& settings,
                                            const Engine::Object& loader);

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
       *  Uses the given sprite sheet id.
       */
      void sheet(unsigned int sheet_id);

      /*
       *  Returns the current sprite sheet.
       */
      // TODO: Sprite sheets can be used simultaneously.
      const Apsis::Sprite::Sheet& sheet() const;

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
      const World::Camera& camera() const;

      /*
       *  Sets the current camera.
       */
      void camera(World::Camera& camera);

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

      /*
       *  Draws the given sprite by its sheet and sprite index.
       */
      void draw(float x,
                float y,
                unsigned int sheet,
                unsigned int sprite) const;

    private:
      // Constructor
      Graphics(const Apsis::Settings::Video& settings,
               const Engine::Object& loader);

      const Engine::Object& _loader;

      // Keep global track of all Graphics Engines
      static std::vector<Apsis::Engine::Graphics*> _graphics_engines;

      // Video Settings
      Apsis::Settings::Video _videoSettings;

      // Keep track of Sprite::Font's and which are loaded...
      // Keep track of Sprite::Sheet's and which are loaded...
      mutable const Apsis::Sprite::Font* _default;
      const Apsis::Sprite::Font* _font;

      const Apsis::Sprite::Sheet* _sheet;

      // Camera
      World::Camera _camera;

      // Matrices
      Primitives::Matrix _projection;
    };
  }
}

#endif