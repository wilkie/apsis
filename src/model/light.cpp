#include "iso-tasty/model/light.h"

IsoTasty::Model::Light::Light(glm::vec3& position,
                              glm::vec3& direction,
                              glm::vec3& color) :
  _position(position),
  _direction(direction),
  _color(color) {
}

glm::vec3& IsoTasty::Model::Light::position() {
  return _position;
}

glm::vec3& IsoTasty::Model::Light::direction() {
  return _direction;
}

glm::vec3& IsoTasty::Model::Light::color() {
  return _color;
}