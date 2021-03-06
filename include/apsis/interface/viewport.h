#ifndef APSIS_INTERFACE_VIEWPORT_H
#define APSIS_INTERFACE_VIEWPORT_H

#include "apsis/engine/scene.h"

#include "apsis/interface/window.h"

#include "apsis/world/object.h"
#include "apsis/world/camera.h"

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
      Apsis::Interface::Window& window();
      const Apsis::Interface::Window& window() const;

    private:
      static void _init(const Apsis::Interface::Window& window,
                        Apsis::World::Object& object,
                        const Engine::Object& loader);

      static void _draw(Apsis::Engine::Graphics& graphics,
                        const Apsis::Interface::Window& window,
                        const Apsis::World::Object& object);

      static void _input(bool pressed,
                         const Apsis::Input::Binding&    binding,
                         const Apsis::Geometry::Point&   point,
                         const Apsis::Interface::Window& window,
                         Apsis::World::Object&           object);

      static void _motion(const Apsis::Geometry::Point& point,
                          const Apsis::Interface::Window& window,
                          Apsis::World::Object& object);

      static void _update(float elapsed,
                          Apsis::Geometry::Rectangle& position,
                          Apsis::World::Object& object);

      static void _enter(const Apsis::Geometry::Point& point,
                         const Apsis::Interface::Window& window,
                         Apsis::World::Object& object);

      static void _leave_(const Apsis::Geometry::Point& point,
                          const Apsis::Interface::Window& window,
                          Apsis::World::Object& object);

      // Viewports have a Scene, a Camera, and a Window
      const Apsis::Engine::Scene& _scene;
      Apsis::World::Camera _camera;
      Apsis::Interface::Window _window;
    };
  }
}

#endif