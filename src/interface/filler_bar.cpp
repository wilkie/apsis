#include <apsis/interface/filler_bar.h>

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

Apsis::Hud::FillerBar::FillerBar(Apsis::Sprite::Sheet* spriteSheet,
                             unsigned int startIndex,
                             unsigned int stages,
                             unsigned int count,
                             float x, float y)
  : _spriteSheet(spriteSheet),
    _stages(stages),
    _count(count),
    _startIndex(startIndex),
    _position(x, y, 0.0f, 0.0f) {
  value(0);

  _itemWidth       = _spriteSheet->width(_startIndex);
  _position.width  = _itemWidth * _count;
  _position.height = _spriteSheet->height(_startIndex);
}

Apsis::Sprite::Sheet* Apsis::Hud::FillerBar::spriteSheet() {
  return _spriteSheet;
}

Apsis::Geometry::Rectangle Apsis::Hud::FillerBar::position() {
  return _position;
}

// Draws the actor
void Apsis::Hud::FillerBar::draw(const Primitives::Matrix& projection,
                                 const World::Camera& camera) {
  glm::mat4 model = glm::mat4(1.0);

  unsigned int tmp = _value;

  // TODO: don't abuse rectangle coordinates... x should represent the center, not the left
  unsigned int x = (unsigned int)_position.x;

  // Draw full
  unsigned int i;

  for (i = (_stages - 1); i <= _value; i += (_stages - 1)) {
    model = glm::translate(glm::mat4(1.0),
                           glm::vec3(x, 0.0, _position.y));
    x += (unsigned int)_itemWidth;

    const Primitives::Matrix& model_matrix
      = *(const Primitives::Matrix*)glm::value_ptr(glm::mat4(1.0f));

    _spriteSheet->draw(_startIndex + (_stages - 1), projection, camera, model_matrix);
  }

  // Draw partial
  if (_value < _count * (_stages-1)) {
    model = glm::translate(glm::mat4(1.0),
                           glm::vec3(x, 0.0, _position.y));
    x += (unsigned int)_itemWidth;

    const Primitives::Matrix& model_matrix
      = *(const Primitives::Matrix*)glm::value_ptr(glm::mat4(1.0f));

    _spriteSheet->draw(_startIndex + (i - _value), projection, camera, model_matrix);
  }

  // Draw empty
  for ( ; i < _count * (_stages - 1); i += (_stages - 1)) {
    model = glm::translate(glm::mat4(1.0),
                           glm::vec3(x, 0.0, _position.y));
    x += (unsigned int)_itemWidth;

    const Primitives::Matrix& model_matrix
      = *(const Primitives::Matrix*)glm::value_ptr(glm::mat4(1.0f));

    _spriteSheet->draw(_startIndex, projection, camera, model_matrix);
  }
}

unsigned int Apsis::Hud::FillerBar::value() {
  return _value;
}

void Apsis::Hud::FillerBar::value(unsigned int updated) {
  _value = updated;
}
