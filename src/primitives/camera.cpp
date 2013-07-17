#include "apsis/primitives/camera.h"

// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>

Apsis::Primitives::Camera::Camera(glm::vec3& position,
                                  glm::vec3& target,
                                  glm::vec3& up,
                                  float zoom)
  : _position(position), _target(target), _up(up) {
  _view = glm::scale(glm::lookAt(_position, _target, _up),
                     glm::vec3(zoom, zoom, zoom));
}

Apsis::Primitives::Camera::Camera(glm::vec2& position,
                                  float zoom) {
  _position = glm::vec3(position.x*zoom, 1.0, position.y*zoom);
  _target = glm::vec3(position.x*zoom, 0.0, position.y*zoom);
  _up     = glm::normalize(glm::vec3(0.0, 0.0, -1.0));
    
  _view = glm::scale(glm::lookAt(_position, _target, _up),
                     glm::vec3(zoom, zoom, zoom));
}

glm::mat4& Apsis::Primitives::Camera::view() {
  return _view;
}

glm::vec3& Apsis::Primitives::Camera::eye() {
  return _position;
}