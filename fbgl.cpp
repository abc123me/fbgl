#ifndef _FBGL_CPP
#define _FBGL_CPP

#include "inttypes.h"

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
void pushPixel(int frameBuf, uint32_t color);

void fillRect(int frameBuf, uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void drawRect(int frameBuf, uint32_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void line(int frameBuf, uint32_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void verticalLine(int frameBuf, uint32_t color, uint16_t y);
void horizontalLine(int frameBuf, uint32_t color, uint16_t x);
void drawEllipse(int frameBuf, uint32_t color, uint16_t xs1, uint16_t ys1, uint16_t rx, uint16_t ry);
void fillEllipse(int frameBuf, uint32_t color, uint16_t xc, uint16_t yc, uint16_t rx, uint16_t ry);
void background(int frameBuf, uint32_t color);

uint32_t getPosition(uint16_t x, uint16_t y);
void setPosition(uint16_t x, uint16_t y);
void toBeginning(int frameBuf);

int openFramebufferLinux(uint8_t id);
void closeFramebuffer(int frameBuf);

#endif
