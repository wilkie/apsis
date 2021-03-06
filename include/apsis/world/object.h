#ifndef APSIS_WORLD_OBJECT_H
#define APSIS_WORLD_OBJECT_H

#include "apsis/registry/state.h"
#include "apsis/registry/property.h"

#include "apsis/world/value.h"

#include "apsis/world/collision_object.h"

#include <set>
#include <map>
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
       *  Returns the value stored at the given property.
       */
      const Apsis::World::Value& getForEvent(unsigned int event_id,
                                             const char* key) const;

      /*
       *  Returns the value stored at the given property.
       */
      const Apsis::World::Value& getForEvent(unsigned int event_id,
                                             unsigned int key) const;

      /*
       *  Returns true when the value at the given property is stored.
       */
      bool hasForEvent(unsigned int event_id,
                       const char* key) const;

      /*
       *  Returns true when the value at the given property is stored.
       */
      bool hasForEvent(unsigned int event_id,
               unsigned int key) const;

      /*
       *  Sets the given property to the given value.
       */
      void setForEvent(unsigned int event_id,
                       unsigned int key,
                       double value);

      /*
       *  Sets the given property to the given value.
       */
      void setForEvent(unsigned int event_id,
                       unsigned int key,
                       long value);

      /*
       *  Sets the given property to the given value.
       */
      void setForEvent(unsigned int event_id,
                       unsigned int key,
                       const char* string);

      /*
       *  Sets the given property to the given value.
       */
      void setForEvent(unsigned int event_id,
                       const char* key,
                       double value);

      /*
       *  Sets the given property to the given value.
       */
      void setForEvent(unsigned int event_id,
                       const char* key,
                       long value);

      /*
       *  Sets the given property to the given value.
       */
      void setForEvent(unsigned int event_id,
                       const char* key,
                       const char* value);

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

      /*
       *  Adds the given event as something this Object responds to by either
       *  its event id or the string name of the event.
       */
      void respondTo(unsigned int event_id);
      void respondTo(const char* event);

      /*
       *  Stops responding to the given event by either its event id or the
       *  string name of the event.
       */
      void ignore(unsigned int event_id);
      void ignore(const char* event);

      /*
       *  Returns whether or not this Object responds to the given event by
       *  either its event id or the string name of the event.
       */
      bool respondsTo(unsigned int event_id) const;
      bool respondsTo(const char* event) const;

      /*
       *  Dequeues the next event that the Object must respond to.
       */
      unsigned int dequeueEvent();

      /*
       *  Returns whether or not the Object has pending events.
       */
      bool hasEvents() const;

      /*
       *  Returns true if the given Object is this Object.
       */
      bool isMe(const Apsis::World::Object& reference) const;

      /*
       *  Adds the given CollisionObject at the specified fraction of movement.
       */
      void addCollision(float period, Apsis::World::CollisionObject& collisionObject);

      /*
       *  Returns the CollisionObject at the specified index.
       */
      const Apsis::World::CollisionObject& collisionObject(unsigned int index) const;

      /*
       *  Returns the period during movement that the collision took place
       *  given by the specified index.
       */
      float collisionPeriod(unsigned int index) const;

      /*
       *  Returns the number of collisions.
       */
      unsigned int collisionCount() const;

      /*
       *  Clears the list of pending collisions.
       */
      void clearCollisions();

      /*
       *  Sets user data pointer.
       */
      void userData(void* value);

      /*
       *  Returns user data pointer.
       */
      const void* userData() const;
    private:
      // Property collection.
      std::unordered_map<unsigned int, Apsis::World::Value> _properties;

      // User data
      void* _user;

      // State collection.
      std::set<unsigned int> _states;

      // Event Queue
      std::deque<unsigned int> _events;

      // The events this object responds to.
      std::set<unsigned int> _responds_to;

      // Event property lists
      std::unordered_map<unsigned int, std::unordered_map<unsigned int, Apsis::World::Value> > _event_properties;

      // Collisions in the current frame
      std::map<float, Apsis::World::CollisionObject> _collisions;

      // Cache for collision lookup
      void _collisionCacheLookup(unsigned int index) const;
      mutable std::map<float, Apsis::World::CollisionObject>::const_iterator _collisions_iterator;
      mutable unsigned int _collisions_index;
    };
  }
}

#endif