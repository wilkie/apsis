#include "apsis/agent/movers/jump.h"

#include "apsis/registry/state.h"
#include "apsis/registry/action.h"

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

  _collideWithLeftState   = Apsis::Registry::State::id("collideWithLeft");
  _collideWithRightState  = Apsis::Registry::State::id("collideWithRight");
  _collideWithTopState  = Apsis::Registry::State::id("collideWithTop");
  _collideWithBottomState  = Apsis::Registry::State::id("collideWithBottom");

  _jumpingState  = Apsis::Registry::State::id("jumping");
  _canJumpState  = Apsis::Registry::State::id("canJump");

  _jumpAction = Apsis::Registry::Action::id("jump");
}

bool Apsis::Agent::Movers::Jump::update(float elapsed,
                                        std::set<unsigned int>& states,
                                        const Apsis::Geometry::Rectangle& original,
                                        Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if (states.count(_collideWithTopState) > 0 &&
      states.count(_jumpingState) == 0) {
    // If we detect we are landed, allow jump.
    _velocity = _startingVelocity;
    _height = 0.0f;
    if (!_inputEngine->isEventHeld(_jumpAction)) {
      states.insert(_canJumpState);
    }
  }
  
  if (states.count(_collideWithBottomState) > 0) {
    // Release jump upon hitting a ceiling.
    states.erase(_jumpingState);
    states.erase(_canJumpState);
    states.erase(_collideWithBottomState);
    return false;
  }
  else if (states.count(_canJumpState) == 0 &&
           states.count(_jumpingState) == 0) {
    // Do not allow double jumps.
  }
  else if (_inputEngine->isEventHeld(_jumpAction) &&
           states.count(_canJumpState) > 0) {
    // While key is held, jump until maximum height.
    states.insert(_jumpingState);
    states.erase(_collideWithTopState);

    float amount = elapsed * _velocity;

    _height += amount;
    if (_height > _maximumHeight) {
      // Do not allow a jump after our maximum height has been reached.
      amount = _maximumHeight - _height;
      states.erase(_canJumpState);
    }

    updated.y -= amount;

    _velocity -= _deacceleration * elapsed;
    if (_velocity < _minimumVelocity) {
      _velocity = _minimumVelocity;
    }
    return true;
  }
  else if (states.count(_jumpingState) > 0) {
    // Released key, done jumping, deaccelerate.
    states.erase(_canJumpState);

    if (_velocity > 0) {
      _velocity -= _peakDeacceleration * elapsed;
      updated.y -= elapsed * _velocity;
      return true;
    }
    else {
      states.erase(_jumpingState);
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