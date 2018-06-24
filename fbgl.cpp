#ifndef _FBGL_CPP
#define _FBGL_CPP

#include "inttypes.h"

class FrameBuffer{
	private:
		uint16_t width, height;
		uint8_t redBits, greenBits, blueBits;
		uint8_t pixelSize;
		int frameBuf;

		void corners(uint32_t color, uint16_t xa, uint16_t ya, uint16_t xs, uint16_t ys, bool fill);
		void midpointEllipse(uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool fill);
	public:
		FrameBuffer(char* name, uint16_t width, uint16_t height);
		FrameBuffer(char* name, uint16_t width, uint16_t height, uint8_t redBits, uint8_t greenBits, uint8_t blueBits, uint8_t pixelSize);

		uint32_t rgb(float r, float g, float b);
		uint32_t rgb(float grey);
		uint32_t hsv(float h, float s, float v);

		void setPixel(uint32_t color, uint16_t x, uint16_t y);
		void pushPixel(uint32_t color);
		void fillRect(uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
		void drawRect(uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
		void line(uint32_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		void verticalLine(uint32_t color, uint16_t y);
		void horizontalLine(uint32_t color, uint16_t x);
		void drawEllipse(uint32_t color, uint16_t xs1, uint16_t ys1, uint16_t rx, uint16_t ry);
		void fillEllipse(uint32_t color, uint16_t xc, uint16_t yc, uint16_t rx, uint16_t ry);
		void background(uint32_t color);
		uint32_t getPosition(uint16_t x, uint16_t y);
		void setPosition(uint16_t x, uint16_t y);
		void toBeginning();

		void destroy();
};
#endif
