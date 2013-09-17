#ifndef APSIS_INTERFACE_VIEWPORT_H
#define APSIS_INTERFACE_VIEWPORT_H

#include "apsis/engine/scene.h"

#include "apsis/interface/window.h"

#include "apsis/world/object.h"

#include "apsis/primitives/camera.h"

#include "apsis/geometry/point3d.h"

// Does Viewport refer to Engine::System to get current Scene?
// That seems better (regardless of dependency cycle)

namespace Apsis {
  namespace Interface {
    class Viewport {
    public:
      /*
       *  Creates a Viewport that will display the given Scene.
       */
      Viewport(const Apsis::Engine::Scene& scene,
               float width,
               float height);
      /*
       *  Moves the position of the Camera.
       */
      void position(Apsis::Geometry::Point3d& point) const;
      void position(float x, float y, float z) const;

      /*
       *  Returns the position of the Camera.
       */
      Apsis::Geometry::Point3d position();

      /*
       *  Moves the target of the Camera.
       */
      void target(Apsis::Geometry::Point3d& point) const;
      void target(float x, float y, float z) const;

      /*
       *  Returns the target of the Camera.
       */
      Apsis::Geometry::Point3d target();

      /*
       *  Draws the viewport.
       */
      void draw(Apsis::Engine::Graphics& graphics) const;

      /*
       *  Returns the main Window element of this Viewport.
       */
      const Apsis::Interface::Window& window() const;

    private:
      static void _init(const Apsis::Geometry::Rectangle& position,
                        Apsis::World::Object& object);

      static void _draw(Apsis::Engine::Graphics& graphics,
                        const Apsis::Geometry::Rectangle& position,
                        const Apsis::World::Object& object);

      // Viewports have a Scene, a Camera, and a Window
      const Apsis::Engine::Scene& _scene;
      Apsis::Primitives::Camera _camera;
      Apsis::Interface::Window _window;
    };
  }
}

#endif