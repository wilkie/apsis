#include "apsis/registry/interface.h"

#include "apsis/interface/window.h"

#include <algorithm>
#include <fstream>

using namespace Apsis;

std::vector<std::string> Registry::Interface::_ids;
std::vector<Registry::Interface*> Registry::Interface::_interfaces;

const Registry::Interface&
Registry::Interface::load(const char* path,
                          const Engine::Object& loader) {
  std::string str = std::string(path);

  std::vector<std::string>::iterator it = std::find(_ids.begin(), _ids.end(), str);
  if (it != _ids.end()) {
    // already exists
    return *_interfaces[std::distance(_ids.begin(), it)];
  }

  _interfaces.push_back(new Registry::Interface(path, loader));
  _ids.push_back(str);
  return *_interfaces[_ids.size() - 1];
}

Registry::Interface::Interface(const char* path,
                               const Engine::Object& loader) {
  Json::Reader reader;
  Json::Value  value;

  std::ifstream file(path);
  reader.parse(file, value);
  file.close();

  if (value.isMember("type") &&
      value["type"].compare(Json::Value("interface")) == 0) {
  }
  else {
    throw "Interface file given is not of type 'interface'.";
  }

  if (value.isMember("inherit")) {
    // TODO: inherit interfaces
  }

  if (value.isMember("widgets")) {
    // Load widgets
    for (Json::Value::iterator it = value["widgets"].begin();
         it != value["widgets"].end();
         ++it) {
      if ((*it).isObject()) {
        if ((*it).isMember("widget")) {
          const char* widget_name = (*it)["widget"].asCString();

          const Registry::Widget& widget = loader.loadWidget(widget_name);
          Apsis::World::Object object;

          if ((*it).isMember("properties")) {
            Json::Value::Members members = (*it)["properties"].getMemberNames();
            for (Json::Value::Members::iterator property_it = members.begin();
                 property_it != members.end();
                 ++property_it) {
              const char* name = (*property_it).c_str();
              unsigned int property_id = Apsis::Registry::Property::id(name);

              if ((*it)["properties"][name].isDouble()) {
                object.set(property_id, (*it)["properties"][name].asDouble());
              }
              else if ((*it)["properties"][name].isString()) {
                object.set(property_id, (*it)["properties"][name].asCString());
              }
              else if ((*it)["properties"][name].isIntegral()) {
                object.set(property_id, (long)(*it)["properties"][name].asInt());
              }
            }
          }

          // Defaults
          for (unsigned int i = 0; i < widget.propertyCount(); i++) {
            const char* name = widget.propertyName(i);
            const char* def  = widget.propertyDefault(i);

            if (!object.has(name)) {
              object.set(name, def);
            }
          }

          Apsis::Interface::Window& window = Apsis::Interface::Window(widget,
                                                                      object,
                                                                      0.0f,
                                                                      0.0f,
                                                                      0.0f,
                                                                      0.0f);
        }
      }
      else {
        throw "Interface file's 'widgets' section is malformed.";
      }
    }
  }
}