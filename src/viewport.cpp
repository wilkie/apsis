#include "iso-tasty/viewport.h"

#include "stdio.h"
#include "math.h"

IsoTasty::Viewport::Viewport(unsigned int width, unsigned int height) :
  _width(width),
  _height(height),
  _rotation(45.0),
  _x(0.0),
  _z(0.0) {
}

unsigned int IsoTasty::Viewport::width() {
	return _width;
}

unsigned int IsoTasty::Viewport::height() {
	return _height;
}

void IsoTasty::Viewport::draw(Renderer* renderer, Map* map) {
	renderer->setProjection(_width, _height, _rotation, _x, _z);

	float h = 0.5f;
	if (_rotation >= 0.0f && _rotation <= 90.0f) {
		for (int z = (int)-(map->width()/2.0f); z <= (int)(map->width()/2.0f); z++) {
			for (int x = (int)(map->height()/2.0f); x >= (int)-(map->height()/2.0f); x--) {
				renderer->drawCube((float)x,h/2.0f,(float)z,1.0f,h,1.0f);
				//drawCubeTop((float)x,h/2.0f,(float)z,1.0f,h,1.0f);
			}
			//h+=0.5;
		}
	}
	else if (_rotation >= 90.0f && _rotation <= 180.0f) {
		for (int z = (int)(map->width()/2.0); z >= (int)-(map->width()/2.0); z--) {
			for (int x = (int)(map->height()/2.0); x >= (int)-(map->height()/2.0); x--) {
				renderer->drawCube((float)x,h/2.0f,(float)z,1.0f,h,1.0f);
				//drawCubeTop((float)x,h/2.0,(float)z,1.0f,h,1.0f);
			}
			//h+=0.5;
		}
	}
	else if (_rotation >= 180.0f && _rotation <= 270.0f) {
		for (int z = (int)(map->width()/2.0); z >= (int)-(map->width()/2.0); z--) {
			for (int x = (int)-(map->height()/2.0); x <= (int)(map->height()/2.0); x++) {
				renderer->drawCube((float)x,h/2.0f,(float)z,1.0f,h,1.0f);
				//drawCubeTop((float)x,h/2.0,(float)z,1.0f,h,1.0f);
			}
			//h+=0.5;
		}
	}
	else {
		for (int z = (int)-(map->width()/2.0); z <= (int)(map->width()/2.0); z++) {
			for (int x = (int)-(map->height()/2.0); x <= (int)(map->height()/2.0); x++) {
				renderer->drawCube((float)x,h/2.0f,(float)z,1.0f,h,1.0f);
				//drawCubeTop((float)x,h/2.0,(float)z,1.0f,h,1.0f);
			}
			//h+=0.5;
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