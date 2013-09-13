#ifndef APSIS_MODEL_LIGHT_H
#define APSIS_MODEL_LIGHT_H

#include "apsis/primitives/math.h"

#include "glm/glm.hpp"

namespace Apsis {
  namespace Model {
    class Light {
    public:
      /*
       *  Constructs a new representation of a light.
       */
      Light(const Apsis::Primitives::Vector3& position,
            const Apsis::Primitives::Vector3& direction,
            const Apsis::Primitives::Vector3& color);

      /*
       *  Retrieve the position of the light.
       */
      const Apsis::Primitives::Vector3& position() const;

      /*
       *  Retrieve the direction of the light.
       */
      const Apsis::Primitives::Vector3& direction() const;

      /*
       *  Retrieve the color of the light.
       */
      const Apsis::Primitives::Vector3& color() const;

    private:
      const Apsis::Primitives::Vector3 _position;
      const Apsis::Primitives::Vector3 _direction;
      const Apsis::Primitives::Vector3 _color;
    };
  }
}

#endif
