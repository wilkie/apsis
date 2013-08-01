#include "apsis/world/background.h"

#include "apsis/primitives/fragment_shader.h"
#include "apsis/primitives/vertex_shader.h"

#include "apsis/primitives/unlinked_program.h"
#include "apsis/primitives/program.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

Apsis::World::Background::Background(Apsis::Primitives::Texture* texture)
  : _texture(texture) {

  _width  = _texture->width();
  _height = _texture->height();

  // Create buffer array:
  unsigned int vertices_size = 4;
  unsigned int elements_size = 6;
  _elements = new unsigned int[elements_size];

  // 8 values for each logical vertex: 3 per axis coordinate,
  //                                   2 per texcoord
  this->_vertices = new float[5 * vertices_size];

  unsigned int i = 0;
  unsigned int ei = 0;

  _vertices[i * 5 + 0] = -(float)_width;
  _vertices[i * 5 + 1] = 0.0f;
  _vertices[i * 5 + 2] = -(float)_height;

  _vertices[i * 5 + 3] = 0.0f;
  _vertices[i * 5 + 4] = 0.0f;

  i++;

  _vertices[i * 5 + 0] = (float)_width;
  _vertices[i * 5 + 1] = 0.0f;
  _vertices[i * 5 + 2] = -(float)_height;

  _vertices[i * 5 + 3] = 1.0f;
  _vertices[i * 5 + 4] = 0.0f;

  i++;

  _vertices[i * 5 + 0] = (float)_width;
  _vertices[i * 5 + 1] = 0.0f;
  _vertices[i * 5 + 2] = (float)_height;

  _vertices[i * 5 + 3] = 1.0f;
  _vertices[i * 5 + 4] = 1.0f;

  i++;

  _vertices[i * 5 + 0] = -(float)_width;
  _vertices[i * 5 + 1] = 0.0f;
  _vertices[i * 5 + 2] = (float)_height;

  _vertices[i * 5 + 3] = 0.0f;
  _vertices[i * 5 + 4] = 1.0f;

  i++;

  _elements[ei] = i-4; ei++;
  _elements[ei] = i-3; ei++;
  _elements[ei] = i-1; ei++;

  _elements[ei] = i-3; ei++;
  _elements[ei] = i-2; ei++;
  _elements[ei] = i-1; ei++;

  _vbo.transfer(_vertices, 5 * vertices_size);
  _ebo.transfer(_elements, elements_size);

  _vao.bindElements(_ebo);

  Primitives::VertexShader   vs = Primitives::VertexShader::fromFile("src/shaders/vertex/position.glsl");
  Primitives::FragmentShader fs = Primitives::FragmentShader::fromFile("src/shaders/fragment/flat.glsl");

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
  _vao.bindTexture(0, *_texture);
  _vao.uploadUniform("tex", 0);
}

/*
  *  Returns the width of the background.
  */
unsigned int Apsis::World::Background::width() {
  return _width;
}

/*
  *  Returns the height of the background.
  */
unsigned int Apsis::World::Background::height() {
  return _height;
}

/*
  *  Returns the Apsis::Primitives::Texture being used to draw the background.
  */
Apsis::Primitives::Texture* Apsis::World::Background::texture() {
  return _texture;
}

/*
  *  Renders the background.
  */
void Apsis::World::Background::draw(const glm::mat4& projection,
                                    Primitives::Camera& camera,
                                    const glm::mat4& model) {
  _vao.uploadUniform("proj", projection);
  _vao.uploadUniform("view", camera.view());
  _vao.bindTexture(0, *_texture);
  _vao.uploadUniform("camera", camera.eye());

  for (unsigned int w = 0; w < 20; w++) {
    for (unsigned int h = 0; h < 20; h++) {
      glm::mat4 current_model = glm::translate(glm::mat4(1.0),
                                               glm::vec3(_width * w, 0.0, _height * h));
      _vao.uploadUniform("model", current_model);
      _vao.draw();
    }
  }
}
