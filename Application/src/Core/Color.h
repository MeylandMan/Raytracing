#pragma once

#include "Image.h"
#include <iostream>

void write_color(const ImageParams& image, const color& pixelColor, data& pixels, int x, int line) {
	auto r = pixelColor.x;
	auto g = pixelColor.y;
	auto b = pixelColor.z;

	// Translate the [0,1] component values to the byte range [0,255].
	int rbyte = int(255.999 * r);
	int gbyte = int(255.999 * g);
	int bbyte = int(255.999 * b);

	// Write out the pixel color components.
	int idx = ((image.Height - 1 - line) * image.Width + x) * 3;

	if (idx + 2 < pixels.size()) {
		pixels[idx + 0] = static_cast<unsigned char>(rbyte);
		pixels[idx + 1] = static_cast<unsigned char>(gbyte);
		pixels[idx + 2] = static_cast<unsigned char>(bbyte);
	}
	else {
		std::cout << "Error editing pixels. id:" << idx << "(r:"<< r << ", g : " << g << ", b : " << b << ")\n";
	}

}
