#pragma once

#include "vec3.hpp"

#include <iostream>

void write_colour(std::ostream &out, colour pixel_colour, int samples_per_pixel) {
	auto r = pixel_colour.x();
	auto g = pixel_colour.y();
	auto b = pixel_colour.z();

	// Divide the colour  by the number of samples
	auto scale = 1.0 / samples_per_pixel;
	r = std::sqrt(scale * r);
	g = std::sqrt(scale * g);
	b = std::sqrt(scale * b);

	// Write the translated [0, 255] value of each component
	out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}