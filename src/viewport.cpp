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
	int start_x = (int)-(map->height()/2.0f);
	int end_x = (int)(map->height()/2.0f);
	int start_z = (int)-(map->width()/2.0f);
	int end_z = (int)(map->width()/2.0f);

	for (int z = start_z; z <= end_z; z++) {
		for (int x = end_x; x >= start_x; x--) {
			renderer->drawCube((float)x, 0.25f,(float)z,1.0f, 0.5,1.0f);
			renderer->drawSquare((float)x, 0.5f, (float)z, 1.0f, 1.0f);
		}
	}
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