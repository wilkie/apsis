#include "apsis/world/terrain.h"

#include "apsis/primitives/fragment_shader.h"
#include "apsis/primitives/vertex_shader.h"

#include "apsis/primitives/unlinked_program.h"
#include "apsis/primitives/program.h"

#include "apsis/geometry/point3d.h"

// Helper function for controlPoints()
static Apsis::Geometry::Point3d* firstControlPoints(Apsis::Geometry::Point3d* rhs, Apsis::Geometry::Point3d* first, unsigned int num) {
  Apsis::Geometry::Point3d* tmp = new Apsis::Geometry::Point3d[num];

  float bx = 2.0f;
  float by = 2.0f;
  first[0].x = rhs[0].x / bx;
  first[0].y = rhs[0].y / by;
  for (unsigned int i = 1; i < num; i++) {
    tmp[i].x = 1.0f / bx;
    tmp[i].y = 1.0f / by;
    if (i < num - 1) {
      bx = 4.0f - tmp[i].x;
      by = 4.0f - tmp[i].y;
    }
    else {
      bx = 3.5f - tmp[i].x;
      by = 3.5f - tmp[i].y;
    }
    first[i].x = (rhs[i].x - first[i - 1].x) / bx;
    first[i].y = (rhs[i].y - first[i - 1].y) / by;
  }
  for (unsigned int i = 1; i < num; i++) {
    first[num - i - 1].x -= tmp[num - i].x * first[num - i].x;
    first[num - i - 1].y -= tmp[num - i].y * first[num - i].y;
  }
  delete [] tmp;
  return first;
}

// This is a routine that gives you control points for a smooth curve through the given
// points. It is based on solving the derivative of the cubic bezier routine.

// It was based upon my own math and the code of http://www.codeproject.com/KB/graphics/BezierSpline.aspx
// Adapted for 3d points and C++.
static void controlPoints(Apsis::Geometry::Point3d* points,
                          Apsis::Geometry::Point3d* first,
                          Apsis::Geometry::Point3d* second,
                          unsigned int num) {
  if (points == NULL) {
    return;
  }

  unsigned int n = num - 1;

  if (n < 1) {
    return;
  }
  else if (n == 1) {
    first[0].x = (2.0f * points[0].x + points[1].x) / 3.0f;
    first[0].y = (2.0f * points[0].y + points[1].y) / 3.0f;

    second[0].x = 2.0f * first[0].x - points[0].x;
    second[0].y = 2.0f * first[0].y - points[0].y;

    return;
  }

  Apsis::Geometry::Point3d* nodes = new Apsis::Geometry::Point3d[n];
  for (unsigned int i = 1; i < n-1; i++) {
    nodes[i].x = 4.0f * points[i].x + 2.0f * points[i + 1].x;
    nodes[i].y = 4.0f * points[i].y + 2.0f * points[i + 1].y;
  }
  nodes[0].x = points[0].x + 2.0f * points[1].x;
  nodes[n - 1].x = (8.0f * points[n - 1].x + points[n].x) / 2.0f;
  nodes[0].y = points[0].y + 2.0f * points[1].y;
  nodes[n - 1].y = (8.0f * points[n - 1].y + points[n].y) / 2.0f;

  firstControlPoints(nodes, first, n);

  for (unsigned int i = 0; i < n; i++) {
    if (i < n - 1) {
      second[i].x = 2.0f * points[i + 1].x - first[i + 1].x;
      second[i].y = 2.0f * points[i + 1].y - first[i + 1].y;
    }
    else {
      second[i].x = (points[n].x + first[n - 1].x) / 2.0f;
      second[i].y = (points[n].y + first[n - 1].y) / 2.0f;
    }
  }
  delete [] nodes;
}

Apsis::World::Terrain::Terrain(unsigned int width,
                               unsigned int height)
  : _width(width), _height(height),
    _vbo(Primitives::VertexBuffer::Target::Data),
    _ebo(Primitives::VertexBuffer::Target::Elements) {
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
  unsigned int vertices_size = 4 * width * height + 6 * (2*width*height - width - height);
  unsigned int elements_size = 6 * width * height + 6 * (2*width*height - width - height);
  _elements = new unsigned short[elements_size];

  // 8 values for each logical vertex: 3 per axis coordinate,
  //                                   3 per normal,
  //                                   2 per texcoord
  this->_vertices = new float[8 * vertices_size];

  unsigned int i = 0;
  unsigned int ei = 0;
  for (unsigned int h = 0; h < height; h++) {
    for (unsigned int w = 0; w < width; w++) {
      _vertices[i * 8 + 0] = (float)w;
      _vertices[i * 8 + 1] = 1.0f + ((float)(rand() % 10) / 10);
      _vertices[i * 8 + 2] = (float)h;

      _vertices[i * 8 + 3] = 0.0;
      _vertices[i * 8 + 4] = 1.0;
      _vertices[i * 8 + 5] = 0.0;

      _vertices[i * 8 + 6] = 0; //textureCoords[i].x;
      _vertices[i * 8 + 7] = 0; //textureCoords[i].y;

      i++;

      _vertices[i * 8 + 0] = (float)w + 1.0f;
      _vertices[i * 8 + 1] = 1.0f + ((float)(rand() % 10) / 10);
      _vertices[i * 8 + 2] = (float)h;

      _vertices[i * 8 + 3] = 0.0;
      _vertices[i * 8 + 4] = 1.0;
      _vertices[i * 8 + 5] = 0.0;

      _vertices[i * 8 + 6] = 0; //textureCoords[i].x;
      _vertices[i * 8 + 7] = 0; //textureCoords[i].y;

      i++;

      _vertices[i * 8 + 0] = (float)w + 1.0f;
      _vertices[i * 8 + 1] = 1.0f + ((float)(rand() % 10) / 10);
      _vertices[i * 8 + 2] = (float)h + 1.0f;

      _vertices[i * 8 + 3] = 0.0;
      _vertices[i * 8 + 4] = 1.0;
      _vertices[i * 8 + 5] = 0.0;

      _vertices[i * 8 + 6] = 0; //textureCoords[i].x;
      _vertices[i * 8 + 7] = 0; //textureCoords[i].y;

      i++;

      _vertices[i * 8 + 0] = (float)w;
      _vertices[i * 8 + 1] = 1.0f + ((float)(rand() % 10) / 10);
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

  for (unsigned int h = 0; h < height; h++) {
    for (unsigned int w = 0; w < width-1; w++) {
      unsigned int vi = (_width * h + w) * 4;

      _vertices[i * 8 + 0] = _vertices[(vi+1) * 8 + 0];
      _vertices[i * 8 + 1] = _vertices[(vi+1) * 8 + 1];
      _vertices[i * 8 + 2] = _vertices[(vi+1) * 8 + 2];

      _vertices[i * 8 + 3] = 1.0;
      _vertices[i * 8 + 4] = 0.0;
      _vertices[i * 8 + 5] = 0.0;

      _vertices[i * 8 + 6] = 0; //textureCoords[i].x;
      _vertices[i * 8 + 7] = 0; //textureCoords[i].y;

      i++;

      _vertices[i * 8 + 0] = _vertices[(vi+4) * 8 + 0];
      _vertices[i * 8 + 1] = _vertices[(vi+4) * 8 + 1];
      _vertices[i * 8 + 2] = _vertices[(vi+4) * 8 + 2];

      _vertices[i * 8 + 3] = 1.0;
      _vertices[i * 8 + 4] = 0.0;
      _vertices[i * 8 + 5] = 0.0;

      _vertices[i * 8 + 6] = 0; //textureCoords[i].x;
      _vertices[i * 8 + 7] = 0; //textureCoords[i].y;

      i++;

      _vertices[i * 8 + 0] = _vertices[(vi+7) * 8 + 0];
      _vertices[i * 8 + 1] = _vertices[(vi+7) * 8 + 1];
      _vertices[i * 8 + 2] = _vertices[(vi+7) * 8 + 2];

      _vertices[i * 8 + 3] = 1.0;
      _vertices[i * 8 + 4] = 0.0;
      _vertices[i * 8 + 5] = 0.0;

      _vertices[i * 8 + 6] = 0; //textureCoords[i].x;
      _vertices[i * 8 + 7] = 0; //textureCoords[i].y;

      i++;

      _vertices[i * 8 + 0] = _vertices[(vi+2) * 8 + 0];
      _vertices[i * 8 + 1] = _vertices[(vi+2) * 8 + 1];
      _vertices[i * 8 + 2] = _vertices[(vi+2) * 8 + 2];

      _vertices[i * 8 + 3] = 1.0;
      _vertices[i * 8 + 4] = 0.0;
      _vertices[i * 8 + 5] = 0.0;

      _vertices[i * 8 + 6] = 0; //textureCoords[i].x;
      _vertices[i * 8 + 7] = 0; //textureCoords[i].y;

      i++;

      /*
      float half_height = map->height() / 2.0f;
      float half_width = map->width() / 2.0f;

      for (unsigned int z = 0; z < map->height(); z++) {
        for (unsigned int x = 0; x < map->width(); x++) {
          Apsis::World::Tile* tile = map->atIndex(x, z);
          float h = 1.0f;
          float top = tile->hover();
          float heights[4];
          for (int i = 0; i < 4; i++) {
            heights[i] = tile->cornerHeight(i);
          }

          float p0 = tile->cornerHeight(Apsis::TOP_LEFT);
          float p1 = tile->firstControl(Apsis::TOP_LEFT);
          float p2 = tile->secondControl(Apsis::TOP_LEFT);
          float p3 = tile->cornerHeight(Apsis::TOP_RIGHT);

          float first_y = cubicBezier(p0,p1,p2,p3,0.33f);
          float second_y = cubicBezier(p0,p1,p2,p3,0.67f);
          float firsts[4] = {5.0f, 5.0f, 5.0f, 5.0f};
          float seconds[4] = {5.0f, 5.0f, 5.0f, 5.0f};
          firsts[Apsis::TOP_LEFT] = first_y;
          seconds[Apsis::TOP_LEFT] = second_y;

          p0 = tile->cornerHeight(Apsis::BOT_LEFT);
          p1 = tile->firstControl(Apsis::BOT_RIGHT);
          p2 = tile->secondControl(Apsis::BOT_RIGHT);
          p3 = tile->cornerHeight(Apsis::BOT_RIGHT);
          first_y = cubicBezier(p0,p1,p2,p3,0.33f);
          second_y = cubicBezier(p0,p1,p2,p3,0.67f);
          firsts[Apsis::BOT_RIGHT] = first_y;
          seconds[Apsis::BOT_RIGHT] = second_y;

          p0 = tile->cornerHeight(Apsis::TOP_RIGHT);
          p1 = tile->firstControl(Apsis::TOP_RIGHT);
          p2 = tile->secondControl(Apsis::TOP_RIGHT);
          p3 = tile->cornerHeight(Apsis::BOT_RIGHT);
          first_y = cubicBezier(p0,p1,p2,p3,0.33f);
          second_y = cubicBezier(p0,p1,p2,p3,0.67f);
          firsts[Apsis::TOP_RIGHT] = first_y;
          seconds[Apsis::TOP_RIGHT] = second_y;

          p0 = tile->cornerHeight(Apsis::TOP_LEFT);
          p1 = tile->firstControl(Apsis::BOT_LEFT);
          p2 = tile->secondControl(Apsis::BOT_LEFT);
          p3 = tile->cornerHeight(Apsis::BOT_LEFT);
          first_y = cubicBezier(p0,p1,p2,p3,0.33f);
          second_y = cubicBezier(p0,p1,p2,p3,0.67f);
          firsts[Apsis::BOT_LEFT] = first_y;
          seconds[Apsis::BOT_LEFT] = second_y;

          //renderer->drawTile((float)x - half_width, -top, (float)z - half_height, 0.5f, 0.5f, 0.5f, heights, first_y, second_y);

          renderer->drawTile((float)(x - half_width), (float)(-3 * (1 / _zoom) - top), (float)(z - half_height), 0.5f, 0.5f, 0.5f, heights, firsts, seconds);
          renderer->drawTileTop((float)(x - half_width), (float)(-3 * (1 / _zoom) - top), (float)(z - half_height), 0.5f, 0.5f, 0.5f, heights, firsts, seconds);
        }
      }//*/

      _elements[ei] = i-4; ei++;
      _elements[ei] = i-3; ei++;
      _elements[ei] = i-1; ei++;

      _elements[ei] = i-3; ei++;
      _elements[ei] = i-2; ei++;
      _elements[ei] = i-1; ei++;
    }
  }

  for (unsigned int h = 0; h < height-1; h++) {
    for (unsigned int w = 0; w < width; w++) {
      unsigned int vi = (_width * h + w) * 4;

      _vertices[i * 8 + 0] = _vertices[(vi+3) * 8 + 0];
      _vertices[i * 8 + 1] = _vertices[(vi+3) * 8 + 1];
      _vertices[i * 8 + 2] = _vertices[(vi+3) * 8 + 2];

      _vertices[i * 8 + 3] = 0.0;
      _vertices[i * 8 + 4] = 0.0;
      _vertices[i * 8 + 5] = 1.0;

      _vertices[i * 8 + 6] = 0; //textureCoords[i].x;
      _vertices[i * 8 + 7] = 0; //textureCoords[i].y;

      i++;

      _vertices[i * 8 + 0] = _vertices[(vi+2) * 8 + 0];
      _vertices[i * 8 + 1] = _vertices[(vi+2) * 8 + 1];
      _vertices[i * 8 + 2] = _vertices[(vi+2) * 8 + 2];

      _vertices[i * 8 + 3] = 0.0;
      _vertices[i * 8 + 4] = 0.0;
      _vertices[i * 8 + 5] = 1.0;

      _vertices[i * 8 + 6] = 0; //textureCoords[i].x;
      _vertices[i * 8 + 7] = 0; //textureCoords[i].y;

      i++;

      _vertices[i * 8 + 0] = _vertices[(vi+_width*4+1) * 8 + 0];
      _vertices[i * 8 + 1] = _vertices[(vi+_width*4+1) * 8 + 1];
      _vertices[i * 8 + 2] = _vertices[(vi+_width*4+1) * 8 + 2];

      _vertices[i * 8 + 3] = 0.0;
      _vertices[i * 8 + 4] = 0.0;
      _vertices[i * 8 + 5] = 1.0;

      _vertices[i * 8 + 6] = 0; //textureCoords[i].x;
      _vertices[i * 8 + 7] = 0; //textureCoords[i].y;

      i++;

      _vertices[i * 8 + 0] = _vertices[(vi+_width*4) * 8 + 0];
      _vertices[i * 8 + 1] = _vertices[(vi+_width*4) * 8 + 1];
      _vertices[i * 8 + 2] = _vertices[(vi+_width*4) * 8 + 2];

      _vertices[i * 8 + 3] = 0.0;
      _vertices[i * 8 + 4] = 0.0;
      _vertices[i * 8 + 5] = 1.0;

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

  Primitives::VertexShader   vs = Primitives::VertexShader::fromFile("assets/shaders/vertex/position.glsl");
  Primitives::FragmentShader fs = Primitives::FragmentShader::fromFile("assets/shaders/fragment/colorize.glsl");
  Primitives::FragmentShader ls = Primitives::FragmentShader::fromFile("assets/shaders/fragment/directional_lighting.glsl");

  Primitives::UnlinkedProgram unlinked;
  unlinked.attach(vs);
  unlinked.attach(fs);
  unlinked.attach(ls);
  unlinked.defineFragmentOutput("outColor");
  Primitives::Program program = unlinked.link();

  _vao.useProgram(program);
  _vbo.defineInput("position", program, 3, Primitives::VertexBuffer::Type::Float, false, 8, 0);
  _vbo.defineInput("normal",   program, 3, Primitives::VertexBuffer::Type::Float, false, 8, 3);
  _vbo.defineInput("texcoord", program, 2, Primitives::VertexBuffer::Type::Float, false, 8, 6);

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

  Primitives::Vector3 emission = {1.0f, 0.0f, 0.0f};
  _materials.push_back(Apsis::Model::Material(0.2f, 5.0f, 8.0f,
                                              emission,
                                              15.0f));

  Primitives::Vector3 position  = {0.0f, 0.0f, 0.0f};
  Primitives::Vector3 direction = {0.0f, 1.0f, 0.0f};
  Primitives::Vector3 color     = {1.0f, 0.0f, 1.0f};

  _lights.push_back(Apsis::Model::Light(position,
                                        direction,
                                        color));

  _vao.uploadUniform("material.ambient_intensity", _materials[0].ambientIntensity());
  _vao.uploadUniform("material.diffuse_intensity", _materials[0].diffuseIntensity());
  _vao.uploadUniform("material.specular_intensity",  _materials[0].specularIntensity());
  _vao.uploadUniform("material.shininess",  _materials[0].shininess());
  _vao.uploadUniform("material.emission",  _materials[0].emission());

  _vao.uploadUniform("light.direction", _lights[0].direction());
  _vao.uploadUniform("light.color", _lights[0].color());

  _vao.bind(_vbo);
  _vao.bind(_ebo);
}

Apsis::World::Terrain::~Terrain() {
  delete [] this->_vertices;
  delete [] this->_elements;
}

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

void Apsis::World::Terrain::draw(const Primitives::Matrix& projection,
                                 const World::Camera& camera) const {
  _vao.uploadUniform("proj", projection);
  _vao.uploadUniform("view", camera.view());

    const Primitives::Matrix& model_matrix
      = *(const Primitives::Matrix*)glm::value_ptr(glm::mat4(1.0f));

  _vao.uploadUniform("model", model_matrix);

  _vao.uploadUniform("camera", camera.eye());
  _vao.draw();
}

Apsis::World::Tile Apsis::World::Terrain::tileAt(unsigned int x,
                                                 unsigned int z) {
  float corners[4];
  unsigned int i = _width * z + x;
  corners[0] = _vertices[i * 8 + 1];
  i++;
  corners[1] = _vertices[i * 8 + 1];
  i++;
  corners[2] = _vertices[i * 8 + 1];
  i++;
  corners[3] = _vertices[i * 8 + 1];
  return Apsis::World::Tile(corners);
}
