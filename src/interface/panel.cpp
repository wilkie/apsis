#include "apsis/interface/panel.h"

#include "apsis/sprite/sheet.h"

struct PanelData {
};

void Apsis::Interface::Panel::init(const Apsis::Interface::Window& window,
                                   Apsis::World::Object& object) {
  PanelData* data = new PanelData;
  object.userData(data);
}

void Apsis::Interface::Panel::draw(Apsis::Engine::Graphics& graphics,
                                   const Apsis::Interface::Window& window,
                                   const Apsis::World::Object& object) {
}

void Apsis::Interface::Panel::input(bool pressed,
                                    const Apsis::Input::Binding&    binding,
                                    const Apsis::Geometry::Point&   point,
                                    const Apsis::Interface::Window& window,
                                    Apsis::World::Object&           object) {
}

void Apsis::Interface::Panel::motion(const Apsis::Geometry::Point& point,
                                     const Apsis::Interface::Window& window,
                                     Apsis::World::Object& object) {
}

void Apsis::Interface::Panel::enter(const Apsis::Geometry::Point& point,
                                    const Apsis::Interface::Window& window,
                                    Apsis::World::Object& object) {
}

void Apsis::Interface::Panel::update(float elapsed,
                                     Apsis::Geometry::Rectangle& position,
                                     Apsis::World::Object& object) {
}

void Apsis::Interface::Panel::leave(const Apsis::Geometry::Point& point,
                                    const Apsis::Interface::Window& window,
                                    Apsis::World::Object& object) {
}