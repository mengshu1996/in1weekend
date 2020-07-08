#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "rtweekend.h"
#include "hittable_list.h"
#include "sphere.h"

//double hit_sphere(const point3& center, double radius, const ray& r)
//{
//	vec3 oc = r.origin() - center;
//	auto a = dot(r.direction(), r.direction());
//	auto half_b = dot(oc, r.direction());
//	auto c = dot(oc, oc) - radius * radius;
//	auto discriminator = half_b * half_b -  a * c;
//
//	if (discriminator < 0)
//		return -1.0;
//	else
//		return (-half_b - sqrt(discriminator)) /  a;
//}
color ray_color(const ray& r, const hittable& world)
{
	hit_record rec;
	if (world.hit(r, 0, infinity, rec))
	{
		return 0.5 * (rec.normal + color(1, 1, 1));
	}
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 384;
	const int image_height = static_cast<int>(image_width / aspect_ratio);

	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;
	auto focal_length = 1.0;

	auto origin = point3(0, 0, 0);
	auto horizontal = vec3(viewport_width, 0, 0);
	auto vertical = vec3(0, viewport_height, 0);
	auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
	int channel = 3;
	
	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	unsigned char* data = new unsigned char[image_width * image_height * channel];
	for (int j = image_height - 1; j >= 0; --j)
	{
		for (int i = 0; i < image_width; ++i)
		{
			auto u = double(i) / (image_width - 1);
			auto v = double(j) / (image_height - 1);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			color pixel_color = ray_color(r, world);

			//color pixel_color(double(i) / double(image_width), double(j) / double(image_height), 0.25);
			int ir = int(255.99 * pixel_color[0]);
			int ig = int(255.99 * pixel_color[1]);
			int ib = int(255.99 * pixel_color[2]);
			data[(image_height - j - 1) * image_width * channel + i * channel] = ir;
			data[(image_height - j - 1) * image_width * channel + i * channel + 1] = ig;
			data[(image_height - j - 1) * image_width * channel + i * channel + 2] = ib;
		}
	}
	stbi_write_jpg("in1wnd.jpg", image_width, image_height, channel, data, 100);
	std::cout << "finish.\n";
	//system("PAUSE");
}