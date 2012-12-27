#ifndef ISOTASTY_MODEL_MATERIAL_H
#define ISOTASTY_MODEL_MATERIAL_H

namespace IsoTasty {
  namespace Model {
    class Material {
    public:
      /*
       *  Constructs a new representation of a material.
       */
      Material(float ambientIntensity,
               float diffuseIntensity,
               float specularIntensity,
               float shininess);

      /*
       *  Retrieve the shininess of the material. This affects how specular
       *    light will reflect.
       */
      float shininess();

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
    };
  }
}

#endif