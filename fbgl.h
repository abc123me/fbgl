#ifndef _FBGL_HEADER
#define _FBGL_HEADER

#ifndef _FBGL_CPP
#include "fbgl.cpp"
#endif

#include "stdlib.h"
#include "math.h"
#include "sys/file.h"
#include "iostream"
#include "unistd.h"
#include "inttypes.h"
#include "stdio.h"

FrameBuffer::FrameBuffer(char* name, uint16_t width, uint16_t height){
	this->frameBuf = open(name, O_RDWR);
	this->width = width;
	this->height = height;
	this->redBits = 8;
	this->blueBits = 8;
	this->greenBits = 8;
	this->pixelSize = 4;
}
FrameBuffer::FrameBuffer(char* name, uint16_t width, uint16_t height, uint8_t redBits, uint8_t greenBits, uint8_t blueBits, uint8_t pixelSize){
	this->frameBuf = open(name, O_RDWR);
	this->width = width;
	this->height = height;
	this->redBits = redBits;
	this->blueBits = blueBits;
	this->greenBits = greenBits;
	this->pixelSize = pixelSize;
}
void FrameBuffer::corners(uint32_t color, uint16_t xs, uint16_t ys, uint16_t x, uint16_t y, bool fill){
	setPixel(color, xs + x, ys + y);
	setPixel(color, xs - x, ys - y);
	setPixel(color, xs - x, ys + y);
	setPixel(color, xs + x, ys - y);
	if(fill){
		lseek(frameBuf, getPosition(xs - x, ys + y), SEEK_SET);
		for(int i = xs - x; i < xs + x; i++)
			write(frameBuf, &color, pixelSize);
		lseek(frameBuf, getPosition(xs - x, ys - y), SEEK_SET);
		for(int i = xs - x; i < xs + x; i++)
			write(frameBuf, &color, pixelSize);
	}
}
void FrameBuffer::midpointEllipse(uint32_t color, uint16_t xc, uint16_t yc, uint16_t rx, uint16_t ry, bool fill){
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
void FrameBuffer::fillEllipse(uint32_t color, uint16_t xc, uint16_t yc, uint16_t rx, uint16_t ry){
	midpointEllipse(color, xc, yc, rx, ry, true);
}
void FrameBuffer::drawEllipse(uint32_t color, uint16_t xc, uint16_t yc, uint16_t rx, uint16_t ry){
	midpointEllipse(color, xc, yc, rx, ry, false);
}
void FrameBuffer::line(uint32_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
	if(x1 == x2 && y1 == y2){
		lseek(frameBuf, getPosition(x1, y1), SEEK_SET);
		write(frameBuf, &color, pixelSize);
		return;
	}
	if(x1 == x2){
		uint16_t ymax = y1 >= y2 ? y1 : y2;
		uint16_t ymin = y1 < y2 ? y1 : y2;
		for (int y = ymin; y < ymax; y++){
			lseek(frameBuf, getPosition(x1, y), SEEK_SET);
			write(frameBuf, &color, pixelSize);
		}
		return;
	}
	if(y1 == y2){
		uint16_t xmax = x1 >= x2 ? x1 : x2;
		uint16_t xmin = x1 < x2 ? x1 : x2;
		lseek(frameBuf, getPosition(xmin, y1), SEEK_SET);
		for (int x = xmin; x < xmax; x++)
			write(frameBuf, &color, pixelSize);
		return;
	}
	int32_t vx = x2 - x1, vy = y2 - y1;
	float inc = 1 / sqrt(vx * vx + vy * vy);
	for(float f = 0; f < 1; f += inc){
		lseek(frameBuf, getPosition(x1 + vx * f, y1 + vy * f), SEEK_SET);
		write(frameBuf, &color, pixelSize);
	}
}
void FrameBuffer::horizontalLine(uint32_t color, uint16_t x){
	lseek(frameBuf, getPosition(x, 0), SEEK_SET);
	for(int i = 0; i < height; i++){
		write(frameBuf, &color, pixelSize);
		lseek(frameBuf, (width - 1) * pixelSize, SEEK_CUR);
	}
}
void FrameBuffer::verticalLine(uint32_t color, uint16_t y){
	lseek(frameBuf, getPosition(0, y), SEEK_SET);
	for(int i = 0; i < width; i++)
		write(frameBuf, &color, pixelSize);
}
void FrameBuffer::background(uint32_t color){
	lseek(frameBuf, 0, SEEK_SET);
	for(int i = 0; i < width * height; i++)
		write(frameBuf, &color, pixelSize);
}
void FrameBuffer::fillRect(uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	uint16_t y2 = h + y;
	uint16_t x2 = w + x;
	lseek(frameBuf, getPosition(x, y), SEEK_SET);
	for(int py = y; py < y2; py++){
		lseek(frameBuf, getPosition(x, py), SEEK_SET);
		for(int px = x; px < x2; px++)
			write(frameBuf, &color, pixelSize);
	}
}
void FrameBuffer::drawRect(uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	uint16_t y2 = h + y, y2d = y2 - 1;
	uint16_t x2 = w + x;
	for(int py = y; py < y2; py++){
		lseek(frameBuf, getPosition(x, py), SEEK_SET);
		if(py == y || py == y2d)
			for(int px = x; px < x2; px++)
				write(frameBuf, &color, pixelSize);
		else{
			write(frameBuf, &color, pixelSize);
			lseek(frameBuf, pixelSize * (w - 2), SEEK_CUR);
			write(frameBuf, &color, pixelSize);
		}
	}
}
void FrameBuffer::setPixel(uint32_t color, uint16_t x, uint16_t y){
	lseek(frameBuf, getPosition(x, y), SEEK_SET);
	write(frameBuf, &color, pixelSize);
}
uint32_t FrameBuffer::getPosition(uint16_t x, uint16_t y){
	return pixelSize * (x + y * width);
}
uint32_t FrameBuffer::rgb(float rp, float gp, float bp){
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
uint32_t FrameBuffer::rgb(float grey){
	return rgb(grey, grey, grey);
}
uint32_t FrameBuffer::hsv(float h, float s, float v){
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

void FrameBuffer::destroy(){
	close(frameBuf);
}
void FrameBuffer::toBeginning(){
	lseek(frameBuf, 0, SEEK_SET);
}
void FrameBuffer::pushPixel(uint32_t color){
	write(frameBuf, &color, pixelSize);
}
void FrameBuffer::setPosition(uint16_t x, uint16_t y){
	lseek(frameBuf, getPosition(x, y), SEEK_SET);
}

#endif
