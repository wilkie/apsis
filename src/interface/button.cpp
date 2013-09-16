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

  float x = 0.0f;
  float y = 0.0f;

  graphics.sheet(sheet_index);
  const Apsis::Sprite::Sheet& sheet = graphics.sheet();

  // Button top
  graphics.draw(position.left(), position.top(), sheet_index, 0);
  
  x += sheet.width(0);
  while (x + sheet.width(1) <= (position.width - sheet.width(2))) {
    graphics.draw(position.left() + x, position.top() + y, sheet_index, 1);
    x += sheet.width(1);
  }
  // TODO: move it so that the right of the corner lines up
  graphics.draw(position.left() + x, position.top() + y, sheet_index, 2);
  
  // Button bottom
  x = 0.0f;
  y = position.height - sheet.height(5);

  graphics.draw(position.left() + x, position.top() + y, sheet_index, 5);
  x += sheet.width(5);
  while (x + sheet.width(6) <= (position.width - sheet.width(7))) {
    graphics.draw(position.left() + x, position.top() + y, sheet_index, 6);
    x += sheet.width(6);
  }
  // TODO: move it so that the right of the corner lines up
  graphics.draw(position.left() + x, position.top() + y, sheet_index, 7);

  // Button left edge
  x = 0.0f;
  y = sheet.height(0);

  while (y + sheet.height(3) <= (position.height - sheet.height(2))) {
    graphics.draw(position.left() + x, position.top() + y, sheet_index, 3);
    y += sheet.height(3);
  }

  // Button right edge
  x = position.width - sheet.width(4);
  y = sheet.height(0);

  while (y + sheet.height(4) <= (position.height - sheet.height(2))) {
    graphics.draw(position.left() + x, position.top() + y, sheet_index, 4);
    y += sheet.height(4);
  }

  // Button inside
  x = sheet.width(0);

  while (x + sheet.width(8) <= (position.width - sheet.width(2))) {
    y = sheet.height(0);
    while (y + sheet.height(8) <= (position.height - sheet.height(5))) {
      graphics.draw(position.left() + x, position.top() + y, sheet_index, 8);
      y += sheet.height(8);
    }
    x += sheet.width(8);
  }
}