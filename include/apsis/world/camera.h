#ifndef APSIS_PRIMITIVE_CAMERA_H
#define APSIS_PRIMITIVE_CAMERA_H

#include <glm/glm.hpp>

#include "apsis/world/object.h"
#include "apsis/world/rule_set.h"

#include "apsis/primitive/math.h"

#include <vector>

namespace Apsis {
  namespace World {
    class Camera {
    public:
      /*
       *  Creates a camera at the given position looking at the given point and
       *    oriented the given direction with the given zoom.
       */
      Camera(const Primitive::Vector3& position,
             const Primitive::Vector3& target,
             const Primitive::Vector3& up,
             float zoom);

      /*
       *  Creates a camera for a 2d environment.
       */
      Camera(const Primitive::Vector2& position,
             float zoom);

      /*
       *  Return a reference to the view matrix.
       */
      const Primitive::Matrix& view() const;

      /*
       *  Return the position of the eye.
       */
      const Primitive::Vector3& eye() const;

      /*
       *  Returns the object.
       */
      World::Object& object();
      const World::Object& object() const;

    private:
      // Positioning
      Primitive::Vector3 _position;
      Primitive::Vector3 _target;
      Primitive::Vector3 _up;

      // View
      Primitive::Matrix _view;

      // Object
      World::Object _object;

      // Rules
      World::RuleSet _ruleSet;
    };
  }
}

#endif
