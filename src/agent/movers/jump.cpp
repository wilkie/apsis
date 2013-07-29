#include "apsis/agent/movers/jump.h"

Apsis::Agent::Movers::Jump::Jump(Apsis::InputEngine& inputEngine,
                                 float maximumHeight,
                                 float startingVelocity,
                                 float deacceleration,
                                 float peakDeacceleration,
                                 float minimumVelocity)
  : _maximumHeight(maximumHeight),
    _startingVelocity(startingVelocity),
    _deacceleration(deacceleration),
    _peakDeacceleration(peakDeacceleration),
    _minimumVelocity(minimumVelocity),
    Apsis::Agent::Mover("jump") {
  _inputEngine = &inputEngine;
}

bool Apsis::Agent::Movers::Jump::update(float elapsed,
                                        std::set<unsigned int>& states,
                                        const Apsis::Geometry::Rectangle& original,
                                        Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if (states.count(Apsis::State::COLLIDE_DOWN_WITH_MAP) > 0 &&
      states.count(Apsis::State::JUMPING) == 0) {
    // If we detect we are landed, allow jump.
    _velocity = _startingVelocity;
    _height = 0.0f;
    if (!_inputEngine->isEventHeld(Apsis::Action::PLAYER_1_JUMP)) {
      states.insert(Apsis::State::CAN_JUMP);
    }
  }
  
  if (states.count(Apsis::State::COLLIDE_UP_WITH_MAP) > 0) {
    // Release jump upon hitting a ceiling.
    states.erase(Apsis::State::JUMPING);
    states.erase(Apsis::State::CAN_JUMP);
    states.erase(Apsis::State::COLLIDE_UP_WITH_MAP);
    return false;
  }
  else if (states.count(Apsis::State::CAN_JUMP) == 0 &&
           states.count(Apsis::State::JUMPING) == 0) {
    // Do not allow double jumps.
  }
  else if (_inputEngine->isEventHeld(Apsis::Action::PLAYER_1_JUMP) &&
           states.count(Apsis::State::CAN_JUMP) > 0) {
    // While key is held, jump until maximum height.
    states.insert(Apsis::State::JUMPING);
    states.erase(Apsis::State::COLLIDE_DOWN_WITH_MAP);

    float amount = elapsed * _velocity;

    _height += amount;
    if (_height > _maximumHeight) {
      // Do not allow a jump after our maximum height has been reached.
      amount = _maximumHeight - _height;
      states.erase(Apsis::State::CAN_JUMP);
    }

    updated.y -= amount;

    _velocity -= _deacceleration * elapsed;
    if (_velocity < _minimumVelocity) {
      _velocity = _minimumVelocity;
    }
    return true;
  }
  else if (states.count(Apsis::State::JUMPING) > 0) {
    // Released key, done jumping, deaccelerate.
    states.erase(Apsis::State::CAN_JUMP);

    if (_velocity > 0) {
      _velocity -= _peakDeacceleration * elapsed;
      updated.y -= elapsed * _velocity;
      return true;
    }
    else {
      states.erase(Apsis::State::JUMPING);
      _velocity = _startingVelocity;
      _height = 0.0f;
    }
  }
  else {
    _velocity = _startingVelocity;
    _height = 0.0f;
  }

  return false;
}