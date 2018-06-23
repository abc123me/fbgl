#include "fbgl.h"
#include "sys/file.h"
#include "unistd.h"

int main(int argc, char** argv){
	int id = 8;
	if(argc > 1){
		try{ id = atoi(argv[1]); }
		catch(int err) {
			fprintf(stderr, "Usage %s <fb 0-128>\n", argv[0]);
			return 1;
		}
		if(id < 0 || id > 128){
			fprintf(stderr, "Usage %s <fb 0-128>\n", argv[0]);
			return 1;
		}
	}
        int frameBuf = openFramebufferLinux(id);

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

	std::cin.get();
        background(frameBuf, makeColorRGB(0, 0, 0));
        for(int i = 0; i < SCREEN_WIDTH; i += SCREEN_WIDTH / 20){
                line(frameBuf, makeColorRGB(1, 0, 0), SCREEN_WIDTH / 2, 0, i, SCREEN_HEIGHT);
                line(frameBuf, makeColorRGB(0, 0, 1), SCREEN_WIDTH / 2, SCREEN_HEIGHT, i, 0);
	}

        std::cin.get();
        background(frameBuf, makeColorRGB(0, 0, 0));
        uint32_t max = SCREEN_WIDTH * SCREEN_HEIGHT;
	toBeginning(frameBuf);
        for(int i = 0; i < max; i++){
                float hue = (i / (float) max) * 360.0;
                float sat = 1.0, val = 1.0;
                uint32_t color = makeColorHSV(hue, sat, val);
        	pushPixel(frameBuf, color);
	}

        closeFramebuffer(frameBuf);
        return 0;
}
