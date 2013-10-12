#include "apsis/world/background.h"

#include "apsis/primitive/fragment_shader.h"
#include "apsis/primitive/vertex_shader.h"

#include "apsis/primitive/unlinked_program.h"
#include "apsis/primitive/program.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

Apsis::World::Background::Background(Apsis::Primitive::Texture* texture)
  : _texture(texture),
    _vbo(Primitive::VertexBuffer::Target::Data),
    _ebo(Primitive::VertexBuffer::Target::Elements) {

  _width  = _texture->width();
  _height = _texture->height();

  // Create buffer array:
  unsigned int vertices_size = 4;
  unsigned int elements_size = 6;
  _elements = new unsigned short[elements_size];

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

  // TODO: pass in loader
  const Registry::Program& program = Engine::Object::basic().loadProgram("basic");

  _vao.useProgram(program);
  _vbo.defineInput("position", program.program(), 3, Primitive::VertexBuffer::Type::Float, false, 5, 0);
  _vbo.defineInput("texcoord", program.program(), 2, Primitive::VertexBuffer::Type::Float, false, 5, 3);

  _vao.bindTexture(0, *_texture);
  _vao.uploadUniform("tex", 0);

  _vao.bind(_vbo);
  _vao.bind(_ebo);
}

/*
  *  Returns the width of the background.
  */
unsigned int Apsis::World::Background::width() const {
  return _width;
}

/*
  *  Returns the height of the background.
  */
unsigned int Apsis::World::Background::height() const {
  return _height;
}

/*
  *  Returns the Apsis::Primitive::Texture being used to draw the background.
  */
const Apsis::Primitive::Texture& Apsis::World::Background::texture() const {
  return *_texture;
}

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

/*
  *  Renders the background.
  */
void Apsis::World::Background::draw(const Primitive::Matrix& projection,
                                    const World::Camera& camera) const {
  _vao.uploadUniform("projection", projection);
  _vao.uploadUniform("view", camera.view());

  for (unsigned int w = 0; w < 20; w++) {
    for (unsigned int h = 0; h < 20; h++) {
      glm::mat4 current_model = glm::translate(glm::mat4(1.0),
                                               glm::vec3(_width * w, 0.0, _height * h));

      const Primitive::Matrix& model_matrix
        = *(const Primitive::Matrix*)glm::value_ptr(current_model);

      _vao.uploadUniform("model", model_matrix);
      _vao.draw();
    }
  }
}
