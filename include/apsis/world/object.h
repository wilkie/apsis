#ifndef APSIS_WORLD_OBJECT_H
#define APSIS_WORLD_OBJECT_H

#include <apsis/registry/state.h>
#include <apsis/registry/property.h>

#include <apsis/world/value.h>

#include <set>
#include <vector>
#include <deque>
#include <unordered_map>

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
      const Apsis::World::Value& get(const char* key) const;

      /*
       *  Returns the value stored at the given property.
       */
      const Apsis::World::Value& get(unsigned int key) const;

      /*
       *  Returns true when the value at the given property is stored.
       */
      bool has(const char* key) const;

      /*
       *  Returns true when the value at the given property is stored.
       */
      bool has(unsigned int key) const;

      /*
       *  Sets the given property to the given value.
       */
      void set(unsigned int key, double value);

      /*
       *  Sets the given property to the given value.
       */
      void set(unsigned int key, long value);

      /*
       *  Sets the given property to the given value.
       */
      void set(unsigned int key, const char* string);

      /*
       *  Sets the given property to the given value.
       */
      void set(const char* key, double value);

      /*
       *  Sets the given property to the given value.
       */
      void set(const char* key, long value);

      /*
       *  Sets the given property to the given value.
       */
      void set(const char* key, const char* value);

      /*
       *  Returns whether or not the object has the given state.
       */
      bool isEnabled(unsigned int state) const;

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

      /*
       *  Queues the given Event by either its event id or the string name
       *  of the event.
       */
      void enqueueEvent(unsigned int event_id);
      void enqueueEvent(const char* event);
    private:
      // Property collection.
      std::unordered_map<unsigned int, Apsis::World::Value> _properties;

      // State collection.
      std::set<unsigned int> _states;

      // Event Queue
      std::deque<unsigned int> _events;
    };
  }
}

#endif