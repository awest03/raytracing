#pragma once

#include "vec3.hpp"

#include <iostream>

void write_colour(std::ostream &out, colour pixel_colour) {
	out << static_cast<int>(255.999 * pixel_colour.x()) << ' '
		<< static_cast<int>(255.999 * pixel_colour.y()) << ' '
		<< static_cast<int>(255.999 * pixel_colour.z()) << '\n';
}