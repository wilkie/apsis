#ifndef APSIS_MAP_H
#define APSIS_MAP_H

#include "iso-tasty/tile.h"

namespace Apsis {
	class Map {
	public:
		Map(unsigned int width, unsigned int height);
		~Map();

		// Properties
		unsigned int width();
		unsigned int height();

		unsigned int x();
		unsigned int x(unsigned int value);
		unsigned int z();
		unsigned int z(unsigned int value);

		// Methods
		void raise(float amount);
		void lift(float amount);

		Tile* atIndex(unsigned int x, unsigned int z);
	private:
		unsigned int _width;
		unsigned int _height;

		Tile** _map;

		unsigned int _x;
		unsigned int _z;
	};
}

#endif