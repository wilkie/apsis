#include "apsis/registry/interface.h"

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
                               const Engine::Object& loader)
  : _loader(loader) {
  _id = _ids.size();

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

  _spans.push_back(0);
  if (value.isMember("widgets")) {
    // Load widgets
    _parseWidgets(value, loader);
  }
}

void Registry::Interface::_parseWidgets(Json::Value& value,
                                        const Engine::Object& loader) {
  unsigned int index = _spans.size() - 1;
  for (Json::Value::iterator it = value["widgets"].begin();
        it != value["widgets"].end();
        ++it) {
    if ((*it).isObject()) {
      if ((*it).isMember("widget")) {
        _spans[index]++;
        const char* widget_name = (*it)["widget"].asCString();

        const Registry::Widget& widget = loader.loadWidget(widget_name);
        Apsis::World::Object& object = *(new Apsis::World::Object);

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

        // Push to tree
        _widgets.push_back(&widget);
        _objects.push_back(&object);

        // Child windows
        _spans.push_back(0);
        if ((*it).isMember("widgets")) {
          _parseWidgets(*it, loader);
        }
      }
    }
    else {
      throw "Interface file's 'widgets' section is malformed.";
    }
  }
}

void Registry::Interface::_buildWidgets(Apsis::Interface::Window* parent,
                                        unsigned int span_index,
                                        unsigned int node_index,
                                        unsigned int& number_spans,
                                        unsigned int& number_nodes) const {
  // traverse tree
  unsigned int children = _spans[span_index];
  span_index++;

  number_spans = 1;
  number_nodes = children;

  for (unsigned int i = 0; i < children; i++) {
    const Apsis::Registry::Widget& widget = *_widgets[node_index];
    const Apsis::World::Object&    object = *_objects[node_index];

    node_index++;

    float x = 0.0f;
    float y = 0.0f;
    float w = 10.0f;
    float h = 10.0f;

    if (object.has("x")) {
      x = (float)object.get("x").asDouble();
    }

    if (object.has("y")) {
      y = (float)object.get("y").asDouble();
    }

    if (object.has("width")) {
      w = (float)object.get("width").asDouble();
    }

    if (object.has("height")) {
      h = (float)object.get("height").asDouble();
    }

    // Create the window
    Apsis::Interface::Window* window = new Apsis::Interface::Window(widget,
                                                                    object,
                                                                    _loader,
                                                                    x,
                                                                    y,
                                                                    w,
                                                                    h);

    parent->add(*window);

    unsigned int n_spans = 0;
    unsigned int n_nodes = 0;

    _buildWidgets(window,
                  span_index,
                  node_index,
                  n_spans,
                  n_nodes);

    number_spans += n_spans;
    number_nodes += n_nodes;

    node_index += n_nodes;
    span_index += n_spans;
  }
}

Apsis::Interface::Window* Registry::Interface::instance(float width,
                                                        float height) const {
  Apsis::Interface::Window* ret
    = new Apsis::Interface::Window(_loader,
                                   width  / 2.0f,
                                   height / 2.0f,
                                   width,
                                   height,
                                   Apsis::Interface::Event::defaultInit,
                                   Apsis::Interface::Event::defaultDraw,
                                   Apsis::Interface::Event::defaultInput,
                                   Apsis::Interface::Event::defaultMotion,
                                   Apsis::Interface::Event::defaultUpdate,
                                   Apsis::Interface::Event::defaultEnter,
                                   Apsis::Interface::Event::defaultLeave);

  Apsis::Interface::Window& current = *ret;

  unsigned int n_nodes, n_spans;
  _buildWidgets(ret, 0, 0, n_spans, n_nodes);

  return ret;
}

unsigned int Registry::Interface::id() const {
  return _id;
}