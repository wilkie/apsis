#include "apsis/agent/movers/up.h"

Apsis::Agent::Movers::Up::Up(Apsis::InputEngine& inputEngine) {
  _inputEngine = &inputEngine;
}

bool Apsis::Agent::Movers::Up::update(float elapsed,
                                      std::set<unsigned int>& states,
                                      Apsis::Geometry::Rectangle& original,
                                      Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if (_inputEngine->isEventHeld(Apsis::Action::PLAYER_1_UP)) {
    updated.y -= elapsed * 96.0f;
    return true;
  }

  return false;
}

char* Apsis::Agent::Movers::Up::rule() {
  return "can move up";
}