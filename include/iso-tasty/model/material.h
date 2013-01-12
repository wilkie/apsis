#ifndef APSIS_MODEL_MATERIAL_H
#define APSIS_MODEL_MATERIAL_H

#include "glm/glm.hpp"

namespace Apsis {
  namespace Model {
    class Material {
    public:
      /*
       *  Constructs a new representation of a material.
       */
      Material(float     ambientIntensity,
               float     diffuseIntensity,
               float     specularIntensity,
               glm::vec3 emission,
               float     shininess);

      /*
       *  Retrieve the shininess of the material. This affects how specular
       *    light will reflect.
       */
      float shininess();

      /*
       *  Retrieve the type of glow the material has. The glow is the color
       *    of light that emits from the material.
       */
      glm::vec3& emission();

      /*
       *  Retrieve the diffuse intensity of the material. This affects how
       *    light will shade the object.
       */
      float diffuseIntensity();

      /*
       *  Retrieve the specular intensity of the material. This affects how
       *    light will reflect off of the object.
       */
      float specularIntensity();

      /*
       *  Retrieve the ambient intensity of the material. This affects how
       *    ambient light (light that hits from all directions) will shade
       *    the object. Ambient light is an unnatural light.
       */
      float ambientIntensity();

    private:
      float _ambientIntensity;
      float _diffuseIntensity;
      float _specularIntensity;
      float _shininess;
      glm::vec3 _emission;
    };
  }
}

#endif