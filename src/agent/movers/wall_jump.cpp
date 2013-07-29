#include "apsis/agent/movers/wall_jump.h"

#include <Windows.h>

Apsis::Agent::Movers::WallJump::WallJump(Apsis::InputEngine& inputEngine,
                                         float maximumHeight,
                                         float startingVelocity,
                                         float deacceleration,
                                         float minimumVelocity,
                                         float startingHorizontalVelocity,
                                         float horizontalDeacceleration)
  : _maximumHeight(maximumHeight),
    _startingVelocity(startingVelocity),
    _deacceleration(deacceleration),
    _minimumVelocity(minimumVelocity),
    _startingHorizontalVelocity(startingHorizontalVelocity),
    _horizontalDeacceleration(horizontalDeacceleration),
    Apsis::Agent::Mover("wall jump") {
  _inputEngine = &inputEngine;

  // Rules this supercedes
  supercede("jump");
}

bool Apsis::Agent::Movers::WallJump::update(float elapsed,
                                            std::set<unsigned int>& states,
                                            const Apsis::Geometry::Rectangle& original,
                                            Apsis::Geometry::Point& updated) {
  updated.y = original.y;

  if ((states.count(Apsis::State::COLLIDE_LEFT_WITH_MAP) > 0 ||
       states.count(Apsis::State::COLLIDE_RIGHT_WITH_MAP) > 0) &&
       states.count(Apsis::State::COLLIDE_DOWN_WITH_MAP) == 0 &&
      states.count(Apsis::State::JUMPING) == 0) {
    // If we detect we are on a wall, but not landed, allow jump.
    _velocity = _startingVelocity;

    _horizontalVelocity = _startingHorizontalVelocity;

    _height = 0.0f;
    _wallJumping = true;
    
    _goingRight = states.count(Apsis::State::COLLIDE_LEFT_WITH_MAP) > 0;

    if (!_inputEngine->isEventHeld(Apsis::Action::PLAYER_1_JUMP)) {
      states.insert(Apsis::State::CAN_JUMP);
    }
  }
  else if (states.count(Apsis::State::JUMPING) == 0) {
    _wallJumping = false;
  }
  
  if (_wallJumping) {
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
      states.erase(Apsis::State::COLLIDE_LEFT_WITH_MAP);
      states.erase(Apsis::State::COLLIDE_RIGHT_WITH_MAP);
      states.erase(Apsis::State::COLLIDE_DOWN_WITH_MAP);

      float amount = elapsed * _velocity;

      _height += amount;
      if (_height > _maximumHeight) {
        // Do not allow a jump after our maximum height has been reached.
        amount = _maximumHeight - _height;
        states.erase(Apsis::State::CAN_JUMP);
        states.erase(Apsis::State::JUMPING);
      }

      updated.y -= amount;

      _velocity -= _deacceleration * elapsed;
      if (_velocity < _minimumVelocity) {
        _velocity = _minimumVelocity;
      }

      // Stop moving upward when we cannot move sideways.
      _horizontalVelocity -= _horizontalDeacceleration * elapsed;
      if (_horizontalVelocity <= 0) {
        _horizontalVelocity = 0;
        states.erase(Apsis::State::CAN_JUMP);
        states.erase(Apsis::State::JUMPING);
      }
      if (_goingRight) {
        updated.x += _horizontalVelocity * elapsed;
      }
      else {
        updated.x -= _horizontalVelocity * elapsed;
      }

      return true;
    }
    else if (states.count(Apsis::State::JUMPING) > 0) {
      // Released key, done jumping.
      states.erase(Apsis::State::CAN_JUMP);
    }
    else {
      _velocity = _startingVelocity;
      _height = 0.0f;
    }
  }

  return false;
}