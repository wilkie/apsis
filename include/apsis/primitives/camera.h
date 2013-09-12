#ifndef APSIS_PRIMITIVES_CAMERA_H
#define APSIS_PRIMITIVES_CAMERA_H

#include <glm/glm.hpp>

#include <vector>

namespace Apsis {
  namespace Primitives {
    class Camera {
    public:
      typedef const float(*Matrix)[4];
      /*
       *  Creates a camera at the given position looking at the given point and
       *    oriented the given direction with the given zoom.
       */
      Camera(const glm::vec3& position,
             const glm::vec3& target,
             const glm::vec3& up,
             float zoom);

      /*
       *  Creates a camera for a 2d environment.
       */
      Camera(const glm::vec2& position,
             float zoom);

      /*
       *  Return a reference to the view matrix.
       */
      Matrix view() const;

      /*
       *  Return the position of the eye.
       */
      const glm::vec3& eye() const;

    private:
      glm::vec3 _position;
      glm::vec3 _target;
      glm::vec3 _up;

      glm::mat4 _view;
    };
  }
}

#endif
