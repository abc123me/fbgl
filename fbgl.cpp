#include "stdlib.h"
#include "math.h"
#include "sys/file.h"
#include "iostream"
#include "unistd.h"
#include "inttypes.h"
#include "stdio.h"

#define _DEBUG_MODE

uint16_t SCREEN_WIDTH = 160;
uint16_t SCREEN_HEIGHT = 128;
uint8_t RED_BITS = 5;
uint8_t GREEN_BITS = 6;
uint8_t BLUE_BITS = 5;
uint8_t BYTES_PER_PIXEL = 2;

uint32_t makeColorRGB(float r, float g, float b);
uint32_t makeColorRGB(float grey);
uint32_t makeColorHSV(float h, float s, float v);
void setPixel(int frameBuf, uint32_t color, uint16_t x, uint16_t y);
void fillRect(int frameBuf, uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void drawRect(int frameBuf, uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void line(int frameBuf, uint32_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void verticalLine(int frameBuf, uint32_t color, uint16_t y);
void horizontalLine(int frameBuf, uint32_t color, uint16_t x);
void background(int frameBuf, uint32_t color);
void drawEllipse(int frameBuf, uint32_t color, uint16_t xs1, uint16_t ys1, uint16_t rx, uint16_t ry);
void fillEllipse(int frameBuf, uint32_t color, uint16_t xc, uint16_t yc, uint16_t rx, uint16_t ry);
uint32_t getPosition(uint16_t x, uint16_t y);

int main(int argc, char** argv){
	int frameBuf = open("/dev/fb8", O_RDWR);
	background(frameBuf, makeColorRGB(0));
	fillRect(frameBuf, makeColorRGB(0.5, 0.5, 0.5), 30, 30, 30, 30);
	drawRect(frameBuf, makeColorRGB(1, 1, 1), 30, 30, 30, 30);
	line(frameBuf, makeColorRGB(1, 0, 0), 30, 30, 60, 60);
	line(frameBuf, makeColorRGB(0, 0, 1), 60, 30, 30, 60);
	verticalLine(frameBuf, makeColorRGB(0, 1, 0), 45);
	horizontalLine(frameBuf, makeColorRGB(0, 1, 0), 45);
	line(frameBuf, makeColorRGB(1, 1, 0), 30, 128 - 30, 30, 30);
	line(frameBuf, makeColorRGB(1, 1, 0), 160 - 30, 30, 30, 30);
	fillEllipse(frameBuf, makeColorRGB(1, 0, 1), 100, 100, 20, 20);
	//drawEllipse(frameBuf, makeColorRGB(1, 0, 1), 100, 100, 20, 10);
	//drawEllipse(frameBuf, makeColorRGB(1, 0, 1), 100, 100, 10, 20);
	//fillEllipse(frameBuf, makeColorRGB(1, 0, 1), 100, 100, 10, 10);
	std::cin.get();
	background(frameBuf, makeColorRGB(0, 0, 0));
	for(int i = 0; i < SCREEN_WIDTH; i += SCREEN_WIDTH / 20){
		//line(frameBuf, makeColorRGB(1, 0, 0), i, SCREEN_HEIGHT, SCREEN_WIDTH / 2, 0);
		line(frameBuf, makeColorRGB(1, 0, 0), SCREEN_WIDTH / 2, 0, i, SCREEN_HEIGHT);
		line(frameBuf, makeColorRGB(0, 0, 1), SCREEN_WIDTH / 2, SCREEN_HEIGHT, i, 0);
		//line(frameBuf, makeColorRGB(0, 0, 1), i, SCREEN_HEIGHT, 0, 0);
		//line(frameBuf, makeColorRGB(1, 0, 0), i, 0, SCREEN_WIDTH - i, SCREEN_HEIGHT);
		//line(frameBuf, makeColorRGB(0, 0, 1), SCREEN_WIDTH - i, 0, i, SCREEN_HEIGHT);
	}
	std::cin.get();
	background(frameBuf, makeColorRGB(0, 0, 0));
	uint32_t max = SCREEN_WIDTH * SCREEN_HEIGHT;
	lseek(frameBuf, 0, SEEK_SET);
	for(int i = 0; i < max; i++){
		float hue = (i / (float) max) * 360.0;
		float sat = 1.0, val = 1.0;
		uint32_t color = makeColorHSV(hue, sat, val);
		write(frameBuf, &color, BYTES_PER_PIXEL);
	}
	close(frameBuf);
	return 0;
}
void corners(int frameBuf, uint32_t color, uint16_t xs, uint16_t ys, uint16_t x, uint16_t y, bool fill){
	setPixel(frameBuf, color, xs + x, ys + y);
	setPixel(frameBuf, color, xs - x, ys - y);
	setPixel(frameBuf, color, xs - x, ys + y);
	setPixel(frameBuf, color, xs + x, ys - y);
	if(fill){
		lseek(frameBuf, getPosition(xs - x, ys + y), SEEK_SET);
		for(int i = xs - x; i < xs + x; i++)
			write(frameBuf, &color, BYTES_PER_PIXEL);
		lseek(frameBuf, getPosition(xs - x, ys - y), SEEK_SET);
		for(int i = xs - x; i < xs + x; i++)
			write(frameBuf, &color, BYTES_PER_PIXEL);
	}
}
void midpointEllipse(int frameBuf, uint32_t color, uint16_t xc, uint16_t yc, uint16_t rx, uint16_t ry, bool fill){
	const uint32_t rx2 = rx * rx, ry2 = ry * ry;
	int32_t x = 0, y = ry;
	int32_t p = ry2 - (rx2 * ry) + (rx2 / 4);
	while((2 * x * ry2) < (2 * y * rx2)){
		corners(frameBuf, color, xc, yc, x, y, fill);
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
		corners(frameBuf, color, xc, yc, x, y, fill);
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
void fillEllipse(int frameBuf, uint32_t color, uint16_t xc, uint16_t yc, uint16_t rx, uint16_t ry){
	midpointEllipse(frameBuf, color, xc, yc, rx, ry, true);
}
void drawEllipse(int frameBuf, uint32_t color, uint16_t xc, uint16_t yc, uint16_t rx, uint16_t ry){
	midpointEllipse(frameBuf, color, xc, yc, rx, ry, false);
}
void line(int frameBuf, uint32_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
	if(x1 == x2 && y1 == y2){
		lseek(frameBuf, getPosition(x1, y1), SEEK_SET);
		write(frameBuf, &color, BYTES_PER_PIXEL);
		return;
	}
	if(x1 == x2){
		uint16_t ymax = y1 >= y2 ? y1 : y2;
		uint16_t ymin = y1 < y2 ? y1 : y2;
		for (int y = ymin; y < ymax; y++){
			lseek(frameBuf, getPosition(x1, y), SEEK_SET);
			write(frameBuf, &color, BYTES_PER_PIXEL);
		}
		return;
	}
	if(y1 == y2){
		uint16_t xmax = x1 >= x2 ? x1 : x2;
		uint16_t xmin = x1 < x2 ? x1 : x2;
		lseek(frameBuf, getPosition(xmin, y1), SEEK_SET);
		for (int x = xmin; x < xmax; x++)
			write(frameBuf, &color, BYTES_PER_PIXEL);
		return;
	}
	int32_t vx = x2 - x1, vy = y2 - y1;
	float inc = 1 / sqrt(vx * vx + vy * vy);
	for(float f = 0; f < 1; f += inc){
		lseek(frameBuf, getPosition(x1 + vx * f, y1 + vy * f), SEEK_SET);
		write(frameBuf, &color, BYTES_PER_PIXEL);
	}
}
void horizontalLine(int frameBuf, uint32_t color, uint16_t x){
	lseek(frameBuf, getPosition(x, 0), SEEK_SET);
	for(int i = 0; i < SCREEN_HEIGHT; i++){
		write(frameBuf, &color, BYTES_PER_PIXEL);
		lseek(frameBuf, (SCREEN_WIDTH - 1) * BYTES_PER_PIXEL, SEEK_CUR);
	}
}
void verticalLine(int frameBuf, uint32_t color, uint16_t y){
	lseek(frameBuf, getPosition(0, y), SEEK_SET);
	for(int i = 0; i < SCREEN_WIDTH; i++)
		write(frameBuf, &color, BYTES_PER_PIXEL);
}
void background(int frameBuf, uint32_t color){
	lseek(frameBuf, 0, SEEK_SET);
	for(int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
		write(frameBuf, &color, BYTES_PER_PIXEL);
}
void fillRect(int frameBuf, uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	uint16_t y2 = h + y, y2d = y2 - 1;
	uint16_t x2 = w + x, x2d = x2 - 1;
	lseek(frameBuf, getPosition(x, y), SEEK_SET);
	for(int py = y; py < y2; py++){
		lseek(frameBuf, getPosition(x, py), SEEK_SET);
		for(int px = x; px < x2; px++)
			write(frameBuf, &color, BYTES_PER_PIXEL);
	}
}
void drawRect(int frameBuf, uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	uint16_t y2 = h + y, y2d = y2 - 1;
	uint16_t x2 = w + x, x2d = x2 - 1;
	for(int py = y; py < y2; py++){
		lseek(frameBuf, getPosition(x, py), SEEK_SET);
		if(py == y || py == y2d)
			for(int px = x; px < x2; px++)
				write(frameBuf, &color, BYTES_PER_PIXEL);
		else{
			write(frameBuf, &color, BYTES_PER_PIXEL);
			lseek(frameBuf, BYTES_PER_PIXEL * (w - 2), SEEK_CUR);
			write(frameBuf, &color, BYTES_PER_PIXEL);
		}
	}
}
void setPixel(int frameBuf, uint32_t color, uint16_t x, uint16_t y){
	lseek(frameBuf, getPosition(x, y), SEEK_SET);
	write(frameBuf, &color, BYTES_PER_PIXEL);
}
uint32_t getPosition(uint16_t x, uint16_t y){
	return BYTES_PER_PIXEL * (x + y * SCREEN_WIDTH);
}
uint32_t makeColorRGB(float rp, float gp, float bp){
	#ifdef _DEBUG_MODE
		if(rp < 0 || rp > 1){ fprintf(stderr, "Red pixel value is invalid: %f", rp); exit(-1); }
		if(gp < 0 || gp > 1){ fprintf(stderr, "Green pixel value is invalid: %f", rp); exit(-1); }
		if(bp < 0 || bp > 1){ fprintf(stderr, "Blue pixel value is invalid: %f", rp); exit(-1); }
	#endif
	static int64_t RMAX = -1, BMAX = -1, GMAX = -1;
	if(RMAX < 0) RMAX = pow(2, RED_BITS) - 1;
	if(GMAX < 0) GMAX = pow(2, GREEN_BITS) - 1;
	if(BMAX < 0) BMAX = pow(2, BLUE_BITS) - 1;
	uint32_t r = rp * RMAX;
	r <<= BLUE_BITS + GREEN_BITS;
	uint16_t g = gp * GMAX;
	g <<= BLUE_BITS;
	uint8_t b = bp * BMAX;
	return r + g + b;
}
uint32_t makeColorRGB(float grey){
	makeColorRGB(grey, grey, grey);
}
uint32_t makeColorHSV(float h, float s, float v){
	float hh, p, q, t, ff;
	int32_t i;

	if(s <= 0.0)
		return makeColorRGB(v, v, v);
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
		case 0: return makeColorRGB(v, t, p);
		case 1: return makeColorRGB(q, v, p);
		case 2: return makeColorRGB(p, v, t);
		case 3: return makeColorRGB(p, q, v);
		case 4: return makeColorRGB(t, p, v);
	}
	return makeColorRGB(v, p, q);
}
