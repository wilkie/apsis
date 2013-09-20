#ifndef APSIS_REGISTRY_WIDGET_H
#define APSIS_REGISTRY_WIDGET_H

#include "apsis/interface/event.h"
#include "apsis/engine/object.h"

#include <json/json.h>

#include <vector>
#include <string>

namespace Apsis {
  namespace Registry {
    class Widget {
    public:
      /*
       *  Loads a widget from the given JSON description or returns an already
       *  loaded copy.
       */
      static const Registry::Widget& load(const char* path,
                                          const Engine::Object& loader);

      /*
       *  Returns the initialization function for this widget.
       */
      Interface::Event::Init& initEvent() const;

      /*
       *  Returns the drawing function for this widget.
       */
      Interface::Event::Draw& drawEvent() const;

      /*
       *  Returns the update function for this widget.
       */
      Interface::Event::Update& updateEvent() const;

      /*
       *  Returns the leave function for this widget.
       */
      Interface::Event::Leave& leaveEvent() const;

      /*
       *  Returns the enter function for this widget.
       */
      Interface::Event::Enter& enterEvent() const;

      /*
       *  Returns the motion event for this widget.
       */
      Interface::Event::Motion& motionEvent() const;

      /*
       *  Returns the input event for this widget.
       */
      Interface::Event::Input& inputEvent() const;

      /*
       *  Returns the number of properties defined for this widget.
       */
      unsigned int propertyCount() const;

      /*
       *  Returns the name of the property by the given index.
       */
      const char* propertyName(unsigned int index) const;

      /*
       *  Returns the default value of the property by the given index.
       */
      const char* propertyDefault(unsigned int index) const;

      /*
       *  Returns the name of this widget.
       */
      const char* name() const;

    private:
      // Constructors
      Widget(const char* path,
             const Engine::Object& loader);

      // Keeps track of Interfaces system-wide.
      static std::vector<std::string> _ids;
      static std::vector<Registry::Widget*> _interfaces;

      // The path to the thing description.
      std::string _path;

      // Parses the given json via the path given in jsonFile.
      void _openJSONFile();

      // Whether or not the JSON has been loaded
      bool _jsonLoaded;

      // Parse JSON
      void _parseJSONFile();

      // JSON value;
      Json::Value _value;

      // Name
      std::string _name;

      // Internal functions
      static std::map<std::string, Interface::Event::Init*>   _internal_inits;
      static std::map<std::string, Interface::Event::Draw*>   _internal_draws;
      static std::map<std::string, Interface::Event::Input*>  _internal_inputs;
      static std::map<std::string, Interface::Event::Motion*> _internal_motions;
      static std::map<std::string, Interface::Event::Enter*>  _internal_enters;
      static std::map<std::string, Interface::Event::Leave*>  _internal_leaves;
      static std::map<std::string, Interface::Event::Update*> _internal_updates;

      // Pull out functions
      Interface::Event::Init&   _getInitFunction();
      Interface::Event::Draw&   _getDrawFunction();
      Interface::Event::Input&  _getInputFunction();
      Interface::Event::Motion& _getMotionFunction();
      Interface::Event::Enter&  _getEnterFunction();
      Interface::Event::Leave&  _getLeaveFunction();
      Interface::Event::Update& _getUpdateFunction();

      // Properties
      std::vector<std::string> _properties;
      std::vector<std::string> _types;
      std::vector<std::string> _values;

      // Event functions
      Interface::Event::Init&   _init;
      Interface::Event::Draw&   _draw;
      Interface::Event::Input&  _input;
      Interface::Event::Motion& _motion;
      Interface::Event::Enter&  _enter;
      Interface::Event::Leave&  _leave_;
      Interface::Event::Update& _update;
    };
  }
}

#endif