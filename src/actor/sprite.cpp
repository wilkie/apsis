#include "apsis/actor/sprite.h"

Apsis::Actor::Sprite::~Sprite() {
  // Deallocate animations
  for (unsigned int i = 0; i < _animations.size(); i++) {
    for (unsigned int j = 0; j < _animations[i]->frames.size(); j++) {
      delete _animations[i]->frames[j];
    }
    delete _animations[i];
  }
}

Apsis::Actor::Sprite::Sprite(const char* actorFile,
                             unsigned int x,
                             unsigned int y) {
  _currentAnimation = NULL;
  _currentFrame = 0;
  _frame = NULL;
  _currentTime = 0;

  FILE* f = fopen(actorFile, "rt");

  _position.x = x;
  _position.y = y;

  char key[129];
  char val[129];

  while(!feof(f)) {
    fscanf(f, "%128s %128s\n", key, val);

    if (strcmp(key, "width") == 0) {
      _position.width = atoi(val);
    }
    else if (strcmp(key, "height") == 0) {
      _position.height = atoi(val);
    }
    else if (strcmp(key, "move_rate") == 0) {
      _moveRate = atoi(val);
    }
    else if (strcmp(key, "sprites") == 0) {
      _spriteSheet = new Apsis::Primitives::SpriteSheet(val);
    }
    else if (strcmp(key, "state") == 0) {
      _newState(val);
    }
    else if (strcmp(key, "default_state") == 0) {
      setCurrentState(val);
    }
    else {
      // Animation

      // Create an animation structure
      Animation* newAnimation = _newAnimation(key);

      // Store all of the sprites
      int spriteIndex = -1;
      do {
        spriteIndex = _spriteSheet->enumerateSprites(val, spriteIndex+1);
        if (spriteIndex != -1) {
          AnimationFrame* frame = new AnimationFrame;
          _spriteSheet->textureCoordinates(spriteIndex, frame->textureCoordinates);
          frame->sprite = _spriteSheet->sprite(spriteIndex);
          newAnimation->frames.push_back(frame);
        }
      } while(spriteIndex != -1);
    }
  }
}

Apsis::Actor::Animation* Apsis::Actor::Sprite::_newAnimation(const char* name) {
  Animation* ret = new Animation;
  strncpy(ret->name, name, 128);
  _animations.push_back(ret);
  return ret;
}

char* Apsis::Actor::Sprite::_newState(const char* name) {
  char* ret = new char[129];
  strncpy(ret, name, 128);
  _states.push_back(ret);
  return ret;
}

void Apsis::Actor::Sprite::setCurrentState(const char* stateName) {
  for (unsigned int i = 0; i < _states.size(); i++) {
    if (strncmp(_states[i], stateName, 128) == 0) {
      _currentState = _states[i];
    }
  }
}

const char* Apsis::Actor::Sprite::currentState() {
  return _currentState;
}

Apsis::Primitives::SpriteSheet* Apsis::Actor::Sprite::spriteSheet() {
  return _spriteSheet;
}

Apsis::Geometry::Rectangle Apsis::Actor::Sprite::position() {
  return _position;
}

void Apsis::Actor::Sprite::animate(const char* animationName) {
  // Look-up the animation
  for (unsigned int i = 0; i < _animations.size(); i++) {
    if (strncmp(_animations[i]->name, animationName, 128) == 0) {
      _currentAnimation = _animations[i];
      _currentFrame = 0;
      _frame = _currentAnimation->frames[_currentFrame];
      return;
    }
  }
}

void Apsis::Actor::Sprite::nextFrame() {
  _currentFrame += 1;
  if (_currentAnimation->frames.size() == 0) {
    _currentFrame = 0;
  }
  else {
    _currentFrame %= _currentAnimation->frames.size();
  }
  _frame = _currentAnimation->frames[_currentFrame];
  _currentTime = 0;
}

void Apsis::Actor::Sprite::textureCoordinates(double coords[4]) {
  coords[0] = _frame->textureCoordinates[0];
  coords[1] = _frame->textureCoordinates[1];
  coords[2] = _frame->textureCoordinates[2];
  coords[3] = _frame->textureCoordinates[3];
}

Apsis::Primitives::Sprite* Apsis::Actor::Sprite::sprite() {
  return _frame->sprite;
}

void Apsis::Actor::Sprite::update(double elapsed, Apsis::Agent::MapCollider* collider) {
  _currentTime += elapsed;
  if (_currentTime > 0.08) {
    nextFrame();
  }

  Apsis::Geometry::Point to;
  to.x = _position.x;
  to.y = _position.y;

  // update actor information based on the state
  if (strcmp(_currentState, "walk_up") == 0) {
    to.y += _moveRate * elapsed;
  }
  else if (strcmp(_currentState, "walk_down") == 0) {
    to.y -= _moveRate * elapsed;
  }
  else if (strcmp(_currentState, "walk_left") == 0) {
    to.x -= _moveRate * elapsed;
  }
  else if (strcmp(_currentState, "walk_right") == 0) {
    to.x += _moveRate * elapsed;
  }
  else {
    return;
  }

  collider->move(&_position, &to);

  _position.x = to.x;
  _position.y = to.y;
}
