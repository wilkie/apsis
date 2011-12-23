#ifndef ISOTASTY_MAP_H
#define ISOTASTY_MAP_H

#include "iso-tasty/tile.h"

namespace IsoTasty {
	class Map {
	public:
		Map(unsigned int width, unsigned int height);
		~Map();
		unsigned int width();
		unsigned int height();

		Tile* atIndex(unsigned int x, unsigned int z);
	private:
		unsigned int _width;
		unsigned int _height;

		Tile* _map;
	};
}

#endif