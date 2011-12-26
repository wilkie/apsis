#include "iso-tasty/viewport.h"

#include "stdio.h"
#include "math.h"

IsoTasty::Viewport::Viewport(unsigned int width, unsigned int height) :
  _width(width),
  _height(height),
  _rotation(45.0),
  _x(0.0),
  _z(0.0),
  _zoom(1.0) {
}

unsigned int IsoTasty::Viewport::width() {
	return _width;
}

unsigned int IsoTasty::Viewport::height() {
	return _height;
}

void IsoTasty::Viewport::draw(Renderer* renderer, Map* map) {
	renderer->setProjection(_width, _height, _rotation, _x, _z, _zoom);

	float half_height = map->height() / 2.0f;
	float half_width = map->width() / 2.0f;

	for (unsigned int z = 0; z < map->height(); z++) {
		for (unsigned int x = 0; x < map->width(); x++) {
			IsoTasty::Tile* tile = map->atIndex(x, z);
			float h = 1.0f;
			float top = tile->hover();
			float heights[4];
			for (int i = 0; i < 4; i++) {
				heights[i] = tile->cornerHeight(i);
			}
			renderer->drawTile((float)x - half_width, -top, (float)z - half_height, 0.5f, 0.5f, 0.5f, heights);
			renderer->drawTileTop((float)x - half_width, -top, (float)z - half_height, 0.5f, 0.5f, 0.5f, heights);
		}
	}

	IsoTasty::Tile* tile = map->atIndex(map->x(), map->z());
	renderer->drawSphere(
		(float)map->x() - half_width - 0.5f, 
		(-tile->hover() + tile->cornerHeight(IsoTasty::TOP_LEFT))/2.0f,
		(float)map->z() - half_height - 0.5f,
		0.25f, 0.25f, 0.25f);

	//renderer->test();
}

void IsoTasty::Viewport::move(double deltaX, double deltaZ) {
	double radians = _rotation / 180.0 * 3.1415926;
	double cosine = cos(radians);
	double sine = sin(radians);

	_x += deltaX*cosine - deltaZ*sine;
	_z += deltaX*sine + deltaZ*cosine;
}

void IsoTasty::Viewport::rotate(double delta) {
	if (delta < 0) {
		delta = -fmod(-delta, 360);
	}
	_rotation += 360.0;
	_rotation += delta;
	_rotation = fmod(_rotation, 360.0);
}

void IsoTasty::Viewport::zoom(double factor) {
	_zoom *= factor;
}