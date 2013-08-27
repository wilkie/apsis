#include "apsis/agent/movers/gridlock_down.h"

#include "apsis/registry/state.h"
#include "apsis/registry/action.h"

Apsis::Agent::Movers::GridlockDown::GridlockDown(Apsis::InputEngine& inputEngine)
  : Apsis::Agent::Mover("can move down locked along a grid") {
  _inputEngine = &inputEngine;

  _movingLeftState  = Apsis::Registry::State::id("movingLeft");
  _movingUpState    = Apsis::Registry::State::id("movingUp");
  _movingRightState = Apsis::Registry::State::id("movingRight");
  _movingDownState  = Apsis::Registry::State::id("movingDown");

  _downAction = Apsis::Registry::Action::id("down");
}

bool Apsis::Agent::Movers::GridlockDown::update(float elapsed,
                                                Apsis::World::Object& object,
                                                const Apsis::Geometry::Rectangle& original,
                                                Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if (object.isEnabled(_movingLeftState)  ||
      object.isEnabled(_movingRightState) ||
      object.isEnabled(_movingUpState)) {
    return false;
  }

  if (_inputEngine->isEventHeld(_downAction)) {
    if (!object.isEnabled(_movingDownState)) {
      object.enableState(_movingDownState);
    }
    updated.y += elapsed * 96.0f;

    return true;
  }
  else if (object.isEnabled(_movingDownState)) {
    updated.y += elapsed * 96.0f;

    float from_grid_0 = floor((float)original.y / 32.0f);
    float from_grid_1 = floor((float)updated.y  / 32.0f);
    
    if (from_grid_1 > from_grid_0) {
      updated.y = from_grid_1 * 32.0f;
      object.disableState(_movingDownState);
    }

    return true;
  }

  return false;
}