#include "apsis/interface/label.h"

#include "apsis/sprite/sheet.h"

void Apsis::Interface::Label::draw(const Apsis::Geometry::Rectangle& position,
                                   const Apsis::World::Object& object) {
  static unsigned int text_id = Apsis::Registry::Property::id("text");
  static unsigned int sprite_id = Apsis::Registry::Property::id("spritesheet");

  const char* text = object.get(text_id).asCString();

  // Draw text
  const Apsis::Sprite::Sheet& sheet = Apsis::Sprite::Sheet::loaded(sprite_id);

  static unsigned int start_id = Apsis::Registry::Property::id("font_start_id");
  static unsigned int end_id = Apsis::Registry::Property::id("font_end_id");

  // TODO: don't abuse rectangle coordinates... x should represent the center, not the left
  //float x = _position.x + _position.width;

  // TODO: Utf8!
  // TODO: Drawing::Font class computes how to draw/layout text.
  // TODO: Engine::Graphics passed along will convey the projection and camera
  while (*text != NULL) {
    //x -= _spriteSheet->width(_startIndex + digit);

    //glm::mat4 model = glm::translate(glm::mat4(1.0),
    //                                 glm::vec3(x, 0.0, _position.y));

    //_spriteSheet->draw(_startIndex + digit, projection, camera, model);

    text++;
  }

  // TO BECOME:

  //graphics.useFont(font_id);
  //graphics.draw(position.left(), position.top(), text);
}