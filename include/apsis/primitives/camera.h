#ifndef APSIS_PRIMITIVES_CAMERA_H
#define APSIS_PRIMITIVES_CAMERA_H

#include <glm/glm.hpp>

#include "apsis/primitives/math.h"

#include <vector>

namespace Apsis {
  namespace Primitives {
    class Camera {
    public:
      /*
       *  Creates a camera at the given position looking at the given point and
       *    oriented the given direction with the given zoom.
       */
      Camera(const Vector3& position,
             const Vector3& target,
             const Vector3& up,
             float zoom);

      /*
       *  Creates a camera for a 2d environment.
       */
      Camera(const Vector2& position,
             float zoom);

      /*
       *  Return a reference to the view matrix.
       */
      const Matrix& view() const;

      /*
       *  Return the position of the eye.
       */
      const Vector3& eye() const;

    private:
      Vector3 _position;
      Vector3 _target;
      Vector3 _up;

      Matrix _view;
    };
  }
}

#endif
