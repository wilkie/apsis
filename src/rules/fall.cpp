#include "apsis/rules/fall.h"

#include "apsis/registry/state.h"

bool Apsis::Rules::Fall::update(float elapsed,
                                const Apsis::World::Scene& scene,
                                const unsigned int objectId,
                                Apsis::World::Object& object) {
  static unsigned int collideWithTopState = Apsis::Registry::State::id("collideWithTop");
  static unsigned int jumpingState        = Apsis::Registry::State::id("jumping");

  static unsigned int y_id     = Apsis::Registry::Property::id("y");
  static unsigned int speed_id = Apsis::Registry::Property::id("fall_starting_velocity");
  static unsigned int start_id = Apsis::Registry::Property::id("fall_starting_velocity");
  static unsigned int accel_id = Apsis::Registry::Property::id("fall_acceleration");
  static unsigned int termv_id = Apsis::Registry::Property::id("fall_terminal_velocity");

  float y = (float)object.get(y_id).asDouble();

  if (!object.has(start_id)) {
    object.set(start_id, 200.0f);
    object.set(speed_id, 200.0f);
    object.set(accel_id, 200.0f);
    object.set(termv_id, 512.0f);
  }

  float velocity = (float)object.get(speed_id).asDouble();

  if (object.isEnabled(collideWithTopState)) {
    velocity = (float)object.get(start_id).asDouble();
  }

  if (!object.isEnabled(jumpingState)) {
    object.disableState(collideWithTopState);

    float acceleration     = (float)object.get(accel_id).asDouble();
    float terminalVelocity = (float)object.get(termv_id).asDouble();

    velocity += acceleration * elapsed;
    if (velocity > terminalVelocity) {
      velocity = terminalVelocity;
    }

    y += elapsed * velocity;

    object.set(speed_id, velocity);
    object.set(y_id, y);

    return true;
  }
  else {
    object.set(speed_id, object.get(start_id).asDouble());
  }

  return false;
}