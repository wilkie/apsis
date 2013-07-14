#ifndef APSIS_ACTOR_SPRITE_H
#define APSIS_ACTOR_SPRITE_H

// Actors: interact with worlds
// Sprite: 2d actor
//  Thing: 3d actor

// Actors have agents that decide how they affect the world, how world affects
// them.

// MapCollider - Collides with Map walls, impassible blocks
// SimplePhysics2d - Simple physics
// Physics2d   - 2d physics

// Agents are given a new position and an old position and some properties and
// decide where the actor actually moves and fires events.

#include <apsis/primitives/sprite_sheet.h>
#include <apsis/agent/map_collider.h>
#include <apsis/geometry/rectangle.h>

#include <vector>

namespace Apsis {
  namespace Actor {
    struct AnimationFrame {
      Apsis::Primitives::Sprite* sprite;
      double textureCoordinates[4];
    };

    struct Animation {
      char name[129];
      std::vector<AnimationFrame*> frames;
    };

    class Sprite {
      public:
        /*
         *  Constructs a Badger::Actor using the actor files in the
         *    given filepath. The sprite used is defined by the given
         *    Badger::SpriteSheet.
         */
        Sprite(const char* actorFile,
              unsigned int x,
              unsigned int y);
        ~Sprite();

        /*
         *  Return: the Badger::SpriteSheet for the Badger::Actor.
         */
        Apsis::Primitives::SpriteSheet* spriteSheet();

        /*
         *  Return: the Badger::Rectangle for the Badger::Actor.
         */
        Apsis::Geometry::Rectangle position();

        /*
         *  Sets the current animation to be played by this Badger::Actor.
         */
        void animate(const char* animationName);

        /*
         *  Advances the frame to the next sprite.
         */
        void nextFrame();

        /*
         *  Fills the given array with the texture coordinates of the current
         *    sprite.
         */
        void textureCoordinates(double coords[4]);

        /*
         *  Returns the Badger::Sprite for the current sprite for this
         *    Badger::Actor.
         */
        Apsis::Primitives::Sprite* sprite();

        /*
         *  Updates the current time for the Actor. Affects animations and movements.
         */
        void update(double elapsed, Apsis::Agent::MapCollider* collider);

        // Set the current state for the Actor.
        void setCurrentState(const char* stateName);

        // Return the current state for the Actor.
        const char* currentState();

      private:

        // The set of sprites for the Actor.
        Apsis::Primitives::SpriteSheet* _spriteSheet;

        // The current position of the Actor in the world.
        Apsis::Geometry::Rectangle _position;

        // Stores the current movement rate of the Actor.
        double _moveRate;

        // Stores all possible states for the Actor.
        std::vector<char*> _states;

        // Stores the current state of the character. State
        // determines how the character updates.
        const char* _currentState;

        // Stores the details about animations.
        std::vector<Animation*> _animations;

        // Stores the current animation.
        Animation* _currentAnimation;

        // Stores the current frame.
        unsigned int _currentFrame;
        AnimationFrame* _frame;

        // time since last frame.
        double _currentTime;

        // Creates a new animation structure.
        Animation* _newAnimation(const char* name);

        // Creates a new state.
        char* _newState(const char* name);
    };
  }
}

#endif
