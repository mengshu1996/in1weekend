#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "rtweekend.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

void avg_color(color& pixel_color, int samples_per_pixel)
{
	auto scale = 1.0 / samples_per_pixel;
	pixel_color[0] = sqrt(pixel_color[0] * scale);
	pixel_color[1] = sqrt(pixel_color[1] * scale);
	pixel_color[2] = sqrt(pixel_color[2] * scale);

	pixel_color[0] = static_cast<int>(256 * clamp(pixel_color[0], 0.0, 0.999));
	pixel_color[1] = static_cast<int>(256 * clamp(pixel_color[1], 0.0, 0.999));
	pixel_color[2] = static_cast<int>(256 * clamp(pixel_color[2], 0.0, 0.999));
}

color ray_color(const ray& r, const hittable& world, int depth)
{
	hit_record rec;
	if (depth <= 0)
		return color(0, 0, 0);
	if (world.hit(r, 0.001, infinity, rec))
	{
		point3 target = rec.p + rec.normal + random_in_hemisphere(rec.normal);
		return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth-1);
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
	const int samples_per_pixel = 100;
	const int max_depth = 50;
	int channel = 3;

	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	camera cam;
	unsigned char* data = new unsigned char[image_width * image_height * channel];
	for (int j = image_height - 1; j >= 0; --j)
	{
		for (int i = 0; i < image_width; ++i)
		{
			color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s)
			{
				
				auto u = double(i + random_double()) / (image_width - 1);
				auto v = double(j + random_double()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}
			avg_color(pixel_color, samples_per_pixel);
			data[(image_height - j - 1) * image_width * channel + i * channel] = pixel_color[0];
			data[(image_height - j - 1) * image_width * channel + i * channel + 1] = pixel_color[1];
			data[(image_height - j - 1) * image_width * channel + i * channel + 2] = pixel_color[2];
		}
	}
	stbi_write_jpg("in1wnd.jpg", image_width, image_height, channel, data, 100);
	std::cout << "finish.\n";
	//system("PAUSE");
}