#ifndef APSIS_MODEL_MATERIAL_H
#define APSIS_MODEL_MATERIAL_H

#include "glm/glm.hpp"

#include "apsis/primitives/math.h"

namespace Apsis {
  namespace Model {
    class Material {
    public:
      /*
       *  Constructs a new representation of a material.
       */
      Material(float ambientIntensity,
               float diffuseIntensity,
               float specularIntensity,
               const Apsis::Primitives::Vector3& emission,
               float shininess);

      /*
       *  Retrieve the shininess of the material. This affects how specular
       *    light will reflect.
       */
      float shininess() const;

      /*
       *  Retrieve the type of glow the material has. The glow is the color
       *    of light that emits from the material.
       */
      const Primitives::Vector3& emission() const;

      /*
       *  Retrieve the diffuse intensity of the material. This affects how
       *    light will shade the object.
       */
      float diffuseIntensity() const;

      /*
       *  Retrieve the specular intensity of the material. This affects how
       *    light will reflect off of the object.
       */
      float specularIntensity() const;

      /*
       *  Retrieve the ambient intensity of the material. This affects how
       *    ambient light (light that hits from all directions) will shade
       *    the object. Ambient light is an unnatural light.
       */
      float ambientIntensity() const;

    private:
      float _ambientIntensity;
      float _diffuseIntensity;
      float _specularIntensity;
      float _shininess;
      const Apsis::Primitives::Vector3 _emission;
    };
  }
}

#endif