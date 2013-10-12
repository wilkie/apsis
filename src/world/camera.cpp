#include "apsis/world/camera.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

Apsis::World::Camera::Camera(const Primitive::Vector3& position,
                             const Primitive::Vector3& target,
                             const Primitive::Vector3& up,
                             float zoom)
  : _position(position), _target(target), _up(up) {

  glm::mat4 view = glm::scale(glm::lookAt(glm::vec3(_position.value[0], _position.value[1], _position.value[2]),
                                          glm::vec3(_target.value[0], _target.value[1], _target.value[2]),
                                          glm::vec3(_up.value[0], _up.value[1], _up.value[2])),
                              glm::vec3(zoom, zoom, zoom));

  _view = *(Primitive::Matrix*)glm::value_ptr(view);
}

Apsis::World::Camera::Camera(const Primitive::Vector2& position,
                             float zoom) {
  glm::vec3 pos      = glm::vec3(position.x() * zoom, 1.0, position.y() * zoom);
  glm::vec3 target   = glm::vec3(position.x() * zoom, 0.0, position.y() * zoom);
  glm::vec3 up       = glm::normalize(glm::vec3(0.0, 0.0, -1.0));

  _position = *(Primitive::Vector3*)glm::value_ptr(pos);
  _target   = *(Primitive::Vector3*)glm::value_ptr(target);
  _up       = *(Primitive::Vector3*)glm::value_ptr(up);

  glm::mat4 view = glm::scale(glm::lookAt(pos, target, up),
                              glm::vec3(zoom, zoom, zoom));

  _view = *(Primitive::Matrix*)glm::value_ptr(view);
}

const Apsis::Primitive::Matrix& Apsis::World::Camera::view() const {
  return _view;
}

const Apsis::Primitive::Vector3& Apsis::World::Camera::eye() const {
  return _position;
}

Apsis::World::Object& Apsis::World::Camera::object() {
  return _object;
}

const Apsis::World::Object& Apsis::World::Camera::object() const {
  return _object;
}