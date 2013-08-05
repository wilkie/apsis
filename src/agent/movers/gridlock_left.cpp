#include "apsis/agent/movers/gridlock_left.h"

#include "apsis/registry/state.h"

Apsis::Agent::Movers::GridlockLeft::GridlockLeft(Apsis::InputEngine& inputEngine)
  : Apsis::Agent::Mover("can move left locked along a grid") {
  _inputEngine = &inputEngine;

  _movingLeftState  = Apsis::Registry::State::id("movingLeft");
  _movingUpState    = Apsis::Registry::State::id("movingUp");
  _movingRightState = Apsis::Registry::State::id("movingRight");
  _movingDownState  = Apsis::Registry::State::id("movingDown");
}

bool Apsis::Agent::Movers::GridlockLeft::update(float elapsed,
                                                std::set<unsigned int>& states,
                                                const Apsis::Geometry::Rectangle& original,
                                                Apsis::Geometry::Point& updated) {
  updated.x = original.x;

  if (states.count(_movingUpState)  +
      states.count(_movingRightState) +
      states.count(_movingDownState) > 0) {
    return false;
  }

  if (_inputEngine->isEventHeld(Apsis::Action::PLAYER_1_LEFT)) {
    if (states.count(_movingLeftState) == 0) {
      states.insert(_movingLeftState);
    }
    updated.x -= elapsed * 96.0f;

    return true;
  }
  else if (states.count(_movingLeftState) != 0) {
    updated.x -= elapsed * 96.0f;

    float from_grid_0 = floor((float)original.x / 32.0f);
    float from_grid_1 = floor((float)updated.x  / 32.0f);

    if (from_grid_1 < from_grid_0) {
      // Cross grid line, align with grid
      updated.x = from_grid_0 * 32.0f;
      states.erase(_movingLeftState);
    }

    return true;
  }

  return false;
}