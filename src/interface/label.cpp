#include "apsis/interface/label.h"

#include "apsis/sprite/sheet.h"

void Apsis::Interface::Label::init(const Apsis::Geometry::Rectangle& position,
                                   Apsis::World::Object& object) {
  static unsigned int font_id = Apsis::Registry::Property::id("font");
  static unsigned int text_id    = Apsis::Registry::Property::id("text");
  static unsigned int font_id_id = Apsis::Registry::Property::id("__fontid");

  if (!object.has(font_id)) {
    object.set(font_id, "assets/fonts/Cinzel/Cinzel-Bold.ttf");
  }

  if (!object.has(text_id)) {
    object.set(text_id, "Label Text");
  }

  const char* fontname = object.get(font_id).asCString();

  const Apsis::Sprite::Font& font = Apsis::Sprite::Font::load(fontname);

  object.set(font_id_id, (long)font.id());
}

void Apsis::Interface::Label::draw(Apsis::Engine::Graphics& graphics,
                                   const Apsis::Geometry::Rectangle& position,
                                   const Apsis::World::Object& object) {
  static unsigned int text_id = Apsis::Registry::Property::id("text");
  static unsigned int font_id = Apsis::Registry::Property::id("__fontid");

  const char* text        = object.get(text_id).asCString();
  unsigned int font_index = object.get(font_id).asInteger();

  // Draw text
  graphics.font(font_index);
  graphics.draw(position.left(), position.top(), text);
}

void Apsis::Interface::Label::enter(const Apsis::Geometry::Point& point,
                                    const Apsis::Geometry::Rectangle& position,
                                    Apsis::World::Object& object) {
}

void Apsis::Interface::Label::update(float elapsed,
                                     Apsis::Geometry::Rectangle& position,
                                     Apsis::World::Object& object) {
}

void Apsis::Interface::Label::leave(const Apsis::Geometry::Point& point,
                                    const Apsis::Geometry::Rectangle& position,
                                    Apsis::World::Object& object) {
}