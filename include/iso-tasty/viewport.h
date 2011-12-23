#ifndef ISOTASTY_VIEWPORT_H
#define ISOTASTY_VIEWPORT_H

#include "iso-tasty/renderer.h"
#include "iso-tasty/map.h"

namespace IsoTasty {
	class Viewport {
	public:
		Viewport(unsigned int width, unsigned int height);
		unsigned int width();
		unsigned int height();

		void move(double deltaX, double deltaZ);
		void rotate(double delta);

		void draw(Renderer* renderer, Map* map);

	private:
		double _rotation;
		double _x;
		double _z;

		unsigned int _width;
		unsigned int _height;
		double _aspect;
	};
}

#endif