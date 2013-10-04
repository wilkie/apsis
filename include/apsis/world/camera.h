#ifndef APSIS_PRIMITIVES_CAMERA_H
#define APSIS_PRIMITIVES_CAMERA_H

#include <glm/glm.hpp>

#include "apsis/primitives/math.h"

#include <vector>

namespace Apsis {
  namespace World {
    class Camera {
    public:
      /*
       *  Creates a camera at the given position looking at the given point and
       *    oriented the given direction with the given zoom.
       */
      Camera(const Primitives::Vector3& position,
             const Primitives::Vector3& target,
             const Primitives::Vector3& up,
             float zoom);

      /*
       *  Creates a camera for a 2d environment.
       */
      Camera(const Primitives::Vector2& position,
             float zoom);

      /*
       *  Return a reference to the view matrix.
       */
      const Primitives::Matrix& view() const;

      /*
       *  Return the position of the eye.
       */
      const Primitives::Vector3& eye() const;

    private:
      Primitives::Vector3 _position;
      Primitives::Vector3 _target;
      Primitives::Vector3 _up;

      Primitives::Matrix _view;
    };
  }
}

#endif
