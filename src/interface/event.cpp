#include "apsis/interface/event.h"

using namespace Apsis;

void Interface::Event::defaultInit(const Apsis::Interface::Window& window,
                                   Apsis::World::Object& object,
                                   const Engine::Object& loader) {
}

void Interface::Event::defaultDraw(Apsis::Engine::Graphics&        graphics,
                                   const Apsis::Interface::Window& window,
                                   const Apsis::World::Object&     object) {
}

void Interface::Event::defaultUpdate(float elapsed,
                                     Apsis::Geometry::Rectangle& position,
                                     Apsis::World::Object&       object) {
}

void Interface::Event::defaultInput(bool pressed,
                                    const Apsis::Input::Binding&    binding,
                                    const Apsis::Geometry::Point&   point,
                                    const Apsis::Interface::Window& window,
                                    Apsis::World::Object&           object) {
}

void Interface::Event::defaultMotion(const Apsis::Geometry::Point&   point,
                                     const Apsis::Interface::Window& window,
                                     Apsis::World::Object&           object) {
}

void Interface::Event::defaultEnter(const Apsis::Geometry::Point&   point,
                                    const Apsis::Interface::Window& window,
                                    Apsis::World::Object&           object) {
}

void Interface::Event::defaultLeave(const Apsis::Geometry::Point&   point,
                                    const Apsis::Interface::Window& window,
                                    Apsis::World::Object&           object) {
}