#include "apsis/agent/movers/gridlock_left.h"

Apsis::Agent::Movers::GridlockLeft::GridlockLeft(Apsis::InputEngine& inputEngine)
  : Apsis::Agent::Mover("can move left locked along a grid") {
  _inputEngine = &inputEngine;
}

bool Apsis::Agent::Movers::GridlockLeft::update(float elapsed,
                                                std::set<unsigned int>& states,
                                                const Apsis::Geometry::Rectangle& original,
                                                Apsis::Geometry::Point& updated) {
  updated.x = original.x;

  if (states.count(Apsis::State::MOVING_UP)  +
      states.count(Apsis::State::MOVING_RIGHT) +
      states.count(Apsis::State::MOVING_DOWN) > 0) {
    return false;
  }

  if (_inputEngine->isEventHeld(Apsis::Action::PLAYER_1_LEFT)) {
    if (states.count(Apsis::State::MOVING_LEFT) == 0) {
      states.insert(Apsis::State::MOVING_LEFT);
    }
    updated.x -= elapsed * 96.0f;

    return true;
  }
  else if (states.count(Apsis::State::MOVING_LEFT) != 0) {
    updated.x -= elapsed * 96.0f;

    float from_grid_0 = floor(original.x / 32.0f);
    float from_grid_1 = floor(updated.x  / 32.0f);

    if (from_grid_1 < from_grid_0) {
      // Cross grid line, align with grid
      updated.x = from_grid_0 * 32.0f;
      states.erase(Apsis::State::MOVING_LEFT);
    }

    return true;
  }

  return false;
}