#pragma once

#include "rtweekend.hpp"
#include "hittable.hpp"

class moving_sphere : public hittable {
public:
	moving_sphere() {}
	moving_sphere(
		point3 cen0, point3 cen1, double _time0, double _time1,
		double r, shared_ptr<material> m
	) : centre0(cen0), centre1(cen1), time0(_time0), time1(_time1),
		radius(r), mat_ptr(m)
	{}

	virtual bool hit(
		const ray& r, double t_min, double t_max, hit_record& rec
	) const override;

	point3 centre(double time) const;

	point3 centre0, centre1;
	double time0, time1;
	double radius;
	shared_ptr<material> mat_ptr;
};

point3 moving_sphere::centre(double time) const {
	return centre0 + ((time - time0) / (time1 - time0)) * (centre1 - centre0);
}

bool moving_sphere::hit(
	const ray& r, double t_min, double t_max, hit_record& rec
) const {
	vec3 oc = r.origin() - centre(r.time());
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius*radius;

	auto discriminant = half_b*half_b - a*c;
	if (discriminant < 0) return false;
	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
	if (root < t_min || t_max < root)
		return false;
	}

	rec.t = root;
	rec.p = r.at(rec.t);
	auto outward_normal = (rec.p - centre(r.time())) / radius;
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mat_ptr;

	return true;
}