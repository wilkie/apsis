#include "iso-tasty/model/material.h"

Apsis::Model::Material::Material(float ambientIntensity,
                                 float diffuseIntensity,
                                 float specularIntensity,
                                 glm::vec3 emission,
                                 float shininess) :
  _ambientIntensity(ambientIntensity),
  _diffuseIntensity(diffuseIntensity),
  _specularIntensity(specularIntensity),
  _emission(emission),
  _shininess(shininess) {
}

float Apsis::Model::Material::shininess() {
  return _shininess;
}

glm::vec3& Apsis::Model::Material::emission() {
  return _emission;
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