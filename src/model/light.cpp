#include "apsis/model/light.h"

Apsis::Model::Light::Light(const Apsis::Primitives::Vector3& position,
                           const Apsis::Primitives::Vector3& direction,
                           const Apsis::Primitives::Vector3& color) :
  _position(position),
  _direction(direction),
  _color(color) {
}

const Apsis::Primitives::Vector3& Apsis::Model::Light::position() const {
  return _position;
}

const Apsis::Primitives::Vector3& Apsis::Model::Light::direction() const {
  return _direction;
}

const Apsis::Primitives::Vector3& Apsis::Model::Light::color() const {
  return _color;
}
