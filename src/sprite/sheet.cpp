#include "apsis/sprite/sheet.h"

#include "apsis/primitives/fragment_shader.h"
#include "apsis/primitives/vertex_shader.h"

#include "apsis/primitives/unlinked_program.h"
#include "apsis/primitives/program.h"

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

#include <string.h>

#include <math.h>
#include <SOIL.h>

#include <fstream>
#include <json/json.h>

#include <algorithm>

std::vector<std::string> Apsis::Sprite::Sheet::_ids;
std::vector<Apsis::Sprite::Sheet*> Apsis::Sprite::Sheet::_sheets;

const Apsis::Sprite::Sheet& Apsis::Sprite::Sheet::load(const char* name) {
  std::string str = std::string(name);

  std::vector<std::string>::iterator it = std::find(_ids.begin(), _ids.end(), str);
  if (it != _ids.end()) {
    // already exists
    return *_sheets[std::distance(_ids.begin(), it)];
  }

  printf("Loading sprite sheet %s\n", name);

  _ids.push_back(str);
  Apsis::Sprite::Sheet* sheet = new Apsis::Sprite::Sheet(name);
  _sheets.push_back(sheet);
  return *_sheets[_ids.size() - 1];
}

const Apsis::Sprite::Sheet& Apsis::Sprite::Sheet::loaded(unsigned int index) {
  return *_sheets[index];
}

Apsis::Sprite::Sheet::Sheet(const char* filename) {
  _id = _sheets.size();

  _texture = new Apsis::Primitives::Texture(filename);
  _width = _texture->width();
  _height = _texture->height();

  // Set GL texture options
  glBindTexture(GL_TEXTURE_2D, _texture->identifier());

  _loadStatSheet(filename);

  unsigned int sprite_count = this->count();

  unsigned int vertices_size = 4 * sprite_count;
  unsigned int elements_size = 6 * sprite_count;
  _elements = new unsigned int[elements_size];

  // 8 values for each logical vertex: 3 per axis coordinate,
  //                                   2 per texcoord
  this->_vertices = new float[5 * vertices_size];

  unsigned int i = 0;
  unsigned int ei = 0;
  unsigned int ti = 0;

  for (unsigned int si = 0; si < sprite_count; si++) {
    float coords[4];
    this->textureCoordinates(si, coords);
    Apsis::Sprite::Sheet::Sprite& sprite = _sprites[si];

    _vertices[i * 5 + 0] = -(float)sprite.center_x;
    _vertices[i * 5 + 1] = 0.0f;
    _vertices[i * 5 + 2] = -(float)sprite.center_y;

    _vertices[i * 5 + 3] = coords[0]; //textureCoords[i].x;
    _vertices[i * 5 + 4] = coords[1]; //textureCoords[i].y;

    i++;

    _vertices[i * 5 + 0] = -(float)sprite.center_x + (float)sprite.width;
    _vertices[i * 5 + 1] = 0.0f;
    _vertices[i * 5 + 2] = -(float)sprite.center_y;

    _vertices[i * 5 + 3] = coords[2]; //textureCoords[i].x;
    _vertices[i * 5 + 4] = coords[1]; //textureCoords[i].y;

    i++;

    _vertices[i * 5 + 0] = -(float)sprite.center_x + (float)sprite.width;
    _vertices[i * 5 + 1] = 0.0f;
    _vertices[i * 5 + 2] = -(float)sprite.center_y + (float)sprite.height;

    _vertices[i * 5 + 3] = coords[2]; //textureCoords[i].x;
    _vertices[i * 5 + 4] = coords[3]; //textureCoords[i].y;

    i++;

    _vertices[i * 5 + 0] = -(float)sprite.center_x;
    _vertices[i * 5 + 1] = 0.0f;
    _vertices[i * 5 + 2] = -(float)sprite.center_y + (float)sprite.height;

    _vertices[i * 5 + 3] = coords[0]; //textureCoords[i].x;
    _vertices[i * 5 + 4] = coords[3]; //textureCoords[i].y;

    i++;

    _elements[ei] = i-4; ei++;
    _elements[ei] = i-3; ei++;
    _elements[ei] = i-1; ei++;

    _elements[ei] = i-3; ei++;
    _elements[ei] = i-2; ei++;
    _elements[ei] = i-1; ei++;
  }

  _vbo.transfer(_vertices, 5 * vertices_size);
  _ebo.transfer(_elements, elements_size);

  _vao.bindElements(_ebo);

  Primitives::VertexShader   vs = Primitives::VertexShader::fromFile("assets/shaders/vertex/position.glsl");
  Primitives::FragmentShader fs = Primitives::FragmentShader::fromFile("assets/shaders/fragment/flat.glsl");

  Primitives::UnlinkedProgram unlinked;
  unlinked.attach(vs);
  unlinked.attach(fs);
  unlinked.defineFragmentOutput("outColor");
  Primitives::Program program = unlinked.link();

  _vao.useProgram(program);
  program.defineInput("position", _vbo, 3, Primitives::Program::Type::Float, false, 5, 0);
  program.defineInput("texcoord", _vbo, 2, Primitives::Program::Type::Float, false, 5, 3);

  _vao.defineUniform("model", program);
  _vao.defineUniform("view",  program);
  _vao.defineUniform("proj",  program);

  _vao.defineUniform("tex", program);
  _vao.bindTexture(0, this->texture());
  _vao.uploadUniform("tex", 0);
}

unsigned int Apsis::Sprite::Sheet::id() const {
  return _id;
}

char* Apsis::Sprite::Sheet::_determineStatSheetFilename(const char* filename) {
  char* stat_sheet = new char[strlen(filename)+3];
  strncpy(stat_sheet, filename, strlen(filename));

  for (unsigned int i = strlen(filename); i > 0; i--) {
    if (stat_sheet[i] == '.') {
      stat_sheet[i] = '\0';
    }
  }

  strcat(stat_sheet, ".json");

  return stat_sheet;
}

void Apsis::Sprite::Sheet::_loadStatSheet(const char* filename) {
  char* stat_sheet = _determineStatSheetFilename(filename);

  Json::Reader reader;
  Json::Value value;

  std::ifstream file(stat_sheet);
  reader.parse(file, value);
  file.close();

  // Sprite List
  // TODO: better handling of invalid values
  for (Json::Value::iterator it = value["frames"].begin(); it != value["frames"].end(); it++) {
    Sprite sprite;

    strcpy(sprite.name, (*it)["name"].asCString());
    sprite.x        = (unsigned int)(*it)["x"].asUInt();
    sprite.y        = (unsigned int)(*it)["y"].asUInt();
    sprite.width    = (unsigned int)(*it)["width"].asUInt();
    sprite.height   = (unsigned int)(*it)["height"].asUInt();
    sprite.center_x = (unsigned int)(*it)["center_x"].asUInt();
    sprite.center_y = (unsigned int)(*it)["center_y"].asUInt();

    _sprites.push_back(sprite);
  }

  delete [] stat_sheet;
}

const Apsis::Primitives::Texture& Apsis::Sprite::Sheet::texture() const {
  return *_texture;
}

void Apsis::Sprite::Sheet::textureCoordinates(unsigned int index, float coords[4]) const {
  const Sprite& sprite = _sprites[index];

  float tu = ((float)sprite.x + 0.1f)  / (float)_width;
  float tv = ((float)sprite.y + 0.1f)  / (float)_height;
  float tu2 = ((float)sprite.x - 0.1f + (float)sprite.width)  / (float)_width;
  float tv2 = ((float)sprite.y - 0.1f + (float)sprite.height)  / (float)_height;

  if (tu < 0.0)  { tu = 0.0; }
  if (tv < 0.0)  { tv = 0.0; }
  if (tu2 < 0.0) { tu2 = 0.0; }
  if (tv2 < 0.0) { tv2 = 0.0; }

  if (tu > 1.0)  { tu = 1.0; }
  if (tv > 1.0)  { tv = 1.0; }
  if (tu2 > 1.0) { tu2 = 1.0; }
  if (tv2 > 1.0) { tv2 = 1.0; }

  coords[0] = (float)tu;
  coords[1] = (float)tv;
  coords[2] = (float)tu2;
  coords[3] = (float)tv2;
}

bool Apsis::Sprite::Sheet::textureCoordinates(const char* name, float coords[4]) const {
  for (unsigned int i = 0; i < _sprites.size(); i++) {
    if (strncmp(name, _sprites[i].name, 64) == 0) {
      textureCoordinates(i, coords);
      return true;
    }
  }
  return false;
}

int Apsis::Sprite::Sheet::enumerateSprites(const char* wildcard, unsigned int last) const {
  int star_pos = -1;

  // Determine where the '*' is in the wildcard
  for (unsigned int i = 0; i < strlen(wildcard); i++) {
    if (wildcard[i] == '*') {
      star_pos = i;
      break;
    }
  }

  if (star_pos == -1) {
    // if we've already found the sprite, don't infinite loop
    if (last > 0) {
      return -1;
    }

    // No star is found... just find by the name
    for (unsigned int i = 0; i < _sprites.size(); i++) {
      if (strncmp(wildcard, _sprites[i].name, 64) == 0) {
        return i;
      }
    }
    return -1;
  }

  // Look for a wildcard match
  for (unsigned int i = last; i < _sprites.size(); i++) {
    if (strncmp(wildcard, _sprites[i].name, star_pos) == 0) {
      // Matches in the front
      return i;
    }
  }

  return -1;
}

unsigned int Apsis::Sprite::Sheet::count() const {
  return _sprites.size();
}

void Apsis::Sprite::Sheet::draw(unsigned int              index,
                                const Primitives::Matrix& projection,
                                const World::Camera& camera,
                                const Primitives::Matrix& model) const {
  _vao.bindProgram();

  _vao.uploadUniform("proj", projection);
  _vao.uploadUniform("view", camera.view());
  _vao.uploadUniform("model", model);

  _vao.drawRange(index * 6, 6);
}

float Apsis::Sprite::Sheet::width(unsigned int index) const {
  return (float)_sprites[index].width;
}

float Apsis::Sprite::Sheet::height(unsigned int index) const {
  return (float)_sprites[index].height;
}

float Apsis::Sprite::Sheet::left(unsigned int index) const {
  return (float)_sprites[index].x;
}

float Apsis::Sprite::Sheet::top(unsigned int index) const {
  return (float)_sprites[index].y;
}
