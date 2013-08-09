#ifndef APSIS_WORLD_OBJECT_H
#define APSIS_WORLD_OBJECT_H

#include <apsis/registry/state.h>
#include <apsis/registry/property.h>

#include <apsis/world/value.h>

#include <set>
#include <vector>

namespace Apsis {
  namespace World {
    /*
     * A representation of a stateful object in the world.
     */
    class Object {
    public:
      /*
       *  Returns the value stored at the given property.
       */
      Apsis::World::Value get(const char* key);

      /*
       *  Returns the value stored at the given property.
       */
      Apsis::World::Value get(unsigned int key);

      /*
       *  Sets the given property to the given value.
       */
      void set(unsigned int key, Apsis::World::Value& value);

      /*
       *  Returns whether or not the object has the given state.
       */
      bool isEnabled(unsigned int state);

      /*
       *  Applies the given state to this object.
       */
      void enableState(unsigned int state);

      /*
       *  Removes the given state from this object.
       */
      void disableState(unsigned int state);

      /*
       *  Toggles the given state for this object. Returns the new
       *  state
       */
      bool toggleState(unsigned int state);

      // State collection.
      std::set<unsigned int>       _states;
    private:
      // Property collection.
      std::vector<Apsis::World::Value> _properties;
      // Whether or not a value has been set
      std::vector<bool> _hasValue;
    };
  }
}

#endif