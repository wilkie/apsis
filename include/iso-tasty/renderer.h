#ifndef ISOTASTY_RENDERER_H
#define ISOTASTY_RENDERER_H

#include "iso-tasty/tile.h"

namespace IsoTasty {
	class Renderer {
	public:
		Renderer();
		bool initializeViewport(unsigned int width, unsigned int height);
		void setProjection(unsigned int width, unsigned int height, 
			double rotation, double translationX, double translationZ, double zoom);
		void drawCube(float x, float y, float z, float width, float height, float depth);
		void drawSquare(float x, float y, float z, float width, float depth);
		void drawTile(float x, float y, float z, float width, float height, float depth, float heights[4], float fy[4], float sy[4]);
		void drawTileTop(float x, float y, float z, float width, float height, float depth, float heights[4], float fy[4], float sy[4]);
		void drawArrays(const float vertices[], const float normals[], const float colors[], const unsigned char indices[], unsigned int num);
		void drawSphere(float x, float y, float z, float width, float height, float depth);
		void clear();

		void test();
	
	private:
	};
}

#endif