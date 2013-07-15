#include "apsis/world/map.h"

#include <stdlib.h>
#include <string.h>

#include "apsis/primitives/fragment_shader.h"
#include "apsis/primitives/vertex_shader.h"

#include "apsis/primitives/unlinked_program.h"
#include "apsis/primitives/program.h"

#include "apsis/model/material.h"
#include "apsis/model/light.h"

Apsis::World::Map::Map(unsigned int width,
                       unsigned int height,
                       Apsis::Primitives::SpriteSheet* spriteSheet) :
                   _width(width),
                   _height(height) {
  for(unsigned int x = 0; x < _width * _height; x++) {
    Apsis::World::Tile tile = Apsis::World::Tile();
    tile.spriteIndex(rand() % 16);
    tile.passable(true);

    _tiles.push_back(tile);
  }

  _generateWalls();

  //_tiles[9 + 6 * _width].passable = false;
  //_tiles[7 + 6 * _width].passable = false;
  //_tiles[8 + 4 * _width].passable = false;

  _spriteSheet = spriteSheet;

  
  // width = 3, height = 2 creates map like this:
  //
  //   +---+---+---+
  //   |   |   |   |
  //   +---+---+---+
  //   |   |   |   |
  //   +---+---+---+
  //
  // which split up into drawable pieces:
  //
  //   +---+   +---+   +---+
  //   |---| x |---| x |---|
  //   +---+   +---+   +---+
  //     x       x       x
  //   +---+   +---+   +---+  (24 vertices: 4 * width * height)
  //   |---| x |---| x |---|
  //   +---+   +---+   +---+  (2wh-w-h quads between tiles)

  // Create buffer array:
  unsigned int vertices_size = 4 * width * height;
  unsigned int elements_size = 6 * width * height;
  _elements = new unsigned int[elements_size];

  // 8 values for each logical vertex: 3 per axis coordinate,
  //                                   3 per normal,
  //                                   2 per texcoord
  this->_vertices = new float[8 * vertices_size];
  
  unsigned int i = 0;
  unsigned int ei = 0;
  for (unsigned int h = 0; h < height; h++) {
    for (unsigned int w = 0; w < width; w++) {
      _vertices[i * 8 + 0] = (float)w;
      _vertices[i * 8 + 1] = 0.0f;
      _vertices[i * 8 + 2] = (float)h;

      _vertices[i * 8 + 3] = 0.0;
      _vertices[i * 8 + 4] = 1.0;
      _vertices[i * 8 + 5] = 0.0;
    
      _vertices[i * 8 + 6] = 0; //textureCoords[i].x;
      _vertices[i * 8 + 7] = 0; //textureCoords[i].y;

      i++;

      _vertices[i * 8 + 0] = (float)w + 1.0f;
      _vertices[i * 8 + 1] = 0.0f;
      _vertices[i * 8 + 2] = (float)h;

      _vertices[i * 8 + 3] = 0.0;
      _vertices[i * 8 + 4] = 1.0;
      _vertices[i * 8 + 5] = 0.0;
    
      _vertices[i * 8 + 6] = 0; //textureCoords[i].x;
      _vertices[i * 8 + 7] = 0; //textureCoords[i].y;

      i++;
      
      _vertices[i * 8 + 0] = (float)w + 1.0f;
      _vertices[i * 8 + 1] = 0.0f;
      _vertices[i * 8 + 2] = (float)h + 1.0f;

      _vertices[i * 8 + 3] = 0.0;
      _vertices[i * 8 + 4] = 1.0;
      _vertices[i * 8 + 5] = 0.0;
    
      _vertices[i * 8 + 6] = 0; //textureCoords[i].x;
      _vertices[i * 8 + 7] = 0; //textureCoords[i].y;

      i++;
      
      _vertices[i * 8 + 0] = (float)w;
      _vertices[i * 8 + 1] = 0.0f;
      _vertices[i * 8 + 2] = (float)h + 1.0f;

      _vertices[i * 8 + 3] = 0.0;
      _vertices[i * 8 + 4] = 1.0;
      _vertices[i * 8 + 5] = 0.0;
    
      _vertices[i * 8 + 6] = 0; //textureCoords[i].x;
      _vertices[i * 8 + 7] = 0; //textureCoords[i].y;

      i++;

      _elements[ei] = i-4; ei++;
      _elements[ei] = i-3; ei++;
      _elements[ei] = i-1; ei++;
      
      _elements[ei] = i-3; ei++;
      _elements[ei] = i-2; ei++;
      _elements[ei] = i-1; ei++;
    }
  }

  _vbo.transfer(_vertices, 8 * vertices_size);
  _ebo.transfer(_elements, elements_size);

  _vao.bindElements(_ebo);

  Primitives::VertexShader   vs = Primitives::VertexShader::fromFile("src/shaders/vertex/position.glsl");
  Primitives::FragmentShader fs = Primitives::FragmentShader::fromFile("src/shaders/fragment/colorize.glsl");
  Primitives::FragmentShader ls = Primitives::FragmentShader::fromFile("src/shaders/fragment/directional_lighting.glsl");

  Primitives::UnlinkedProgram unlinked;
  unlinked.attach(vs);
  unlinked.attach(fs);
  unlinked.attach(ls);
  unlinked.defineFragmentOutput("outColor");
  Primitives::Program program = unlinked.link();

  _vao.useProgram(program);
  program.defineInput("position", _vbo, 3, Primitives::Program::Type::Float, false, 8, 0);
  program.defineInput("normal",   _vbo, 3, Primitives::Program::Type::Float, false, 8, 3);
  program.defineInput("texcoord", _vbo, 2, Primitives::Program::Type::Float, false, 8, 6);

  _vao.defineUniform("model", program);
  _vao.defineUniform("view",  program);
  _vao.defineUniform("proj",  program);
  _vao.defineUniform("camera",  program);

  _vao.defineUniform("material.ambient_intensity",  program);
  _vao.defineUniform("material.diffuse_intensity",  program);
  _vao.defineUniform("material.specular_intensity",  program);
  _vao.defineUniform("material.shininess",  program);
  _vao.defineUniform("material.emission",  program);
  _vao.defineUniform("light.color",  program);
  _vao.defineUniform("light.direction",  program);

  Primitives::Texture t = Primitives::Texture("resources/sample.png");
  _vao.defineUniform("tex", program);
  _vao.bindTexture(0, t);
  _vao.uploadUniform("tex", 0);

  Apsis::Model::Material material = Apsis::Model::Material(0.2f, 5.0f, 8.0f,
                                                           glm::vec3(1.0, 0.0, 0.0),
                                                           15.0f);

  Apsis::Model::Light light = Apsis::Model::Light(glm::vec3(0.0),
                                                  glm::vec3(0.0, 1.0, 0.0),
                                                  glm::vec3(1.0, 0.0, 1.0));

  _vao.uploadUniform("material.ambient_intensity", material.ambientIntensity());
  _vao.uploadUniform("material.diffuse_intensity", material.diffuseIntensity());
  _vao.uploadUniform("material.specular_intensity",  material.specularIntensity());
  _vao.uploadUniform("material.shininess",  material.shininess());
  _vao.uploadUniform("material.emission",  material.emission());

  _vao.uploadUniform("light.direction", light.direction());
  _vao.uploadUniform("light.color", light.color());
}

unsigned int Apsis::World::Map::width() {
  return _width;
}

unsigned int Apsis::World::Map::height() {
  return _height;
}

Apsis::World::Tile* Apsis::World::Map::tile(unsigned int x, unsigned int y) {
  return &_tiles[(y * _width) + x];
}

Apsis::Primitives::SpriteSheet* Apsis::World::Map::spriteSheet() {
  return _spriteSheet;
}

void Apsis::World::Map::_generateWalls() {
  for(unsigned int x = 0; x < _width * _height; x++) {
    if(rand() % 5 == 1) {
      _tiles[x].spriteIndex(16);
      _tiles[x].passable(false);
    }
  }
}

void Apsis::World::Map::draw(glm::mat4& projection,
                             Primitives::Camera& camera,
                             glm::mat4& model) {
/*  double coords[4];

  // Render all background tiles
  //renderer.depthTest(false);
  for (long y = this->height() - 1; y >= 0; y--) {
    for (long x = 0; x < (long)this->width(); x++) {
      Tile* tile = this->tile(x, y);
      if (!tile->passable()) {
        continue;
      }

      this->spriteSheet()->textureCoordinates(tile->spriteIndex(), coords);

      Apsis::Primitives::Sprite* sprite = this->spriteSheet()->sprite(tile->spriteIndex());
      /*renderer.drawSquare(x * 32.0, y * 32.0,
                          sprite->width, sprite->height,
                          coords[0], coords[1], coords[2], coords[3],
                          0);*
    }
  }*/
  _vao.uploadUniform("proj", projection);
  _vao.uploadUniform("view", camera.view());
  _vao.uploadUniform("model", model);

  _vao.uploadUniform("camera", camera.eye());
  _vao.draw();
}