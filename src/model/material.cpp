#include "apsis/model/material.h"

Apsis::Model::Material::Material(float ambientIntensity,
                                 float diffuseIntensity,
                                 float specularIntensity,
                                 const Apsis::Primitive::Vector3& emission,
                                 float shininess) :
  _ambientIntensity(ambientIntensity),
  _diffuseIntensity(diffuseIntensity),
  _specularIntensity(specularIntensity),
  _emission(emission),
  _shininess(shininess) {
}

float Apsis::Model::Material::shininess() const {
  return _shininess;
}

const Apsis::Primitive::Vector3& Apsis::Model::Material::emission() const {
  return _emission;
}

float Apsis::Model::Material::diffuseIntensity() const {
  return _diffuseIntensity;
}

float Apsis::Model::Material::specularIntensity() const {
  return _specularIntensity;
}

float Apsis::Model::Material::ambientIntensity() const {
  return _ambientIntensity;
}