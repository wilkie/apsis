#include "apsis/agent/rules/right.h"

#include "apsis/registry/action.h"

bool Apsis::Agent::Rules::Right::update(float elapsed,
                                        const Apsis::World::Scene& scene,
                                        const unsigned int objectId,
                                        Apsis::World::Object& object) {

  static unsigned int x_id     = Apsis::Registry::Property::id("x");
  static unsigned int speed_id = Apsis::Registry::Property::id("right_speed");
  static unsigned int right_id = Apsis::Registry::State::id("moving_right");

  float x = (float)object.get(x_id).asDouble();

  if (!object.has(speed_id)) {
    object.set(speed_id, 70.0f);
  }

  if (object.isEnabled(right_id)) {
    x += elapsed * (float)object.get(speed_id).asDouble();
    object.set(x_id, x);
  }

  return false;
}

std::string Apsis::Agent::Rules::Right::action = "right";

bool Apsis::Agent::Rules::Right::act(unsigned int action_id,
                                     bool held,
                                     const Apsis::World::Scene& scene,
                                     const unsigned int objectId,
                                     Apsis::World::Object& object) {
  static unsigned int right_action_id = Apsis::Registry::Action::id("right").id();
  static unsigned int right_id = Apsis::Registry::State::id("moving_right");

  if (action_id == right_action_id) {
    if (held) {
      object.enableState(right_id);
    }
    else {
      object.disableState(right_id);
    }
    return true;
  }

  return false;
}