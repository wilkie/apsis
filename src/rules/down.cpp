#include "apsis/rules/down.h"

#include "apsis/registry/action.h"

bool Apsis::Rules::Down::update(float elapsed,
                                const Apsis::World::Scene& scene,
                                Apsis::World::Object& object) {

  static unsigned int y_id     = Apsis::Registry::Property::id("y");
  static unsigned int speed_id = Apsis::Registry::Property::id("down_speed");
  static unsigned int down_id  = Apsis::Registry::State::id("moving_down");

  float y = (float)object.get(y_id).asDouble();

  if (!object.has(speed_id)) {
    object.set(speed_id, 70.0f);
  }

  if (object.isEnabled(down_id)) {
    y += elapsed * (float)object.get(speed_id).asDouble();
    object.set(y_id, y);
  }

  return false;
}

const char* Apsis::Rules::Down::action = "down";

bool Apsis::Rules::Down::act(unsigned int action_id,
                             bool held,
                             const Apsis::World::Scene& scene,
                             Apsis::World::Object& object) {
  static unsigned int down_action_id = Apsis::Registry::Action::id("down").id();
  static unsigned int down_id = Apsis::Registry::State::id("moving_down");

  if (action_id == down_action_id) {
    if (held) {
      object.enableState(down_id);
    }
    else {
      object.disableState(down_id);
    }
    return true;
  }

  return false;
}