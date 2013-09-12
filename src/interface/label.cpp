#include "apsis/interface/label.h"

#include "apsis/sprite/sheet.h"

void Apsis::Interface::Label::draw(Apsis::Engine::Graphics& graphics,
                                   const Apsis::Geometry::Rectangle& position,
                                   const Apsis::World::Object& object) {
  static unsigned int text_id = Apsis::Registry::Property::id("text");
  static unsigned int font_id = Apsis::Registry::Property::id("font");

  const char* text = object.get(text_id).asCString();

  // Draw text
  graphics.font(font_id);
  graphics.draw(position.left(), position.top(), text);
}