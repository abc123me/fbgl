# fbgl
Framebuffer graphics library for displays such as the ST7735

## Tested devices
- ST7735 (works great) `FrameBuffer("/dev/fbX", 160, 128, 5, 6, 5, 2);`

## Usage (psuedocode)
```
FrameBuffer fb = FrameBuffer(width, height, red_bits, blue_bits, green_bits, pixel_size);
                 //or just FrameBuffer(width, height); (autodefaults to 8 red, 8 green, 8 blue, 4 bytes)
fb.drawRect(fb.rgb(percent 0 - 1, 0 - 1, 0 - 1), x, y, w, h);
fb.fillRect(fb.hsv(angle 0 - 360, percent 0 - 1, 0 - 1), x, y, w, h);
fb.destroy();
```
