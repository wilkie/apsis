#include "apsis/interface/label.h"

void Apsis::Interface::Label::draw(const Apsis::Geometry::Rectangle& position,
                                   const Apsis::World::Object& object) {
  static unsigned int text_id = Apsis::Registry::Property::id("text");

  const char* text = object.get(text_id).asCString();

  // Draw text
}