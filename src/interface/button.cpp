#include "apsis/interface/button.h"

#include "apsis/sprite/sheet.h"
#include "apsis/sprite/batch.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

void Apsis::Interface::Button::init(const Apsis::Geometry::Rectangle& position,
                                    Apsis::World::Object& object) {
  static unsigned int font_id = Apsis::Registry::Property::id("font");
  static unsigned int text_id    = Apsis::Registry::Property::id("text");
  static unsigned int font_id_id = Apsis::Registry::Property::id("__fontid");
  static unsigned int sprite_id = Apsis::Registry::Property::id("sprite");
  static unsigned int sprite_id_id = Apsis::Registry::Property::id("__spriteid");
  static unsigned int batch_id_id = Apsis::Registry::Property::id("__batchid");
  
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

  Apsis::Sprite::Batch& batch = Apsis::Sprite::Batch::load(sheet);

  float x = 0.0f;
  float y = 0.0f;

  // Button top

  // Corner (topleft)
  batch.add(0, 0.0f, 0.0f);
  x += sheet.width(0);
  while (x + sheet.width(1) <= (position.width - sheet.width(2))) {
    batch.add(1, x, y);
    x += sheet.width(1);
  }
  // Add last bit
  batch.add(1, x, y, (position.width - sheet.width(2)) - x, sheet.height(1));
  // Corner (topright)
  batch.add(2, position.width - sheet.width(2), y);
  
  // Button bottom
  x = 0.0f;
  y = position.height - sheet.height(5);

  batch.add(5, x, y);
  x += sheet.width(5);
  while (x + sheet.width(6) <= (position.width - sheet.width(7))) {
    batch.add(6, x, y);
    x += sheet.width(6);
  }
  // Add last bit
  batch.add(6, x, y, (position.width - sheet.width(7)) - x, sheet.height(6));
  // Corner
  batch.add(7, position.width - sheet.width(7), y);

  // Button left edge
  x = 0.0f;
  y = sheet.height(0);
  while (y + sheet.height(3) <= (position.height - sheet.height(2))) {
    batch.add(3, x, y);
    y += sheet.height(3);
  }
  // Add last bit
  batch.add(3, x, y, sheet.width(3), (position.height - sheet.height(2)) - y);

  // Button right edge
  x = position.width - sheet.width(4);
  y = sheet.height(0);
  while (y + sheet.height(4) <= (position.height - sheet.height(2))) {
    batch.add(4, x, y);
    y += sheet.height(4);
  }
  // Add last bit
  batch.add(4, x, y, sheet.width(4), (position.height - sheet.height(2)) - y);

  // Button inside
  x = sheet.width(0);
  while (x + sheet.width(8) <= (position.width - sheet.width(2))) {
    y = sheet.height(0);
    while (y + sheet.height(8) <= (position.height - sheet.height(5))) {
      batch.add(8, x, y);
      y += sheet.height(8);
    }
    // Add little bit
    batch.add(8, x, y, sheet.width(8), (position.height - sheet.height(5)) - y);
    x += sheet.width(8);
  }

  // Add little bit (corner)
  y = sheet.height(0);
  batch.add(8, x, y, (position.width - sheet.width(2)) - x, (position.height - sheet.height(5)) - y);

  object.set(batch_id_id, (long)batch.id());
}

void Apsis::Interface::Button::draw(Apsis::Engine::Graphics& graphics,
                                   const Apsis::Geometry::Rectangle& position,
                                   const Apsis::World::Object& object) {
  static unsigned int text_id   = Apsis::Registry::Property::id("text");
  static unsigned int font_id   = Apsis::Registry::Property::id("__fontid");
  static unsigned int sprite_id = Apsis::Registry::Property::id("__spriteid");
  static unsigned int batch_id  = Apsis::Registry::Property::id("__batchid");

  const char* text         = object.get(text_id).asCString();
  unsigned int font_index  = object.get(font_id).asInteger();
  unsigned int sheet_index = object.get(sprite_id).asInteger();
  unsigned int batch_index = object.get(batch_id).asInteger();

  // Draw text
  graphics.font(font_index);
  graphics.draw(position.left(), position.top(), text);

  if (!object.has(batch_id)) {
    graphics.sheet(sheet_index);
  }

  graphics.sheet(sheet_index);
  const Apsis::Sprite::Batch& batch = Apsis::Sprite::Batch::loaded(batch_index);
  batch.draw(graphics.projection(), graphics.camera(), *(const Apsis::Primitives::Matrix*)glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(position.left(), 0.0f, position.top()))));
}