#include "apsis/agent/movers/gridlock_up.h"

#include "apsis/registry/state.h"

Apsis::Agent::Movers::GridlockUp::GridlockUp(Apsis::InputEngine& inputEngine)
  : Apsis::Agent::Mover("can move up locked along a grid") {
  _inputEngine = &inputEngine;

  _movingLeftState  = Apsis::Registry::State::id("movingLeft");
  _movingUpState    = Apsis::Registry::State::id("movingUp");
  _movingRightState = Apsis::Registry::State::id("movingRight");
  _movingDownState  = Apsis::Registry::State::id("movingDown");
}

bool Apsis::Agent::Movers::GridlockUp::update(float elapsed,
                                                std::set<unsigned int>& states,
                                                const Apsis::Geometry::Rectangle& original,
                                                Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if (states.count(_movingLeftState)  +
      states.count(_movingRightState) +
      states.count(_movingDownState) > 0) {
    return false;
  }

  if (_inputEngine->isEventHeld(Apsis::Action::PLAYER_1_UP)) {
    if (states.count(_movingUpState) == 0) {
      states.insert(_movingUpState);
    }
    updated.y -= elapsed * 96.0f;

    return true;
  }
  else if (states.count(_movingUpState) != 0) {
    updated.y -= elapsed * 96.0f;

    float from_grid_0 = floor((float)original.y / 32.0f);
    float from_grid_1 = floor((float)updated.y  / 32.0f);
    
    if (from_grid_1 < from_grid_0) {
      updated.y = from_grid_0 * 32.0f;
      states.erase(_movingUpState);
    }

    return true;
  }

  return false;
}