#include "renderer.hpp"

#include "../utility/utility.hpp"

#include <cmath>

// structs
struct Camera {
	Vector3 position;
};

struct Sphere {
	Vector3 position;
	Vector3 color;
	int r;
};

// static global constants
constexpr Vector3 background_color = { 255, 255, 255 };

// static global vars
static Camera camera{ Vector3{0, 0, 0} };
static Sphere scene[1] = {
	Sphere{
		.position = Vector3{0, 0, 110},
		.color = Vector3{0, 255, 0},
		.r = 100
	},
};

// functions
static Vector3 trace_ray(Vector3 origin, Vector3 direction, int t_min, int t_max);
static Vector3 buffer_to_viewport(int x, int y, Vector2 origin);
static void set_pixel(BackBuffer* buffer, int x, int y, Vector3 rgb);

void render(BackBuffer* buffer) {

	int buffer_w = buffer->width;
	int buffer_h = buffer->height;

	Vector2 origin;
	origin.x = buffer_w / 2;
	origin.y = buffer_h / 2;

	for (int y = 0; y < buffer_h; ++y) {
		for (int x = 0; x < buffer_w; ++x) {
			Vector3 ray_direction = buffer_to_viewport(x, y, origin);
			Vector3 color = trace_ray(camera.position, ray_direction, 1, INT_MAX);
			set_pixel(buffer, x, y, color);
		}
	}
}

static Vector3 trace_ray(Vector3 origin, Vector3 direction, int t_min, int t_max) {
	Vector3 result = background_color;
	f64 closest_t = t_max;

	Sphere sphere;
	for (s64 i = 0; i < ARRAY_COUNT(scene); ++i) {
		sphere = scene[i];

		// looking for the intersections between the ray and the sphere by
		// calculating the roots for the quadratic equation representing the intersection between 
		// the parametric line: point = camera_position + ray_direction * t and the sphere: point = center + radius
		Vector3 co = origin - sphere.position;
		int r = sphere.r;

		f64 a = (f64)(dot(direction, direction));
		f64 b = (f64)(dot(co, direction) * 2);
		f64 c = (f64)(dot(co, co) - (r * r));

		f64 delta = (b * b) - (4 * a * c);

		if (delta < 0.0) {
			continue;
		}

		f64 t1 = (-b + sqrt(delta)) / (2 * a);
		f64 t2 = (-b - sqrt(delta)) / (2 * a);

		if (t1 > t_min && t1 < closest_t) {
			closest_t = t1;
			result = sphere.color;
		}
		if (t2 > t_min && t2 < closest_t) {
			closest_t = t2;
			result = sphere.color;
		}
	}

	return result;
}

static Vector3 buffer_to_viewport(int x, int y, Vector2 origin) {
	return Vector3{
		(x - origin.x),
		(y - origin.y),
		1 // 1 is the distance between the camera and viewport
	};
}

static void set_pixel(BackBuffer* buffer, int x, int y, Vector3 rgb) {
	ASSERT(
		(buffer->width > x) &&
		(buffer->height > y) &&
		(x >= 0) &&
		(y >= 0)
	);

	u32* pixel = (u32*)((byte*)buffer->memory + (y * buffer->pitch) + (x * bytes_per_pixel));

	*pixel = (rgb.b << 16) | (rgb.g << 8) | rgb.r;
}

