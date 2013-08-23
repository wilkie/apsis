#ifndef APSIS_REGISTRY_ACTION_H
#define APSIS_REGISTRY_ACTION_H

#include "apsis/input/binding.h"

#include <vector>
#include <string>

namespace Apsis {
  namespace Registry {
    /*
     *  Contains a registry of all actions that exist in the system. Actions
     *  are an input-triggered event. For instance, you may have an Action for
     *  "jump" that can be bound to a key. The rules in the game will respond
     *  to a held or triggered action.
     */
    class Action {
    public:
      // TODO: static methods that set various properties about keys.
      //       * Allow duplicates (Keep track of duplicates in an std::set)
      //       * Allow or disallow modifiers (shift+A can trigger an action)
      //         This is important when you have something mapped to SHIFT
      //         and something else mapped to CTRL+A... the system will get
      //         confused when it sees CTRL+SHIFT+A... although maybe it should
      //         figure this out, but it seems hard. Always trigger bindings
      //         that are solely modifiers? Trigger both somehow??

      // TODO: descriptions? i18n considerations?

      /*
       *  Loads a JSON bindings description that will load default bindings
       *  for a set of actions.
       */
      static void load(const char* path);

      /*
       *  Registers or returns the action for the given name.
       */
      static Apsis::Registry::Action& id(const char* name);

      /*
       *  Returns the Action by the given index.
       */
      static Apsis::Registry::Action& id(unsigned int id);

      /*
       *  Returns the number of actions in the system.
       */
      static unsigned int count();

      /*
       *  Yields the action triggered by the given key binding. Returns true
       *  when a binding yields an event. In this case, action_id will be set
       *  to the id of the action.
       */
      static bool yield(const Apsis::Input::Binding& binding,
                        unsigned int& action_id);

      /*
       *  Binds the given key binding as the primary means of activating this
       *  action.
       */
      void bindPrimary(Apsis::Input::Binding& binding);

      /*
       *  Binds the given key binding as the secondary means of activating this
       *  action.
       */
      void bindSecondary(Apsis::Input::Binding& binding);

      /*
       *  Returns the primary key binding for this action.
       */
      const Apsis::Input::Binding& primary() const;

      /*
       *  Returns the secondary key binding for this action. If there is no
       *  primary, this will actually set the primary binding.
       */
      const Apsis::Input::Binding& secondary() const;

      /*
       *  Returns the number of bindings that will trigger this action.
       */
      unsigned int bindingCount() const;

      /*
       *  Clears the primary binding for this action.
       */
      void clearPrimary();

      /*
       *  Clears the secondary binding for this action.
       */
      void clearSecondary();

      /*
       *  Clears all bindings for this action.
       */
      void clear();

      /*
       *  Returns the unique identifier for this Action within the system.
       */
      unsigned int id() const;

    private:
      // Global repository of all action ids.
      static std::vector<std::string> _ids;

      // Global repository of all actions and their bindings.
      static std::vector<Apsis::Registry::Action*> _actions;

      // JSON descriptions that have been loaded for bindings.
      static std::vector<std::string> _paths;

      // The unique id for this action within the system.
      unsigned int _id;

      // Bindings that are mapped to this action
      std::vector<Apsis::Input::Binding> _bindings;
    };
  }
}

#endif