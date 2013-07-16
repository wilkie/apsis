#ifndef APSIS_WORLD_ACTOR_H
#define APSIS_WORLD_ACTOR_H

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
#include <apsis/agent/impeder.h>
#include <apsis/agent/mover.h>
#include <apsis/geometry/rectangle.h>

#include <vector>

namespace Apsis {
  namespace World {
    struct AnimationFrame {
      Apsis::Primitives::Sprite* sprite;
      float textureCoordinates[4];
    };

    struct Animation {
      char name[129];
      std::vector<AnimationFrame*> frames;
    };

    class Actor {
      public:
        /*
         *  Constructs a Apsis::Sprite using the actor files in the
         *    given filepath. The sprite used is defined by the given
         *    Apsis::Primitives::SpriteSheet.
         */
        Actor(const char* actorFile,
               unsigned int x,
               unsigned int y);
        ~Actor();

        /*
         *  Return: the Apsis::Primitives::SpriteSheet for the Apsis::Sprite.
         */
        Apsis::Primitives::SpriteSheet* spriteSheet();

        /*
         *  Return: the Apsis::Geometry::Rectangle for the Apsis::Sprite.
         */
        Apsis::Geometry::Rectangle position();

        /*
         *  Sets the current animation to be played by this Apsis::Sprite.
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
         *  Returns the Apsis::Primitives::Sprite for the current sprite for this
         *    Apsis::Sprite.
         */
        Apsis::Primitives::Sprite* sprite();

        /*
         *  Updates the current time for the Sprite. Affects animations and movements.
         */
        void update(double elapsed);

        void move(Apsis::Geometry::Point& to);

        // Set the current state for the Actor.
        void state(const char* stateName);

        // Return the current state for the Actor.
        const char* state();

        // Add the given agent that will provide movement to the actor.
        void attachMover(Apsis::Agent::Mover& agent);

        // Add the given agent that will alter intended movement.
        void attachImpeder(Apsis::Agent::Impeder& agent);

        // Draws the actor
        void draw(glm::mat4& projection,
                  Primitives::Camera& camera,
                  glm::mat4& model);

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
        const char* _state;

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

        // Before Move Agents
        std::vector<Apsis::Agent::Impeder> _impederAgents;
        std::vector<Apsis::Agent::Mover> _moverAgents;
    };
  }
}

#endif
