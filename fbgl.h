#ifndef _FBGL_HEADERS
#define _FBGL_HEADERS

#include "inttypes.h"
#include "stdio.h"

class FrameBufferGraphics{
	private:
		void err(){
			fprintf(stderr, "This function has not yet been overidden, this class is meant to be overriden by FrameBuffer or BufferedFrameBuffer to allow for greater interchangability\n!");
		}
	public:
		virtual uint32_t rgb(float r, float g, float b) { err(); };
		virtual uint32_t rgb(float grey) { err(); };
		virtual uint32_t hsv(float h, float s, float v) { err(); };

		virtual void setPixel(uint32_t color, uint16_t x, uint16_t y) { err(); };
		virtual void fillRect(uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h) { err(); };
		virtual void drawRect(uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h) { err(); };
		virtual void line(uint32_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) { err(); };
		virtual void verticalLine(uint32_t color, uint16_t y) { err(); };
		virtual void horizontalLine(uint32_t color, uint16_t x) { err(); };
		virtual void drawEllipse(uint32_t color, uint16_t xs1, uint16_t ys1, uint16_t rx, uint16_t ry) { err(); };
		virtual void fillEllipse(uint32_t color, uint16_t xc, uint16_t yc, uint16_t rx, uint16_t ry) { err(); };
		virtual void background(uint32_t color) { err(); };

		virtual void pushPixel(uint32_t color) { err(); };
		virtual uint32_t getPosition(uint16_t x, uint16_t y) { err(); };
		virtual void setPosition(uint16_t x, uint16_t y) { err(); };
		virtual void toBeginning() { err(); };

};
class FrameBuffer : public FrameBufferGraphics{
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

		//Virtuals of FrameBufferGraphics
		uint32_t rgb(float r, float g, float b);
		uint32_t rgb(float grey);
		uint32_t hsv(float h, float s, float v);
		void setPixel(uint32_t color, uint16_t x, uint16_t y);
		void fillRect(uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
		void drawRect(uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
		void line(uint32_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		void verticalLine(uint32_t color, uint16_t y);
		void horizontalLine(uint32_t color, uint16_t x);
		void drawEllipse(uint32_t color, uint16_t xs1, uint16_t ys1, uint16_t rx, uint16_t ry);
		void fillEllipse(uint32_t color, uint16_t xc, uint16_t yc, uint16_t rx, uint16_t ry);
		void background(uint32_t color);
		void pushPixel(uint32_t color);
		uint32_t getPosition(uint16_t x, uint16_t y);
		void setPosition(uint16_t x, uint16_t y);
		void toBeginning();

		void destroy();
};
class BufferedFrameBuffer : public FrameBufferGraphics{
	private:
		uint16_t width, height;
		uint8_t redBits, greenBits, blueBits;
		uint8_t pixelSize;
		uint8_t* buffer;
		uint32_t bufferPos;
		int frameBuf;

		void corners(uint32_t color, uint16_t xa, uint16_t ya, uint16_t xs, uint16_t ys, bool fill);
		void midpointEllipse(uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool fill);
	public:
		BufferedFrameBuffer(char* name, uint16_t width, uint16_t height);
		BufferedFrameBuffer(char* name, uint16_t width, uint16_t height, uint8_t redBits, uint8_t greenBits, uint8_t blueBits, uint8_t pixelSize);

		//Virtuals of FrameBufferGraphics
		uint32_t rgb(float r, float g, float b);
		uint32_t rgb(float grey);
		uint32_t hsv(float h, float s, float v);
		void setPixel(uint32_t color, uint16_t x, uint16_t y);
		void fillRect(uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
		void drawRect(uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
		void line(uint32_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		void verticalLine(uint32_t color, uint16_t y);
		void horizontalLine(uint32_t color, uint16_t x);
		void drawEllipse(uint32_t color, uint16_t xs1, uint16_t ys1, uint16_t rx, uint16_t ry);
		void fillEllipse(uint32_t color, uint16_t xc, uint16_t yc, uint16_t rx, uint16_t ry);
		void background(uint32_t color);
		void pushPixel(uint32_t color);
		uint32_t getPosition(uint16_t x, uint16_t y);
		void setPosition(uint16_t x, uint16_t y);
		void toBeginning();

		void destroy();
		void pushBuffer();
};

#include "fbgl.cpp"
#include "fbgl_buffered.cpp"

#endif
