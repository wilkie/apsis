#ifndef ISOTASTY_RENDERER_H
#define ISOTASTY_RENDERER_H

namespace IsoTasty {
	class Renderer {
	public:
		Renderer();
		bool initializeViewport(unsigned int width, unsigned int height);
		void setProjection(unsigned int width, unsigned int height, 
			double rotation, double translationX, double translationZ, double zoom);
		void drawCube(float x, float y, float z, float width, float height, float depth);
		void drawSquare(float x, float y, float z, float width, float depth);
		void clear();
	
	private:
	};
}

#endif