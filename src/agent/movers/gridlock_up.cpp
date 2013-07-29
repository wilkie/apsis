#include "apsis/agent/movers/gridlock_up.h"

Apsis::Agent::Movers::GridlockUp::GridlockUp(Apsis::InputEngine& inputEngine)
  : Apsis::Agent::Mover("can move up locked along a grid") {
  _inputEngine = &inputEngine;
}

bool Apsis::Agent::Movers::GridlockUp::update(float elapsed,
                                                std::set<unsigned int>& states,
                                                const Apsis::Geometry::Rectangle& original,
                                                Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if (states.count(Apsis::State::MOVING_LEFT)  +
      states.count(Apsis::State::MOVING_RIGHT) +
      states.count(Apsis::State::MOVING_DOWN) > 0) {
    return false;
  }

  if (_inputEngine->isEventHeld(Apsis::Action::PLAYER_1_UP)) {
    if (states.count(Apsis::State::MOVING_UP) == 0) {
      states.insert(Apsis::State::MOVING_UP);
    }
    updated.y -= elapsed * 96.0f;

    return true;
  }
  else if (states.count(Apsis::State::MOVING_UP) != 0) {
    updated.y -= elapsed * 96.0f;

    float from_grid_0 = floor(original.y / 32.0f);
    float from_grid_1 = floor(updated.y  / 32.0f);
    
    if (from_grid_1 < from_grid_0) {
      updated.y = from_grid_0 * 32.0f;
      states.erase(Apsis::State::MOVING_UP);
    }

    return true;
  }

  return false;
}