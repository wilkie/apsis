#include "iso-tasty/model/material.h"

Apsis::Model::Material::Material(float ambientIntensity,
                                    float diffuseIntensity,
                                    float specularIntensity,
                                    float shininess) :
  _ambientIntensity(ambientIntensity),
  _diffuseIntensity(diffuseIntensity),
  _specularIntensity(specularIntensity),
  _shininess(shininess) {
}

float Apsis::Model::Material::shininess() {
  return _shininess;
}

float Apsis::Model::Material::diffuseIntensity() {
  return _diffuseIntensity;
}

float Apsis::Model::Material::specularIntensity() {
  return _specularIntensity;
}

float Apsis::Model::Material::ambientIntensity() {
  return _ambientIntensity;
}