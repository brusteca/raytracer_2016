#pragma once
#ifndef COLOR_H
#define COLOR_H

#define byte unsigned char

using namespace std;

struct Color {
	byte red;
	byte green;
	byte blue;
	Color() {
		red = green = blue = 0;
	}
	Color(byte r, byte g, byte b) {
		red = r;
		green = g;
		blue = b;
	}
};
//uso esta para calculos intermedios asi no pierdo informacion
struct ColorInt {
	int red;
	int green;
	int blue;
	ColorInt() {
		red = green = blue = 0;
	}
	ColorInt(int r, int g, int b) {
		red = r;
		green = g;
		blue = b;
	}
};

#endif
