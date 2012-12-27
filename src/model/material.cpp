#include "iso-tasty/model/material.h"

IsoTasty::Model::Material::Material(float ambientIntensity,
                                    float diffuseIntensity,
                                    float specularIntensity,
                                    float shininess) :
  _ambientIntensity(ambientIntensity),
  _diffuseIntensity(diffuseIntensity),
  _specularIntensity(specularIntensity),
  _shininess(shininess) {
}

float IsoTasty::Model::Material::shininess() {
  return _shininess;
}

float IsoTasty::Model::Material::diffuseIntensity() {
  return _diffuseIntensity;
}

float IsoTasty::Model::Material::specularIntensity() {
  return _specularIntensity;
}

float IsoTasty::Model::Material::ambientIntensity() {
  return _ambientIntensity;
}