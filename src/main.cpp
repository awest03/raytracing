#include "rtweekend.hpp"

#include "colour.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"

#include <iostream>

colour ray_colour(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // If the ray bounce limit is exceeded, no more light is gathered
    if (depth <= 0)
        return colour(0, 0, 0);

    if (world.hit(r, 0.001, infinity, rec)) {
        //point3 target = rec.p + rec.normal + random_unit_vector();
        point3 target = rec.p + random_in_hemisphere(rec.normal);
        return 0.5 * ray_colour(ray(rec.p, target - rec.p), world, depth - 1);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * colour(1.0, 1.0, 1.0) + t * colour(0.5, 0.7, 1.0);
}

int main() {
	// Image
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

	// World
	hittable_list world;
	world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

	// Camera
	camera cam;

	// Render
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	for (int j = image_height - 1; j >= 0; --j) {
	    std::cerr << "\rScanlines remaining: " << j << std::flush;
	    for (int i = 0; i < image_width; ++i) {
	        colour pixel_colour(0, 0, 0);
	        for (int s = 0; s < samples_per_pixel; ++s) {
	            auto u = (i + random_double()) / (image_width - 1);
	            auto v = (j + random_double()) / (image_height - 1);
	            ray r = cam.get_ray(u, v);
	            pixel_colour += ray_colour(r, world, max_depth);
	        }
	        write_colour(std::cout, pixel_colour, samples_per_pixel);
	    }
	}

	std::cerr << "\nDone\n";

	return 0;
}
