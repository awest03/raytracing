#include "rtweekend.hpp"

#include "colour.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "moving_sphere.hpp"

#include <iostream>

colour ray_colour(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // If the ray bounce limit is exceeded, no more light is gathered
    if (depth <= 0)
        return colour(0, 0, 0);

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        colour attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_colour(scattered, world, depth - 1);
        return colour(0, 0, 0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * colour(1.0, 1.0, 1.0) + t * colour(0.5, 0.7, 1.0);
}

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(colour(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = colour::random() * colour::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto centre2 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<moving_sphere>(center, centre2, 0.0,
                        1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = colour::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(colour(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(colour(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int main() {
	// Image
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

	// World
	auto world = random_scene();

	// Camera
	point3 lookfrom(13, 2, 3);
	point3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.1;
	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus,
	    0.0, 1.0);

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
