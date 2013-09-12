#include "apsis/primitives/camera.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

Apsis::Primitives::Camera::Camera(const glm::vec3& position,
                                  const glm::vec3& target,
                                  const glm::vec3& up,
                                  float zoom)
  : _position(position), _target(target), _up(up) {
  _view = glm::scale(glm::lookAt(_position, _target, _up),
                     glm::vec3(zoom, zoom, zoom));
}

Apsis::Primitives::Camera::Camera(const glm::vec2& position,
                                  float zoom) {
  _position = glm::vec3(position.x*zoom, 1.0, position.y*zoom);
  _target = glm::vec3(position.x*zoom, 0.0, position.y*zoom);
  _up     = glm::normalize(glm::vec3(0.0, 0.0, -1.0));

  _view = glm::scale(glm::lookAt(_position, _target, _up),
                     glm::vec3(zoom, zoom, zoom));
}

float (*Apsis::Primitives::Camera::view() const)[4] {
  float (*matrix)[4] = (float (*)[4])glm::value_ptr(_view);

  return matrix;
}

const glm::vec3& Apsis::Primitives::Camera::eye() const {
  return _position;
}
