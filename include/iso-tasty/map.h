#ifndef ISOTASTY_MAP_H
#define ISOTASTY_MAP_H

namespace IsoTasty {
	class Map {
	public:
		Map(unsigned int width, unsigned int height);
		unsigned int width();
		unsigned int height();

	private:
		unsigned int _width;
		unsigned int _height;
	};
}

#endif