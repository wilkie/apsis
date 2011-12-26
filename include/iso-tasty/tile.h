#ifndef ISOTASTY_TILE_H
#define ISOTASTY_TILE_H

namespace IsoTasty {
	enum Corner {
		TOP_LEFT = 0,
		TOP_RIGHT,
		BOT_LEFT,
		BOT_RIGHT
	};

	class Tile {
	public:
		Tile();

		// Properties
		float hover();
		float hover(float value);

		float cornerHeight(unsigned int index);
		float cornerHeight(unsigned int index, float value);

		float firstControl(unsigned int index);
		float firstControl(unsigned int index, float value);

		float secondControl(unsigned int index);
		float secondControl(unsigned int index, float value);

		// Methods
		float minHeight();
		float maxHeight();

	private:
		// Tile parameters
		float _height;
		float _hover;
		float _cornerHeight[4];
		float _firstControl[4];
		float _secondControl[4];
	};
}

#endif