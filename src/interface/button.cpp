#include "apsis/interface/button.h"

#include "apsis/sprite/sheet.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

void Apsis::Interface::Button::init(Apsis::World::Object& object) {
  static unsigned int font_id = Apsis::Registry::Property::id("font");
  static unsigned int text_id    = Apsis::Registry::Property::id("text");
  static unsigned int font_id_id = Apsis::Registry::Property::id("__fontid");
  static unsigned int sprite_id = Apsis::Registry::Property::id("sprite");
  static unsigned int sprite_id_id = Apsis::Registry::Property::id("__spriteid");
  
  if (!object.has(sprite_id)) {
    object.set(sprite_id, "button");
  }

  if (!object.has(font_id)) {
    object.set(font_id, "assets/fonts/Cinzel/Cinzel-Bold.ttf");
  }

  if (!object.has(text_id)) {
    object.set(text_id, "Start");
  }

  const char* fontname = object.get(font_id).asCString();

  const Apsis::Sprite::Font& font = Apsis::Sprite::Font::load(fontname);

  object.set(font_id_id, (long)font.id());

  const Apsis::Sprite::Sheet& sheet = Apsis::Sprite::Sheet::load("assets/graphics/button.png");

  object.set(sprite_id_id, (long)sheet.id());
}

void Apsis::Interface::Button::draw(Apsis::Engine::Graphics& graphics,
                                   const Apsis::Geometry::Rectangle& position,
                                   const Apsis::World::Object& object) {
  static unsigned int text_id = Apsis::Registry::Property::id("text");
  static unsigned int font_id = Apsis::Registry::Property::id("__fontid");
  static unsigned int sprite_id = Apsis::Registry::Property::id("__spriteid");

  const char* text         = object.get(text_id).asCString();
  unsigned int font_index  = object.get(font_id).asInteger();
  unsigned int sheet_index = object.get(sprite_id).asInteger();

  // Draw text
  graphics.font(font_index);
  graphics.draw(position.left(), position.top(), text);

  const Apsis::Sprite::Sheet& sheet = Apsis::Sprite::Sheet::loaded(sheet_index);

  float x = 0.0f;
  float y = 0.0f;

  // Button top
  sheet.draw(0, graphics.projection(), graphics.camera(), *(const Primitives::Matrix*)glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(position.left(), 0.0, position.top()))));
  x += sheet.width(0);
  while (x + sheet.width(1) <= (position.width - sheet.width(2))) {
    sheet.draw(1, graphics.projection(), graphics.camera(), *(const Primitives::Matrix*)glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(position.left() + x, 0.0, position.top()))));
    x += sheet.width(1);
  }
  // TODO: move it so that the right of the corner lines up
  sheet.draw(2, graphics.projection(), graphics.camera(), *(const Primitives::Matrix*)glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(position.left() + x, 0.0, position.top()))));
  
  // Button bottom
  x = 0.0f;
  y = position.height - sheet.height(5);

  sheet.draw(5, graphics.projection(), graphics.camera(), *(const Primitives::Matrix*)glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(position.left(), 0.0, position.top() + y))));
  x += sheet.width(5);
  while (x + sheet.width(6) <= (position.width - sheet.width(7))) {
    sheet.draw(6, graphics.projection(), graphics.camera(), *(const Primitives::Matrix*)glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(position.left() + x, 0.0, position.top() + y))));
    x += sheet.width(6);
  }
  // TODO: move it so that the right of the corner lines up
  sheet.draw(7, graphics.projection(), graphics.camera(), *(const Primitives::Matrix*)glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(position.left() + x, 0.0, position.top() + y))));

  // Button left edge
  x = 0.0f;
  y = sheet.height(0);

  while (y + sheet.height(3) <= (position.height - sheet.height(2))) {
    sheet.draw(3, graphics.projection(), graphics.camera(), *(const Primitives::Matrix*)glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(position.left() + x, 0.0, position.top() + y))));
    y += sheet.height(3);
  }

  // Button right edge
  x = position.width - sheet.width(4);
  y = sheet.height(0);

  while (y + sheet.height(4) <= (position.height - sheet.height(2))) {
    sheet.draw(4, graphics.projection(), graphics.camera(), *(const Primitives::Matrix*)glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(position.left() + x, 0.0, position.top() + y))));
    y += sheet.height(4);
  }

  // Button inside
  x = sheet.width(0);

  while (x + sheet.width(8) <= (position.width - sheet.width(2))) {
    y = sheet.height(0);
    while (y + sheet.height(8) <= (position.height - sheet.height(5))) {
      sheet.draw(8, graphics.projection(), graphics.camera(), *(const Primitives::Matrix*)glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(position.left() + x, 0.0, position.top() + y))));
      y += sheet.height(8);
    }
    x += sheet.width(8);
  }
}