#pragma once
#include "hittable.h"
#include "vec3.h"

class sphere : public hittable
{
public:
	point3 center;
	double radius;
	sphere() {}
	sphere(point3 cen, double r) :center(cen), radius(r) {}
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	vec3 oc = r.origin() - center;
	auto a = dot(r.direction(), r.direction());
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squred() - radius * radius;
	auto discriminator = half_b * half_b - a * c;

	if (discriminator > 0)
	{
		auto root = sqrt(discriminator);
		auto temp = (-half_b - root) / a;
		if (temp<t_max && temp>t_min)
		{
			rec.t = temp;
			rec.p = r.at(temp);
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_noraml(r, outward_normal);
			return true;
		}
		temp = (-half_b + root) / a;
		if (temp<t_max && temp>t_min)
		{
			rec.t = temp;
			rec.p = r.at(temp);
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_noraml(r, outward_normal);
			return true;
		}
	}
	return false;
}