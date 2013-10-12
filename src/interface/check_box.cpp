#include "apsis/interface/check_box.h"

#include "apsis/sprite/sheet.h"
#include "apsis/sprite/batch.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

using namespace Apsis;

struct CheckBoxData {
  const Sprite::Font*  font;
  const Sprite::Sheet* sheet;

  const Sprite::Batch* batch;
  const Sprite::Batch* check;
  const Sprite::Batch* check_hover;
  const Sprite::Batch* check_pressed_hover;

  float text_width;
  float text_height;

  float text_x;
  float text_y;
};

void Interface::CheckBox::init(const Interface::Window& window,
                               World::Object& object,
                               const Engine::Object& loader) {
  static unsigned int font_id = Apsis::Registry::Property::id("font");
  static unsigned int text_id    = Apsis::Registry::Property::id("text");
  static unsigned int sprite_id = Apsis::Registry::Property::id("sprite");

  const Apsis::Geometry::Rectangle& position = window.position();

  CheckBoxData* data = new CheckBoxData;
  
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

  const Apsis::Sprite::Font& font = Apsis::Sprite::Font::load(fontname, loader);

  data->font = &font;

  const Apsis::Sprite::Sheet& sheet = loader.loadSheet("checkbox");

  data->sheet = &sheet;

  Apsis::Sprite::Batch& batch = Apsis::Sprite::Batch::load(sheet, loader);

  float x = 0.0f;
  float y = 0.0f;

  float box_width = position.width;
  float box_height = position.height;

  // Make it square
  if (box_width > box_height) {
    box_width = box_height;
  }
  else {
    box_height = box_width;
  }

  // Button top

  // Corner (topleft)
  batch.add(0, 0.0f, 0.0f);
  x += sheet.width(0);
  while (x + sheet.width(1) <= (box_width - sheet.width(2))) {
    batch.add(1, x, y);
    x += sheet.width(1);
  }
  // Add last bit
  batch.add(1, x, y, (box_width - sheet.width(2)) - x, sheet.height(1));
  // Corner (topright)
  batch.add(2, box_width - sheet.width(2), y);
  
  // Button bottom
  x = 0.0f;
  y = position.height - sheet.height(5);

  batch.add(5, x, y);
  x += sheet.width(5);
  while (x + sheet.width(6) <= (box_width - sheet.width(7))) {
    batch.add(6, x, y);
    x += sheet.width(6);
  }
  // Add last bit
  batch.add(6, x, y, (box_width - sheet.width(7)) - x, sheet.height(6));
  // Corner
  batch.add(7, box_width - sheet.width(7), y);

  // Button left edge
  x = 0.0f;
  y = sheet.height(0);
  while (y + sheet.height(3) <= (box_height - sheet.height(2))) {
    batch.add(3, x, y);
    y += sheet.height(3);
  }
  // Add last bit
  batch.add(3, x, y, sheet.width(3), (box_height - sheet.height(2)) - y);

  // Button right edge
  x = box_width - sheet.width(4);
  y = sheet.height(0);
  while (y + sheet.height(4) <= (box_height - sheet.height(2))) {
    batch.add(4, x, y);
    y += sheet.height(4);
  }
  // Add last bit
  batch.add(4, x, y, sheet.width(4), (box_height - sheet.height(2)) - y);

  // Button inside
  x = sheet.width(0);
  while (x + sheet.width(8) <= (box_width - sheet.width(2))) {
    y = sheet.height(0);
    while (y + sheet.height(8) <= (box_height - sheet.height(5))) {
      batch.add(8, x, y);
      y += sheet.height(8);
    }
    // Add little bit
    batch.add(8, x, y, sheet.width(8), (box_height - sheet.height(5)) - y);
    x += sheet.width(8);
  }

  // Add little bit (corner)
  y = sheet.height(0);
  batch.add(8, x, y, (box_width - sheet.width(2)) - x, (box_height - sheet.height(5)) - y);

  data->batch = &batch;

  // Check mark
  Apsis::Sprite::Batch& check_batch = Apsis::Sprite::Batch::load(sheet, loader);
  check_batch.add(9,
                  sheet.width(0),
                  sheet.height(0),
                  box_width - sheet.width(0) - sheet.width(7),
                  box_height - sheet.height(0) - sheet.height(7),
                  0.0f,
                  0.0f,
                  sheet.width(9),
                  sheet.height(9));
  data->check = &check_batch;

  Apsis::Sprite::Batch& check_hover_batch = Apsis::Sprite::Batch::load(sheet, loader);
  check_hover_batch.add(10,
                        sheet.width(0),
                        sheet.height(0),
                        box_width - sheet.width(0) - sheet.width(7),
                        box_height - sheet.height(0) - sheet.height(7),
                        0.0f,
                        0.0f,
                        sheet.width(9),
                        sheet.height(9));
  data->check_hover = &check_hover_batch;

  Apsis::Sprite::Batch& check_hover_2_batch = Apsis::Sprite::Batch::load(sheet, loader);
  check_hover_2_batch.add(11,
                        sheet.width(0),
                        sheet.height(0),
                        box_width - sheet.width(0) - sheet.width(7),
                        box_height - sheet.height(0) - sheet.height(7),
                        0.0f,
                        0.0f,
                        sheet.width(9),
                        sheet.height(9));
  data->check_pressed_hover = &check_hover_2_batch;

  const char* text = object.get(text_id).asCString();
  data->text_width = font.width(text);
  data->text_height = font.height(text);

  data->text_x = box_width + 10.0f;
  data->text_y = (position.height - data->text_height - font.descent()) / 2.0f;
  data->text_y += font.ascent();

  object.userData(data);
}

void Interface::CheckBox::draw(Engine::Graphics& graphics,
                               const Interface::Window& window,
                               const World::Object& object) {
  static unsigned int text_id = Apsis::Registry::Property::id("text");

  const Apsis::Geometry::Rectangle& position = window.position();

  const CheckBoxData& data = *(CheckBoxData*)object.userData();
  const Apsis::Sprite::Batch& batch = *data.batch;
  const Apsis::Sprite::Batch& check_batch = *data.check;
  const Apsis::Sprite::Batch& check_hover_batch = *data.check_hover;
  const Apsis::Sprite::Batch& check_pressed_hover_batch = *data.check_pressed_hover;
  const Apsis::Sprite::Sheet& sheet = *data.sheet;
  const Apsis::Sprite::Font&  font  = *data.font;

  const char* text = object.get(text_id).asCString();

  // Draw area
  graphics.sheet(sheet.id());
  batch.draw(graphics.projection(), graphics.camera(), *(const Apsis::Primitives::Matrix*)glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(position.left(), 0.0f, position.top()))));

  if (object.has("value") && object.get("value").asInteger() == 1) {
    // Draw check
    if (window.hovered()) {
      check_pressed_hover_batch.draw(graphics.projection(), graphics.camera(), *(const Apsis::Primitives::Matrix*)glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(position.left(), 0.0f, position.top()))));
    }
    else {
      check_batch.draw(graphics.projection(), graphics.camera(), *(const Apsis::Primitives::Matrix*)glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(position.left(), 0.0f, position.top()))));
    }
  }
  else if (window.hovered()) {
    check_hover_batch.draw(graphics.projection(), graphics.camera(), *(const Apsis::Primitives::Matrix*)glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(position.left(), 0.0f, position.top()))));
  }

  // Draw text
  graphics.font(font.id());
  graphics.draw(position.left() + data.text_x, position.top() + data.text_y, text);
}

void Apsis::Interface::CheckBox::input(bool pressed,
                                       const Input::Binding&    binding,
                                       const Geometry::Point&   point,
                                       const Interface::Window& window,
                                       World::Object&           object) {
  if (!binding.isMouse()) {
  }
  else if (pressed) {
    object.set("pressed", (long)1);
  }
  else if (object.has("pressed") && object.get("pressed").asInteger() == 1) {
    bool value = false;
    if (object.has("value") && object.get("value").asInteger() == 1) {
      value = true;
    }

    value = !value;
    object.set("value", (long)value);

    object.set("pressed", (long)0);
  }
}

void Interface::CheckBox::motion(const Geometry::Point& point,
                                 const Interface::Window& window,
                                 World::Object& object) {
}

void Interface::CheckBox::update(float elapsed,
                                 Geometry::Rectangle& position,
                                 World::Object& object) {
}

void Interface::CheckBox::enter(const Geometry::Point& point,
                                const Interface::Window& window,
                                World::Object& object) {
}

void Interface::CheckBox::leave(const Apsis::Geometry::Point& point,
                                const Interface::Window& window,
                                World::Object& object) {
}