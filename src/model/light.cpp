#include "apsis/model/light.h"

Apsis::Model::Light::Light(const glm::vec3& position,
                           const glm::vec3& direction,
                           const glm::vec3& color) :
  _position(position),
  _direction(direction),
  _color(color) {
}

glm::vec3& Apsis::Model::Light::position() {
  return _position;
}

glm::vec3& Apsis::Model::Light::direction() {
  return _direction;
}

glm::vec3& Apsis::Model::Light::color() {
  return _color;
}
