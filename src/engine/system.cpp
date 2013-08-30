#include "apsis/engine/system.h"

#include "apsis/sync/clock.h"

#include <fstream>
#include <stdlib.h>

// TODO: Move draw and view management to a class
// ----------- GOES TO VIEWPORT ----------------- //

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

// --------- END GOES TO VIEWPORT --------------- //

Apsis::Engine::System::System(const char* path,
                              const Apsis::Settings::Video& videoSettings)
  : _path(path),                                         // Set path
    _videoSettings(videoSettings),                       // Copy video settings
    _backend(*(new Apsis::Backend::Sdl(videoSettings))), // Initialize Devices
    _jsonLoaded(false),                                  // JSON
    _input(_parseOrCreateInput()),                       // Initialize Input Engine
    _objects(_parseOrCreateObject()),                    // Initialize Object Engine
    _scene(NULL) {                                       // Initially have no active scene

  // Parse objects and system data and populate data and caches.
  _parseJSONFile();
}

const Apsis::Engine::Input& Apsis::Engine::System::inputEngine() const {
  return _input;
}

Apsis::Engine::Input& Apsis::Engine::System::inputEngine() {
  return _input;
}

const Apsis::Engine::Object& Apsis::Engine::System::objectEngine() const {
  return _objects;
}

void Apsis::Engine::System::_openJSONFile() {
  if (_jsonLoaded) {
    return;
  }

  Json::Reader reader;

  std::ifstream file(_path);
  reader.parse(file, _value);
  file.close();

  _jsonLoaded = true;

  if (_value.isMember("inherit")) {
    // TODO: inherit games
  }
}

const Apsis::Engine::Object& Apsis::Engine::System::_parseOrCreateObject() {
  _openJSONFile();

  if (!(_value.isMember("type") &&
        _value["type"].compare(Json::Value("system")) == 0)) {
    throw "System description file given does not of type 'system'.";
  }

  if (_value.isMember("paths")) {
    if (_value["paths"].isObject()) {
      // Embedded
      return Apsis::Engine::Object::load(_value["paths"]);
    }
    else if (_value["paths"].isString()) {
      // Specified
      return Apsis::Engine::Object::load(_value["paths"].asCString());
    }
  }

  // Default
  return Apsis::Engine::Object::default();
}

Apsis::Engine::Input& Apsis::Engine::System::_parseOrCreateInput() {
  _openJSONFile();

  // Default
  return Apsis::Engine::Input::default();
}

void Apsis::Engine::System::_parseJSONFile() {
  _openJSONFile();

  // Load Scenes
  if (_value.isMember("scenes")) {
    if (!_value["scenes"].isArray()) {
      throw "System description file's 'scenes' member is not an array of strings.";
    }

    for (Json::Value::iterator it = _value["scenes"].begin();
         it != _value["scenes"].end();
         ++it) {
      if ((*it).isString()) {
        const Apsis::Registry::Scene& scene
          = _objects.loadScene((*it).asCString());

        if (_scene == NULL) {
          _scene = new Apsis::World::Scene(scene);
        }
      }
      else {
        throw "A member of the system description file's 'scenes' is not a string.";
      }
    }
  }

  // Load key bindings
  if (_value.isMember("bindings")) {
    if (!_value["bindings"].isArray()) {
      throw "System description file's 'bindings' member is not an array of strings.";
    }

    for (Json::Value::iterator it = _value["bindings"].begin();
         it != _value["bindings"].end();
         ++it) {
      if ((*it).isString()) {
        _objects.loadBindings((*it).asCString());
      }
      else {
        throw "A member of the system description file's 'scenes' is not a string.";
      }
    }
  }
}

void Apsis::Engine::System::run() {
  Apsis::Clock clock;

  Apsis::Engine::Event core_event;

  if (_scene == NULL) {
    return;
  }

  unsigned int action_id = 0;
  while(true) {
    if (_backend.poll(core_event)) {
      if (core_event.type() == Apsis::Engine::Event::Type::Press) {
        if (_input.press(core_event.binding(), action_id)) {
          _scene->act(action_id, true);
        }
      }
      else if (core_event.type() == Apsis::Engine::Event::Type::Release) {
        if (_input.release(core_event.binding(), action_id)) {
          _scene->act(action_id, false);
        }
      }
      else if (core_event.type() == Apsis::Engine::Event::Type::SystemEvent) {
        if (core_event.systemEvent() == Apsis::Engine::Event::SystemEvent::Quit) {
          break;
        }
      }
    }
    _scene->update(clock.elapsedTime());

    // TODO: Obviously, there should be a viewport manager class...
    // -- GOES TO VIEWPORT -- //

    bool orthographic = true;

    float rotation = 0.0;

    float aspect = (float)_videoSettings.resolutionX / (float)_videoSettings.resolutionY;

    float nearf = 1;
    float farf = 20.0;

    float fov = 45.0f;

    float top = tanf(fov * 0.5f) * nearf;
    float bottom = -top;

    float left = aspect * bottom;
    float right = aspect * top;

    glm::mat4 projection;

    float half_height = _videoSettings.resolutionY/2.0f;
    float half_width  = _videoSettings.resolutionX/2.0f;

    if (orthographic) {
      projection = glm::ortho(-half_width, half_width, -half_height, half_height);
    }
    else {
      projection = glm::perspective(fov, aspect, nearf, farf);
    }

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    float x = half_width;
    float z = half_height;

    float zoom = 1.0f;

    Apsis::Primitives::Camera camera = Primitives::Camera(glm::vec2((float)(int)(x+0.5), (float)(int)(z+0.5)), zoom);

    _scene->draw(projection, camera, glm::mat4(1.0f));

    // END GOES TO VIEWPORT //

    _backend.swap();
  }

  _backend.quit();
}