#include "apsis/agent/movers/gridlock_right.h"

#include "apsis/registry/state.h"
#include "apsis/registry/action.h"

Apsis::Agent::Movers::GridlockRight::GridlockRight(Apsis::InputEngine& inputEngine)
  : Apsis::Agent::Mover("can move right locked along a grid") {
  _inputEngine = &inputEngine;

  _rightAction = Apsis::Registry::Action::id("right");

  _movingLeftState  = Apsis::Registry::State::id("movingLeft");
  _movingUpState    = Apsis::Registry::State::id("movingUp");
  _movingRightState = Apsis::Registry::State::id("movingRight");
  _movingDownState  = Apsis::Registry::State::id("movingDown");
}

bool Apsis::Agent::Movers::GridlockRight::update(float elapsed,
                                                std::set<unsigned int>& states,
                                                const Apsis::Geometry::Rectangle& original,
                                                Apsis::Geometry::Point& updated) {
  updated.x = original.x;

  if (states.count(_movingLeftState) +
      states.count(_movingUpState)   +
      states.count(_movingDownState) > 0) {
    return false;
  }

  if (_inputEngine->isEventHeld(_rightAction)) {
    if (states.count(_movingRightState) == 0) {
      states.insert(_movingRightState);
    }
    updated.x += elapsed * 96.0f;

    return true;
  }
  else if (states.count(_movingRightState) != 0) {
    updated.x += elapsed * 96.0f;

    float from_grid_0 = floor((float)original.x / 32.0f);
    float from_grid_1 = floor((float)updated.x  / 32.0f);
    
    if (from_grid_1 > from_grid_0) {
      updated.x = from_grid_1 * 32.0f;
      states.erase(_movingRightState);
    }

    return true;
  }

  return false;
}