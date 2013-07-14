#ifndef APSIS_MODEL_LIGHT_H
#define APSIS_MODEL_LIGHT_H

#include "glm/glm.hpp"

namespace Apsis {
  namespace Model {
    class Light {
    public:
      /*
       *  Constructs a new representation of a light.
       */
      Light(glm::vec3& position,
            glm::vec3& direction,
            glm::vec3& color);

      /*
       *  Retrieve the position of the light.
       */
      glm::vec3& position();

      /*
       *  Retrieve the direction of the light.
       */
      glm::vec3& direction();

      /*
       *  Retrieve the color of the light.
       */
      glm::vec3& color();

    private:
      glm::vec3 _position;
      glm::vec3 _direction;
      glm::vec3 _color;
    };
  }
}

#endif