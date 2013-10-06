#include "apsis/primitives/math.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

Apsis::Primitives::Matrix Apsis::Primitives::Matrix::identity() {
  const Primitives::Matrix& matrix
    = *(const Primitives::Matrix*)glm::value_ptr(glm::mat4(1.0f));
  return matrix;
}

float Apsis::Primitives::Vector2::x() const {
  return value[0];
}

float Apsis::Primitives::Vector2::y() const {
  return value[1];
}

float Apsis::Primitives::Vector3::x() const {
  return value[0];
}

float Apsis::Primitives::Vector3::y() const {
  return value[1];
}

float Apsis::Primitives::Vector3::z() const {
  return value[2];
}

float Apsis::Primitives::Vector4::x() const {
  return value[0];
}

float Apsis::Primitives::Vector4::y() const {
  return value[1];
}

float Apsis::Primitives::Vector4::z() const {
  return value[2];
}

float Apsis::Primitives::Vector4::w() const {
  return value[3];
}
