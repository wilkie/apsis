#include "apsis/model/light.h"

Apsis::Model::Light::Light(const Apsis::Primitive::Vector3& position,
                           const Apsis::Primitive::Vector3& direction,
                           const Apsis::Primitive::Vector3& color) :
  _position(position),
  _direction(direction),
  _color(color) {
}

const Apsis::Primitive::Vector3& Apsis::Model::Light::position() const {
  return _position;
}

const Apsis::Primitive::Vector3& Apsis::Model::Light::direction() const {
  return _direction;
}

const Apsis::Primitive::Vector3& Apsis::Model::Light::color() const {
  return _color;
}
