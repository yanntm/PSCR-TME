#pragma once

#include <iostream>
#include <algorithm>
#include "util/mtrand.h"


namespace pr {

// couleur RGB sur 24 bits
class Color {
	unsigned char b;  // Deliberate order: b, g, r for BGR memory layout to allow direct fwrite in BMP export
	unsigned char g;
	unsigned char r;
public:
	constexpr Color(unsigned char red = 255, unsigned char green = 255, unsigned char blue = 255) : b(blue), g(green), r(red) {}
	// shade by ratio / assombrir
	Color operator *(double ratio) {
		if (ratio > 1) {
			ratio = 1.0;
		}
		return Color( (unsigned char)(double(r)*ratio), (unsigned char)(double(g) *ratio) , (unsigned char)(double(b)* ratio));
	}
	// sommer deux couleurs (RGB donc additif)
	Color operator+ (const Color & c2) const {
		return Color(std::min(255,(int)r+c2.r),std::min(255,(int)g+c2.g),std::min(255,(int)b+c2.b));
	}
};

// ensure no padding : we depend on this in Image::exportToBMP
static_assert(sizeof(Color) == 3, "Color struct has unexpected padding");

namespace Colors {
	constexpr Color white (255, 255, 255);
	constexpr Color red (255, 0, 0);
	constexpr Color blue (0, 0, 255);
	constexpr Color black (0, 0, 0);

	inline Color random() {
		// 30 minimum to avoid too dark colors
		return Color(mtrand(30, 256), mtrand(30, 256), mtrand(30, 256));
	}
}

} /* namespace pr */

