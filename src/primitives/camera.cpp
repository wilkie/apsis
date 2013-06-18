#include "iso-tasty/primitives/camera.h"

// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>

Apsis::Primitives::Camera::Camera(glm::vec3& position,
                                  glm::vec3& target,
                                  glm::vec3& up)
  : _position(position), _target(target), _up(up) {
  _view = glm::lookAt(_position, target, up);
}

glm::mat4& Apsis::Primitives::Camera::view() {
  return _view;
}

glm::vec3& Apsis::Primitives::Camera::eye() {
  return _position;
}