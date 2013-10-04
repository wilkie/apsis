#include <apsis/interface/numbers.h>

#include "apsis/primitives/fragment_shader.h"
#include "apsis/primitives/vertex_shader.h"

#include "apsis/primitives/unlinked_program.h"
#include "apsis/primitives/program.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

Apsis::Hud::Numbers::Numbers(Apsis::Sprite::Sheet* spriteSheet,
                             unsigned int startIndex,
                             float x, float y)
  : _spriteSheet(spriteSheet),
    _startIndex(startIndex),
    _position(x, y, 0.0f, 0.0f) {
  value(0);
}

Apsis::Sprite::Sheet* Apsis::Hud::Numbers::spriteSheet() {
  return _spriteSheet;
}

Apsis::Geometry::Rectangle Apsis::Hud::Numbers::position() {
  return _position;
}

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

// Draws the actor
void Apsis::Hud::Numbers::draw(const Primitives::Matrix& projection,
                               const World::Camera& camera) {
  unsigned int tmp = _value;

  // TODO: don't abuse rectangle coordinates... x should represent the center, not the left
  float x = _position.x + _position.width;

  while (tmp > 0) {
    unsigned int digit = tmp % 10;
    tmp /= 10;

    x -= _spriteSheet->width(_startIndex + digit);

    glm::mat4 model = glm::translate(glm::mat4(1.0),
                                     glm::vec3(x, 0.0, _position.y));

    const Primitives::Matrix& model_matrix
      = *(const Primitives::Matrix*)glm::value_ptr(glm::mat4(1.0f));

    _spriteSheet->draw(_startIndex + digit, projection, camera, model_matrix);
  }
}

void Apsis::Hud::Numbers::_calculateSize() {
  unsigned int tmp = _value;

  _position.width = 0;
  _position.height = 0;

  while (tmp > 0) {
    unsigned int digit = tmp % 10;
    tmp /= 10;

    _position.width += _spriteSheet->width(_startIndex + digit);
    if (_spriteSheet->height(_startIndex + digit) > _position.height) {
      _position.height = (float)_spriteSheet->height(_startIndex + digit);
    }
  }
}

unsigned int Apsis::Hud::Numbers::value() {
  return _value;
}

void Apsis::Hud::Numbers::value(unsigned int updated) {
  _value = updated;
  _calculateSize();
}
