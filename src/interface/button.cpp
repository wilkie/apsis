#include "apsis/interface/button.h"

#include "apsis/sprite/sheet.h"
#include "apsis/sprite/batch.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

struct ButtonData {
  const Apsis::Sprite::Font*  font;
  const Apsis::Sprite::Sheet* sheet;

  const Apsis::Sprite::Batch* batch;
  const Apsis::Sprite::Batch* batch_hover;
  const Apsis::Sprite::Batch* batch_down;

  float text_width;
  float text_height;

  float text_x;
  float text_y;
};

void Apsis::Interface::Button::init(const Apsis::Interface::Window& window,
                                    Apsis::World::Object& object) {
  static unsigned int font_id = Apsis::Registry::Property::id("font");
  static unsigned int text_id    = Apsis::Registry::Property::id("text");
  static unsigned int sprite_id = Apsis::Registry::Property::id("sprite");

  const Apsis::Geometry::Rectangle& position = window.position();

  ButtonData* data = new ButtonData;
  
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

  data->font = &font;

  const Apsis::Sprite::Sheet& sheet = Apsis::Sprite::Sheet::load("assets/graphics/button2.png");

  data->sheet = &sheet;

  Apsis::Sprite::Batch& batch = Apsis::Sprite::Batch::load(sheet);
  Apsis::Sprite::Batch& batch_hover = Apsis::Sprite::Batch::load(sheet);
  Apsis::Sprite::Batch& batch_down = Apsis::Sprite::Batch::load(sheet);

  float x = 0.0f;
  float y = 0.0f;

  // Button top

  // Corner (topleft)
  batch.add(0, 0.0f, 0.0f);
  batch_hover.add(9, 0.0f, 0.0f);
  batch_down.add(18, 0.0f, 0.0f);
  x += sheet.width(0);
  while (x + sheet.width(1) <= (position.width - sheet.width(2))) {
    batch.add(1, x, y);
    batch_hover.add(10, x, y);
    batch_down.add(19, x, y);
    x += sheet.width(1);
  }
  // Add last bit
  batch.add(1, x, y, (position.width - sheet.width(2)) - x, sheet.height(1));
  batch_hover.add(10, x, y, (position.width - sheet.width(2)) - x, sheet.height(1));
  batch_down.add(19, x, y, (position.width - sheet.width(2)) - x, sheet.height(1));
  // Corner (topright)
  batch.add(2, position.width - sheet.width(2), y);
  batch_hover.add(11, position.width - sheet.width(2), y);
  batch_down.add(20, position.width - sheet.width(2), y);
  
  // Button bottom
  x = 0.0f;
  y = position.height - sheet.height(5);

  batch.add(5, x, y);
  batch_hover.add(14, x, y);
  batch_down.add(23, x, y);
  x += sheet.width(5);
  while (x + sheet.width(6) <= (position.width - sheet.width(7))) {
    batch.add(6, x, y);
    batch_hover.add(15, x, y);
    batch_down.add(24, x, y);
    x += sheet.width(6);
  }
  // Add last bit
  batch.add(6, x, y, (position.width - sheet.width(7)) - x, sheet.height(6));
  batch_hover.add(15, x, y, (position.width - sheet.width(7)) - x, sheet.height(6));
  batch_down.add(24, x, y, (position.width - sheet.width(7)) - x, sheet.height(6));
  // Corner
  batch.add(7, position.width - sheet.width(7), y);
  batch_hover.add(16, position.width - sheet.width(7), y);
  batch_down.add(25, position.width - sheet.width(7), y);

  // Button left edge
  x = 0.0f;
  y = sheet.height(0);
  while (y + sheet.height(3) <= (position.height - sheet.height(2))) {
    batch.add(3, x, y);
    batch_hover.add(12, x, y);
    batch_down.add(21, x, y);
    y += sheet.height(3);
  }
  // Add last bit
  batch.add(3, x, y, sheet.width(3), (position.height - sheet.height(2)) - y);
  batch_hover.add(12, x, y, sheet.width(3), (position.height - sheet.height(2)) - y);
  batch_down.add(21, x, y, sheet.width(3), (position.height - sheet.height(2)) - y);

  // Button right edge
  x = position.width - sheet.width(4);
  y = sheet.height(0);
  while (y + sheet.height(4) <= (position.height - sheet.height(2))) {
    batch.add(4, x, y);
    batch_hover.add(13, x, y);
    batch_down.add(22, x, y);
    y += sheet.height(4);
  }
  // Add last bit
  batch.add(4, x, y, sheet.width(4), (position.height - sheet.height(2)) - y);
  batch_hover.add(13, x, y, sheet.width(4), (position.height - sheet.height(2)) - y);
  batch_down.add(22, x, y, sheet.width(4), (position.height - sheet.height(2)) - y);

  // Button inside
  x = sheet.width(0);
  while (x + sheet.width(8) <= (position.width - sheet.width(2))) {
    y = sheet.height(0);
    while (y + sheet.height(8) <= (position.height - sheet.height(5))) {
      batch.add(8, x, y);
      batch_hover.add(17, x, y);
      batch_down.add(26, x, y);
      y += sheet.height(8);
    }
    // Add little bit
    batch.add(8, x, y, sheet.width(8), (position.height - sheet.height(5)) - y);
    batch_hover.add(17, x, y, sheet.width(8), (position.height - sheet.height(5)) - y);
    batch_down.add(26, x, y, sheet.width(8), (position.height - sheet.height(5)) - y);
    x += sheet.width(8);
  }

  // Add little bit (corner)
  y = sheet.height(0);
  batch.add(8, x, y, (position.width - sheet.width(2)) - x, (position.height - sheet.height(5)) - y);
  batch_hover.add(17, x, y, (position.width - sheet.width(2)) - x, (position.height - sheet.height(5)) - y);
  batch_down.add(26, x, y, (position.width - sheet.width(2)) - x, (position.height - sheet.height(5)) - y);

  data->batch = &batch;
  data->batch_hover = &batch_hover;
  data->batch_down = &batch_down;

  const char* text = object.get(text_id).asCString();
  data->text_width = font.width(text);
  data->text_height = font.height(text);

  data->text_x = (position.width  - data->text_width)  / 2.0f;
  data->text_y = (position.height - data->text_height - font.descent()) / 2.0f;
  data->text_y += font.ascent();

  object.userData(data);
}

void Apsis::Interface::Button::draw(Apsis::Engine::Graphics& graphics,
                                    const Apsis::Interface::Window& window,
                                    const Apsis::World::Object& object) {
  static unsigned int text_id = Apsis::Registry::Property::id("text");

  const Apsis::Geometry::Rectangle& position = window.position();

  const ButtonData& data = *(ButtonData*)object.userData();
  const Apsis::Sprite::Batch& batch = *data.batch;
  const Apsis::Sprite::Batch& batch_hover = *data.batch_hover;
  const Apsis::Sprite::Batch& batch_down = *data.batch_down;
  const Apsis::Sprite::Sheet& sheet = *data.sheet;
  const Apsis::Sprite::Font&  font  = *data.font;

  const char* text = object.get(text_id).asCString();

  // Draw button
  graphics.sheet(sheet.id());
  
  if (object.has("pressed") && object.get("pressed").asInteger() == 1) {
    batch_down.draw(graphics.projection(), graphics.camera(), *(const Apsis::Primitives::Matrix*)glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(position.left(), 0.0f, position.top()))));
  }
  else if (window.hovered()) {
    batch_hover.draw(graphics.projection(), graphics.camera(), *(const Apsis::Primitives::Matrix*)glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(position.left(), 0.0f, position.top()))));
  }
  else {
    batch.draw(graphics.projection(), graphics.camera(), *(const Apsis::Primitives::Matrix*)glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(position.left(), 0.0f, position.top()))));
  }

  // Draw text
  graphics.font(font.id());
  graphics.draw(position.left() + data.text_x, position.top() + data.text_y, text);
}

void Apsis::Interface::Button::input(bool pressed,
                                     const Apsis::Input::Binding&    binding,
                                     const Apsis::Geometry::Point&   point,
                                     const Apsis::Interface::Window& window,
                                     Apsis::World::Object&           object) {
  if (!binding.isMouse()) {
  }
  else if (pressed) {
    object.set("pressed", (long)1);
  }
  else {
    object.set("pressed", (long)0);
  }
}

void Apsis::Interface::Button::motion(const Apsis::Geometry::Point& point,
                                      const Apsis::Interface::Window& window,
                                      Apsis::World::Object& object) {
}

void Apsis::Interface::Button::update(float elapsed,
                                      Apsis::Geometry::Rectangle& position,
                                      Apsis::World::Object& object) {
}

void Apsis::Interface::Button::enter(const Apsis::Geometry::Point& point,
                                     const Apsis::Interface::Window& window,
                                     Apsis::World::Object& object) {
}

void Apsis::Interface::Button::leave(const Apsis::Geometry::Point& point,
                                     const Apsis::Interface::Window& window,
                                     Apsis::World::Object& object) {
}