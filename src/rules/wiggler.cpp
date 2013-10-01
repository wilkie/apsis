#include "apsis/rules/wiggler.h"

#include "apsis/registry/state.h"

#include <math.h>

bool Apsis::Rules::Wiggler::update(float elapsed,
                                   const Apsis::World::Scene& scene,
                                   Apsis::World::Object& object) {
  static unsigned int wiggle_amount_id    = Apsis::Registry::Property::id("wiggle_amount");
  static unsigned int wiggle_time_id      = Apsis::Registry::Property::id("wiggle_time");
  static unsigned int wiggle_travelled_id = Apsis::Registry::Property::id("wiggle_travelled");
  static unsigned int wiggle_direction_id = Apsis::Registry::Property::id("wiggle_direction");

  static unsigned int x_id = Apsis::Registry::Property::id("x");
  static unsigned int y_id = Apsis::Registry::Property::id("y");

  if (!object.has(wiggle_amount_id)) {
    object.set(wiggle_amount_id,    (double)20.0);
    object.set(wiggle_time_id,      (double)0.5);
    object.set(wiggle_travelled_id, (double)0.0);
    object.set(wiggle_direction_id, (double)0.0);
  }

  float amount = (float)object.get(wiggle_amount_id).asDouble();
  float time   = (float)object.get(wiggle_time_id).asDouble();

  float travel = elapsed * (amount / time);

  float travelled = (float)object.get(wiggle_travelled_id).asDouble();
  travelled += travel;

  float direction = (float)object.get(wiggle_direction_id).asDouble();

  if (travelled > amount) {
    travel = amount - (travelled - elapsed);
    travelled = 0;
    direction -= 3.1415296f;
    object.set(wiggle_direction_id, (double)direction);
  }

  float x = (float)object.get(x_id).asDouble();
  float y = (float)object.get(y_id).asDouble();

  x += sin(direction) * travel;
  y += cos(direction) * travel;

  object.set(x_id, (double)x);
  object.set(y_id, (double)y);

  object.set(wiggle_travelled_id, travelled);

  return true;
}
