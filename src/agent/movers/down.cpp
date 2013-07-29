#include "apsis/agent/movers/down.h"

Apsis::Agent::Movers::Down::Down(Apsis::InputEngine& inputEngine)
  : Apsis::Agent::Mover("can move down") {
  _inputEngine = &inputEngine;
}

bool Apsis::Agent::Movers::Down::update(float elapsed,
                                        std::set<unsigned int>& states,
                                        const Apsis::Geometry::Rectangle& original,
                                        Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if (_inputEngine->isEventHeld(Apsis::Action::PLAYER_1_DOWN)) {
    updated.y += elapsed * 96.0f;
    return true;
  }

  return false;
}