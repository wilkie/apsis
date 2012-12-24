#include "iso-tasty/model/mesh.h"

#include "iso-tasty/primitives/fragment_shader.h"
#include "iso-tasty/primitives/vertex_shader.h"

#include "iso-tasty/primitives/unlinked_program.h"
#include "iso-tasty/primitives/program.h"

IsoTasty::Model::Mesh::Mesh(std::vector<glm::vec3>& vertices,
                            std::vector<glm::vec3>& normals,
                            std::vector<glm::vec2>& textureCoords,
                            std::vector<unsigned int>& elements) {
  // Create buffer array
  float* vert_array = new float[8 * vertices.size()];

  for (unsigned int i = 0; i < vertices.size(); i++) {
    vert_array[i * 8 + 0] = vertices[i].x;
    vert_array[i * 8 + 1] = vertices[i].y;
    vert_array[i * 8 + 2] = vertices[i].z;

    vert_array[i * 8 + 3] = normals[i].x;
    vert_array[i * 8 + 4] = normals[i].y;
    vert_array[i * 8 + 5] = normals[i].z;
    
    vert_array[i * 8 + 6] = textureCoords[i].x;
    vert_array[i * 8 + 7] = textureCoords[i].y;
  }
  _vbo.transfer(vert_array, 8 * vertices.size());

  delete [] vert_array;

  unsigned int * element_array = new unsigned int[elements.size()];
  for (unsigned int i = 0; i < elements.size(); i++) {
    element_array[i] = elements[i];
  }
  _ebo.transfer(element_array, elements.size());

  delete [] element_array;

  _vao.bindElements(_ebo);

  Primitives::VertexShader   vs = Primitives::VertexShader::fromFile("../../src/shaders/vertex/position.glsl");
  Primitives::FragmentShader fs = Primitives::FragmentShader::fromFile("../../src/shaders/fragment/colorize.glsl");

  Primitives::UnlinkedProgram unlinked;
  unlinked.attach(vs);
  unlinked.attach(fs);
  unlinked.defineFragmentOutput("outColor");
  Primitives::Program program = unlinked.link();

  _vao.useProgram(program);
  program.defineInput("position", _vbo, 3, Primitives::Program::Type::Float, false, 8, 0);
  //program.defineInput("normal",   _vbo, 3, Primitives::Program::Type::Float, false, 8, 3);
  //program.defineInput("texcoord", _vbo, 2, Primitives::Program::Type::Float, false, 8, 6);
  _vao.defineUniform("model", program);
  _vao.defineUniform("view",  program);
  _vao.defineUniform("proj",  program);

  Primitives::Texture t = Primitives::Texture("../../resources/sample.png");
  _vao.defineUniform("tex", program);
  _vao.bindTexture(0, t);
  _vao.uploadUniform("tex", 0);
}

IsoTasty::Model::Mesh::~Mesh() {
}

void IsoTasty::Model::Mesh::draw(glm::mat4& projection, glm::mat4& view, glm::mat4& model) {
  _vao.uploadUniform("proj", projection);
  _vao.uploadUniform("view", view);
  _vao.uploadUniform("model", model);
  _vao.draw();
}