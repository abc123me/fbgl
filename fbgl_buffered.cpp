#ifndef _FBGL_BUFFERED_SOURCE
#define _FBGL_BUFFERED_SOURCE

#include "stdlib.h"
#include "math.h"
#include "sys/file.h"
#include "iostream"
#include "unistd.h"
#include "inttypes.h"
#include "stdio.h"
#include "string.h"

BufferedFrameBuffer::BufferedFrameBuffer(char* name, uint16_t width, uint16_t height){
	this->width = width;
	this->height = height;
	this->redBits = 8;
	this->blueBits = 8;
	this->greenBits = 8;
	this->pixelSize = 4;
	this->bufferPos = 0;
	this->frameBuf = open(name, O_RDWR);
	buffer = new uint8_t[width * height * 4];
}
BufferedFrameBuffer::BufferedFrameBuffer(char* name, uint16_t width, uint16_t height, uint8_t redBits, uint8_t greenBits, uint8_t blueBits, uint8_t pixelSize){
	this->width = width;
	this->height = height;
	this->redBits = redBits;
	this->blueBits = blueBits;
	this->greenBits = greenBits;
	this->pixelSize = pixelSize;
	this->bufferPos = 0;
	this->frameBuf = open(name, O_RDWR);
	buffer = new uint8_t[width * height * pixelSize];
}
void BufferedFrameBuffer::corners(uint32_t color, uint16_t xs, uint16_t ys, uint16_t x, uint16_t y, bool fill){
	memcpy(buffer + getPosition(xs + x, ys + y), &color, pixelSize);
	memcpy(buffer + getPosition(xs + x, ys - y), &color, pixelSize);
	memcpy(buffer + getPosition(xs - x, ys + y), &color, pixelSize);
	memcpy(buffer + getPosition(xs - x, ys - y), &color, pixelSize);
	if(fill){
		bufferPos = getPosition(xs - x, ys + y);
		for(int i = xs - x; i < xs + x; i++)
			pushPixel(color);
		bufferPos = getPosition(xs - x, ys - y);
		for(int i = xs - x; i < xs + x; i++)
			pushPixel(color);
	}
}
void BufferedFrameBuffer::midpointEllipse(uint32_t color, uint16_t xc, uint16_t yc, uint16_t rx, uint16_t ry, bool fill){
	const uint32_t rx2 = rx * rx, ry2 = ry * ry;
	int32_t x = 0, y = ry;
	int32_t p = ry2 - (rx2 * ry) + (rx2 / 4);
	while((2 * x * ry2) < (2 * y * rx2)){
		corners(color, xc, yc, x, y, fill);
		if(p < 0){
			x++;
			p += (2 * ry2 * x) + ry2;
		}
		else{
			x++; y--;
			p += (2 * ry2 * x + ry2) - (2 * rx2 * y);
		}
	}
	float px = x * x + x + 0.25f;
	float py = y * y - 2 * y + 1.0f;
	p = (px * ry2) + (py * rx2) - (rx2 * ry2);
	while(y >= 0){
		corners(color, xc, yc, x, y, fill);
		if(p > 0){
			y--;
			p -= (2 * rx2 * y) + rx2;
		}
		else{
			y--; x++;
			p += (2 * ry2 * x) - (2 * rx2 * y) - rx2;
		}
	}
}
void BufferedFrameBuffer::fillEllipse(uint32_t color, uint16_t xc, uint16_t yc, uint16_t rx, uint16_t ry){
	midpointEllipse(color, xc, yc, rx, ry, true);
}
void BufferedFrameBuffer::drawEllipse(uint32_t color, uint16_t xc, uint16_t yc, uint16_t rx, uint16_t ry){
	midpointEllipse(color, xc, yc, rx, ry, false);
}
void BufferedFrameBuffer::line(uint32_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
	if(x1 == x2 && y1 == y2){
		memcpy(buffer + getPosition(x1, y1), &color, pixelSize);
		return;
	}
	if(x1 == x2){
		uint16_t ymax = y1 >= y2 ? y1 : y2;
		uint16_t ymin = y1 < y2 ? y1 : y2;
		for (int y = ymin; y < ymax; y++)
			memcpy(buffer + getPosition(x1, y), &color, pixelSize);
		return;
	}
	if(y1 == y2){
		uint16_t xmax = x1 >= x2 ? x1 : x2;
		uint16_t xmin = x1 < x2 ? x1 : x2;
		bufferPos = getPosition(xmin, y1);
		for (int x = xmin; x < xmax; x++)
			pushPixel(color);
		return;
	}
	int32_t vx = x2 - x1, vy = y2 - y1;
	float inc = 1 / sqrt(vx * vx + vy * vy);
	for(float f = 0; f < 1; f += inc)
		memcpy(buffer + getPosition(x1 + vx * f, y1 + vy * f), &color, pixelSize);
}
void BufferedFrameBuffer::horizontalLine(uint32_t color, uint16_t x){
	for(int i = 0; i < height; i++)
		memcpy(buffer + getPosition(x, i), &color, pixelSize);
}
void BufferedFrameBuffer::verticalLine(uint32_t color, uint16_t y){
	bufferPos = getPosition(0, y);
	for(int i = 0; i < width; i++)
		pushPixel(color);
}
void BufferedFrameBuffer::background(uint32_t color){
	bufferPos = 0;
	for(int i = 0; i < width * height; i++)
		pushPixel(color);
}
void BufferedFrameBuffer::fillRect(uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	uint16_t y2 = h + y;
	uint16_t x2 = w + x;
	for(int py = y; py < y2; py++){
		bufferPos = getPosition(x, py);
		for(int px = x; px < x2; px++)
			pushPixel(color);
	}
}
void BufferedFrameBuffer::drawRect(uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	uint16_t y2 = h + y, y2d = y2 - 1;
	uint16_t x2 = w + x;
	for(int py = y; py < y2; py++){
		bufferPos = getPosition(x, py);
		if(py == y || py == y2d)
			for(int px = x; px < x2; px++)
				pushPixel(color);
		else{
			pushPixel(color);
			memcpy(buffer + (bufferPos + pixelSize * (w - 2)), &color, pixelSize);
		}
	}
}
void BufferedFrameBuffer::setPixel(uint32_t color, uint16_t x, uint16_t y){
	memcpy(buffer + getPosition(x, y), &color, pixelSize);
}
uint32_t BufferedFrameBuffer::getPosition(uint16_t x, uint16_t y){
	return pixelSize * (x + y * width);
}
uint32_t BufferedFrameBuffer::rgb(float rp, float gp, float bp){
	static int64_t RMAX = -1, BMAX = -1, GMAX = -1;
	if(RMAX < 0) RMAX = pow(2, redBits) - 1;
	if(GMAX < 0) GMAX = pow(2, greenBits) - 1;
	if(BMAX < 0) BMAX = pow(2, blueBits) - 1;
	uint32_t r = rp * RMAX;
	r <<= blueBits + greenBits;
	uint16_t g = gp * GMAX;
	g <<= blueBits;
	uint8_t b = bp * BMAX;
	return r + g + b;
}
uint32_t BufferedFrameBuffer::rgb(float grey){
	return rgb(grey, grey, grey);
}
uint32_t BufferedFrameBuffer::hsv(float h, float s, float v){
	float hh, p, q, t, ff;
	int32_t i;

	if(s <= 0.0)
		return rgb(v, v, v);
	hh = h;
	while(hh >= 360.0)
		hh -= 360.0;
	while(hh < 0.0)
		hh += 360.0;
	hh /= 60.0;
	i = (int32_t) hh;
	ff = hh - i;
	p = v * (1.0 - s);
	q = v * (1.0 - (s * ff));
	t = v * (1.0 - (s * (1.0 - ff)));

	switch(i) {
		case 0: return rgb(v, t, p);
		case 1: return rgb(q, v, p);
		case 2: return rgb(p, v, t);
		case 3: return rgb(p, q, v);
		case 4: return rgb(t, p, v);
	}
	return rgb(v, p, q);
}

void BufferedFrameBuffer::destroy(){
	delete[] buffer;
	close(frameBuf);
}
void BufferedFrameBuffer::toBeginning(){
	bufferPos = 0;
}
void BufferedFrameBuffer::pushPixel(uint32_t color){
	memcpy(buffer + bufferPos, &color, pixelSize);
	bufferPos += pixelSize;
}
void BufferedFrameBuffer::setPosition(uint16_t x, uint16_t y){
	bufferPos = getPosition(x, y);
}
void BufferedFrameBuffer::pushBuffer(){
	lseek(frameBuf, 0, SEEK_SET);
	write(frameBuf, buffer, width * height * pixelSize);
}

#endif
