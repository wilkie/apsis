#include "apsis/sprite/font.h"

#include "apsis/primitives/fragment_shader.h"
#include "apsis/primitives/vertex_shader.h"

#include "apsis/primitives/unlinked_program.h"
#include "apsis/primitives/program.h"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

#include <algorithm>
#include <fstream>

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

#include <stdlib.h>

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

std::vector<std::string> Apsis::Sprite::Font::_ids;
std::vector<Apsis::Sprite::Font*> Apsis::Sprite::Font::_fonts;

static FT_Library _library;

bool Apsis::Sprite::Font::_initialized = false;

void Apsis::Sprite::Font::_init() {
  if (_initialized) {
    return;
  }

  if (FT_Init_FreeType(&_library)) {
    throw "Sprite::Font Error: Cannot initialize font reader.";
  }

  _initialized = true;
}

Apsis::Sprite::Font::Font(const char* family)
  : _family(family),
    _texture(NULL),
    _vertices(NULL),
    _elements(NULL),
    _x(0),
    _y(0),
    _line_height(0) {
  _id = _fonts.size();

  _load();
}

unsigned int Apsis::Sprite::Font::id() const {
  return _id;
}

const Apsis::Sprite::Font& Apsis::Sprite::Font::get(unsigned int id) {
  return *_fonts[id];
}

const Apsis::Sprite::Font& Apsis::Sprite::Font::load(const char* family) {
  std::string str = std::string(family);

  std::vector<std::string>::iterator it = std::find(_ids.begin(), _ids.end(), str);
  if (it != _ids.end()) {
    // already exists
    return *_fonts[std::distance(_ids.begin(), it)];
  }

  _fonts.push_back(new Apsis::Sprite::Font(family));
  _ids.push_back(str);
  return *_fonts[_ids.size() - 1];
}

const char* Apsis::Sprite::Font::family() const {
  return _family.c_str();
}

void Apsis::Sprite::Font::_load() {
  _init();

  float height = 16.0f;

  _internal = (void*)new FT_Face;
  FT_Face& face = *(FT_Face*)_internal;
  if (FT_New_Face(_library, _family.c_str(), 0, &face)) {
    throw "Sprite::Font Error: Cannot find font family on this system.";
  }

  _ascent  = (float)(face->ascender >> 6)
           + ((float)(face->ascender & 0x3f)  / (float)0x3f);
  _descent = (float)(face->descender >> 6)
           + ((float)(face->descender & 0x3f)  / (float)0x3f);
  _lineGap = (float)(face->height >> 6)
           + ((float)(face->height & 0x3f)  / (float)0x3f);

  // Our height is in pixels, this is in device points of 1/64 of a pixel.
  // Use dpi of 96
  // TODO: DON'T IGNORE FRACTION!! FT uses a 26.6 fixed point representation.
  unsigned int h = (unsigned int)height;
  FT_Set_Char_Size(face, h << 6, h << 6, 96, 96);

  // We can now either 1. Render characters to a bitmap or
  //                   2. Copy glyph data to a vertex buffer

  bool renderToBitmap = true;

  if (renderToBitmap) {
    if (_texture == NULL) {
      _texture = new Apsis::Primitives::Texture(512, 512);

      Primitives::VertexShader   vs = Primitives::VertexShader::fromFile("src/shaders/vertex/position.glsl");
      Primitives::FragmentShader fs = Primitives::FragmentShader::fromFile("src/shaders/fragment/color.glsl");

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
      _vao.defineUniform("color", program);

      _vao.defineUniform("tex", program);
      _vao.bindTexture(0, *_texture);
      _vao.uploadUniform("tex", 0);
      Primitives::Vector4 color = {0.0f, 0.3f, 0.0f, 1.0f};
      _vao.uploadUniform("color", color);
    }
  }
  else {
    // Get glyph data, pass to vertex buffer attached to GPU code
    // that will render it.

    // TODO: Read glyph data into vertex buffer
  }

  // Deallocate freetype info
  //FT_Done_Face(face);
}

void Apsis::Sprite::Font::_loadGlyphBitmap(unsigned int character) const {
  FT_Face& face = *(FT_Face*)_internal;

  if (FT_Load_Glyph(face, FT_Get_Char_Index(face, character), FT_LOAD_DEFAULT)) {
    throw "Sprite::Font Error: Glyph cannot be loaded.";
  }

  FT_Glyph glyph;
  if (FT_Get_Glyph(face->glyph, &glyph)) {
    throw "Sprite::Font Error: Glyph cannot be read.";
  }

  // Render glyph to a bitmap with FreeType
  FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
  FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
  const FT_Bitmap& bitmap = bitmapGlyph->bitmap;

  // Get glyph metrics
  FT_Glyph_Metrics& metrics = face->glyph->metrics;

  float glyph_width  = (float)bitmap.width;
  float glyph_height = (float)bitmap.rows;

  // TODO: Support simpler texture formats. We just need luminance/alpha
  //       texture here instead of full rgba8
  int* expanded = new int[(bitmap.rows+2) * (bitmap.width+2)];

  // Top/Bottom border
  for (unsigned int i = 0; i < ((unsigned int)bitmap.rows + 2); i+=bitmap.rows+1) {
    for (unsigned int j = 0; j < ((unsigned int)bitmap.width + 2); j++) {
      expanded[(i * (bitmap.width+2)) + j] = 0;
    }
  }

  // Left/Right border
  for (unsigned int j = 0; j < ((unsigned int)bitmap.width + 2); j+=bitmap.width+1) {
    for (unsigned int i = 0; i < ((unsigned int)bitmap.rows + 2); i++) {
      expanded[(i * (bitmap.width+2)) + j] = 0;
    }
  }

  // Bitmap Glyph
  for (unsigned int i = 0; i < (unsigned int)bitmap.rows; i++) {
    for (unsigned int j = 0; j < (unsigned int)bitmap.width; j++) {
      unsigned int comp = bitmap.buffer[(i * bitmap.width) + j];
      comp = 0xffffff | (comp << 24);
      expanded[((i+1) * (bitmap.width+2)) + (j+1)] = comp;
    }
  }

  // Copy glyph over

  if (_x + (unsigned int)bitmap.width + 2 > _texture->width()) {
    _x = 0;
    _y += _line_height;
  }

  _texture->blit(_x, _y, expanded, bitmap.width+2, bitmap.rows+2);

  delete[] expanded;

  Glyph glyphInfo;

  glyphInfo.codePoint = character;

  glyphInfo.width = glyph_width;
  glyphInfo.height = glyph_height;

  // TODO: vertical rendering hints
  glyphInfo.bearingLeft   = (float)(metrics.horiBearingX >> 6)
                          + ((float)(metrics.horiBearingX & 0x3f) / (float)0x3f);
  glyphInfo.bearingTop    = (float)(metrics.horiBearingY >> 6)
                          + ((float)(metrics.horiBearingY & 0x3f) / (float)0x3f);
  glyphInfo.advanceWidth  = (float)(metrics.horiAdvance >> 6)
                          + ((float)(metrics.horiAdvance & 0x3f)  / (float)0x3f);
  glyphInfo.advanceHeight = (float)(metrics.vertAdvance >> 6)
                          + ((float)(metrics.vertAdvance & 0x3f)  / (float)0x3f);

  _addGlyph(character, (float)_x + 1.0f, (float)_y + 1.0f, glyphInfo);

  _x += (unsigned int)bitmap.width + 2;

  if ((unsigned int)bitmap.rows + 2 > _line_height) {
    _line_height = (unsigned int)bitmap.rows + 2;
  }

  _vbo.transfer(_vertices, 5 * 4 * _bitmapGlyphs.size());
  _ebo.transfer(_elements, 6 * _bitmapGlyphs.size());

  _vao.bindElements(_ebo);
}

unsigned int Apsis::Sprite::Font::_addGlyph(char character,
                                            float x,
                                            float y,
                                            Glyph& glyph) const {
  if (_vertices == NULL) {
    unsigned int glyph_count = 100;

    unsigned int vertices_size = 4 * glyph_count;
    unsigned int elements_size = 6 * glyph_count;

    _elements = new unsigned int[elements_size];

    // 8 values for each logical vertex: 3 per axis coordinate,
    //                                   2 per texcoord
    _vertices = new float[5 * vertices_size];
  }

  // Add glyph
  unsigned int index = _bitmapGlyphs.size();
  
  unsigned int i = 4 * index;
  unsigned int ei = 6 * index;

  float w = glyph.width;
  float h = glyph.height;

  float coords[4];
  coords[0] = x / (float)_texture->width();
  coords[1] = y / (float)_texture->height();
  coords[2] = (w + x) / (float)_texture->width();
  coords[3] = (h + y) / (float)_texture->height();

  _vertices[i * 5 + 0] = 0.0f;
  _vertices[i * 5 + 1] = 0.0f;
  _vertices[i * 5 + 2] = 0.0f;

  _vertices[i * 5 + 3] = coords[0]; //textureCoords[i].x;
  _vertices[i * 5 + 4] = coords[1]; //textureCoords[i].y;

  i++;

  _vertices[i * 5 + 0] = 0.0f + w;
  _vertices[i * 5 + 1] = 0.0f;
  _vertices[i * 5 + 2] = 0.0f;

  _vertices[i * 5 + 3] = coords[2]; //textureCoords[i].x;
  _vertices[i * 5 + 4] = coords[1]; //textureCoords[i].y;

  i++;
      
  _vertices[i * 5 + 0] = 0.0f + w;
  _vertices[i * 5 + 1] = 0.0f;
  _vertices[i * 5 + 2] = 0.0f + h;

  _vertices[i * 5 + 3] = coords[2]; //textureCoords[i].x;
  _vertices[i * 5 + 4] = coords[3]; //textureCoords[i].y;

  i++;
      
  _vertices[i * 5 + 0] = 0.0f;
  _vertices[i * 5 + 1] = 0.0f;
  _vertices[i * 5 + 2] = 0.0f + h;

  _vertices[i * 5 + 3] = coords[0]; //textureCoords[i].x;
  _vertices[i * 5 + 4] = coords[3]; //textureCoords[i].y;

  i++;

  _elements[ei] = i-4; ei++;
  _elements[ei] = i-3; ei++;
  _elements[ei] = i-1; ei++;

  _elements[ei] = i-3; ei++;
  _elements[ei] = i-2; ei++;
  _elements[ei] = i-1; ei++;

  _bitmapGlyphs[character] = index;

  glyph.codePoint = (int)character;
  glyph.index = index;

  _glyphs.push_back(glyph);

  return index;
}

const Apsis::Sprite::Font::Glyph& Apsis::Sprite::Font::glyph(unsigned int codePoint) const {
  if (_bitmapGlyphs.count(codePoint) == 0) {
    _loadGlyphBitmap((char)codePoint);
  }

  return _glyphs[_bitmapGlyphs.at(codePoint)];
}

void Apsis::Sprite::Font::draw(const Primitives::Matrix& projection,
                               const Primitives::Camera& camera,
                               const Primitives::Vector4& color,
                               float x,
                               float y,
                               unsigned int index) const {
  _vao.bindProgram();

  _vao.uploadUniform("proj", projection);
  _vao.uploadUniform("view", camera.view());

  _vao.uploadUniform("camera", camera.eye());
  _vao.uploadUniform("color", color);

  const Glyph& glyph = _glyphs[index];

  glm::mat4 model = glm::translate(glm::mat4(1.0),
    glm::vec3(x + glyph.bearingLeft, 0.0, y - glyph.bearingTop));
  
  const Primitives::Matrix& model_matrix
    = *(const Primitives::Matrix*)glm::value_ptr(model);
  _vao.uploadUniform("model", model_matrix);
  _vao.drawRange(glyph.index * 6, 6);
}

void Apsis::Sprite::Font::draw(const Primitives::Matrix& projection,
                               const Primitives::Camera& camera,
                               const Primitives::Vector4& color,
                               float x,
                               float y,
                               const char* string) const {
  _vao.bindProgram();

  _vao.uploadUniform("proj", projection);
  _vao.uploadUniform("view", camera.view());
  
  _vao.uploadUniform("camera", camera.eye());
  _vao.uploadUniform("color", color);

  while(*string != NULL) {
    const Glyph& glyph = this->glyph(*string);

    glm::mat4 model = glm::translate(glm::mat4(1.0),
      glm::vec3(x + glyph.bearingLeft, 0.0, y - glyph.bearingTop));

    const Primitives::Matrix& model_matrix
      = *(const Primitives::Matrix*)glm::value_ptr(model);
    _vao.uploadUniform("model", model_matrix);
    _vao.drawRange(glyph.index * 6, 6);

    x += glyph.advanceWidth;
    string++;
  }
}

float Apsis::Sprite::Font::lineGap() const {
  return _lineGap;
}

float Apsis::Sprite::Font::ascent() const {
  return _ascent;
}

float Apsis::Sprite::Font::descent() const {
  return _descent;
}

float Apsis::Sprite::Font::width(const char* string) const {
  float x = 0;
  while(*string != NULL) {
    const Glyph& glyph = this->glyph(*string);
    x += glyph.advanceWidth;
    string++;
  }

  return x;
}

float Apsis::Sprite::Font::height(const char* string) const {
  return _lineGap;
}