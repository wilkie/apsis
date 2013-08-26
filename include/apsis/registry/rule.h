#ifndef APSIS_REGISTRY_RULE_H
#define APSIS_REGISTRY_RULE_H

#include "apsis/agent/impeder.h"
#include "apsis/agent/mover.h"
#include "apsis/agent/act_function.h"

#include <vector>
#include <string>
#include <map>

#include <json/json.h>

namespace Apsis {
  namespace Registry {
    /*
     *  Contains a registry of all properties in the system. A property is a
     *  key of which an object can have a value. For instance, a common
     *  property for an object to have would be "hitPoints" which contains the
     *  health of the actor. Events can trigger rules that will then subtract
     *  or add to this value for an object.
     */
    class Rule {
    public:
      // TODO: isInherited() function? (should exist for other objects)

      /*
       *  Loads a Rule from the rule info represented by the given file.
       */
      static const Apsis::Registry::Rule& load(const char* path);

      /*
       *  Returns the unique id for this rule.
       */
      unsigned int id() const;

      /*
       *  Returns the name of this rule.
       */
      const char* name() const;

      /*
       *  Returns the path of the rule description, if used. Will return ""
       *  when such a description was not used.
       */
      const char* path() const;

      /*
       *  Registers or returns the existing id for the given property.
       */
      static unsigned int id(const char* name);

      /*
       *  Returns true when the given rule, specified by name, is superceded
       *  by this rule. Returns false otherwise.
       */
      bool supercedes(const char* rule) const;

      /*
       *  Returns the number of subrules.
       */
      unsigned int ruleCount() const;

      /*
       *  Returns the subrule at the given index.
       */
      const Apsis::Registry::Rule& rule(unsigned int id) const;

      /*
       *  Returns the number of collide functions.
       */
      unsigned int collideFunctionCount() const;

      /*
       *  Returns the collide function at the given index.
       */
      Apsis::Agent::CollideFunction collideFunction(unsigned int id) const;

      /*
       *  Returns the number of update functions.
       */
      unsigned int updateFunctionCount() const;

      /*
       *  Returns the update function at the given index.
       */
      Apsis::Agent::UpdateFunction updateFunction(unsigned int id) const;

      /*
       *  Returns a name of rules that this rule replaces when
       *  the rule actively updates with the given index.
       */
      const char* replacement(unsigned int id) const;

      /*
       *  Returns the number of names of rules that this rule will
       *  replace when the rule actively updates.
       */
      unsigned int replacementCount() const;

      unsigned int actFunctionCount() const;

      Apsis::Agent::ActFunction actFunction(unsigned int id) const;

      unsigned int actionId(unsigned int id) const;

    private:
      // Global list of rules
      static std::vector<std::string>            _ids;
      static std::vector<Apsis::Registry::Rule*> _all_rules;

      // Internal rules
      static std::map<std::string, Apsis::Agent::UpdateFunction>  _internal_updates;
      static std::map<std::string, Apsis::Agent::CollideFunction> _internal_collides;
      static std::map<std::string, std::string>                   _internal_act_actions;
      static std::map<std::string, Apsis::Agent::ActFunction>     _internal_acts;

      // Constructors
      Rule(const char* path);

      // Whether the json value exists
      bool _jsonLoaded;

      // Opens the JSON description file
      void _openJSONFile();

      // Parse JSON from an open description file
      void _parseJSONFile();

      // JSON value. This exists when _jsonLoaded is true.
      Json::Value _value;

      // The name of the rule.
      std::string _name;

      // The path of the rule description.
      std::string _path;

      // All of the rules that are subrules.
      std::vector<Apsis::Registry::Rule*> _rules;

      // The names of rules this rule supercedes
      std::vector<std::string> _supercedes;

      // All of the collider and updater functions that make up this rule.
      // (Does not include ones included in subrules)
      std::vector<Apsis::Agent::CollideFunction> _collides;
      std::vector<Apsis::Agent::UpdateFunction>  _updates;
      std::vector<Apsis::Agent::ActFunction>     _acts;
      std::vector<unsigned int>                  _action_ids;

      // The inherited rule, if there is one. Is NULL when it does not exist.
      const Apsis::Registry::Rule* _inherited;
    };
  }
}

#endif