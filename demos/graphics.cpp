#include "fbgl.h"
#include "sys/file.h"
#include "unistd.h"

int main(int argc, char** argv){
	int width = 160, height = 128;
	FrameBuffer fb = FrameBuffer("/dev/fb8", width, height, 5, 6, 5, 2);

        fb.background(fb.rgb(0));
        fb.fillRect(fb.rgb(0.5, 0.5, 0.5), 30, 30, 30, 30);
        fb.drawRect(fb.rgb(1, 1, 1), 30, 30, 30, 30);
        fb.line(fb.rgb(1, 0, 0), 30, 30, 60, 60);
        fb.line(fb.rgb(0, 0, 1), 60, 30, 30, 60);
        fb.verticalLine(fb.rgb(0, 1, 0), 45);
	fb.horizontalLine(fb.rgb(0, 1, 0), 45);
	fb.line(fb.rgb(1, 1, 0), 30, 128 - 30, 30, 30);
	fb.line(fb.rgb(1, 1, 0), 160 - 30, 30, 30, 30);
	fb.fillEllipse(fb.rgb(1, 0, 1), 100, 100, 20, 20);

	std::cin.get();
        fb.background(fb.rgb(0, 0, 0));
        for(int i = 0; i < width; i += width / 20){
                fb.line(fb.rgb(1, 0, 0), width / 2, 0, i, height);
                fb.line(fb.rgb(0, 0, 1), width / 2, height, i, 0);
	}

        std::cin.get();
        fb.background(fb.rgb(0, 0, 0));
        uint32_t max = width * height;
	fb.toBeginning();
        for(int i = 0; i < max; i++){
                float hue = (i / (float) max) * 360.0;
                float sat = 1.0, val = 1.0;
                uint32_t color = fb.hsv(hue, sat, val);
        	fb.pushPixel(color);
	}

	fb.destroy();
        return 0;
}
