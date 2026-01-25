#include "renderer.hpp"

#include "../utility/utility.hpp"

#include <cmath>

// structs
struct Camera {
	Vector3f position;
};

struct Sphere {
	Vector3f position;
	Vector3 color;
	f32 r;
};

// static global constants
constexpr Vector3 background_color = { 255, 255, 255 };

// static global vars
static Camera camera{ .position = Vector3f{0, 0, 0} };
static Sphere scene[1] = {
	Sphere{
		.position = Vector3f{0.0f, 0.0f, 110.0f},
		.color = Vector3{0, 255, 0},
		.r = 100.0f
	},
};

// functions
static Vector3 trace_ray(Vector3f origin, Vector3f direction, int t_min, int t_max);
static Vector3f canvas_to_viewport(int x, int y, Canvas canvas);
static void set_pixel(BackBuffer* buffer, int x, int y, Vector3 rgb);

void render(BackBuffer* buffer, Canvas canvas) {

	for (int y = 0; y < canvas.height; ++y) {
		for (int x = 0; x < canvas.width; ++x) {
			Vector3f ray_direction = canvas_to_viewport(x, y, canvas);
			Vector3 color = trace_ray(camera.position, ray_direction, 1, INT_MAX);
			set_pixel(buffer, x, y, color);
		}
	}
}

static Vector3 trace_ray(Vector3f origin, Vector3f direction, int t_min, int t_max) {
	Vector3 result = background_color;
	f64 closest_t = t_max;

	Sphere sphere;
	for (s64 i = 0; i < ARRAY_COUNT(scene); ++i) {
		sphere = scene[i];

		// looking for the intersections between the ray and the sphere by
		// calculating the roots for the quadratic equation representing the intersection between 
		// the parametric line: point = camera_position + ray_direction * t and the sphere: point = center + radius
		Vector3f co = origin - sphere.position;
		int r = sphere.r;

		f32 a = dot(direction, direction);
		f32 b = dot(co, direction) * 2;
		f32 c = dot(co, co) - (r * r);

		f32 delta = (b * b) - (4 * a * c);

		if (delta < 0.0) {
			continue;
		}

		f32 t1 = (-b + sqrt(delta)) / (2 * a);
		f32 t2 = (-b - sqrt(delta)) / (2 * a);

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
// TODO: viewport is in world units;
static Vector3f canvas_to_viewport(int x, int y, Canvas canvas) {
	return Vector3f{
		(f32)(x - canvas.origin.x),
		(f32)(y - canvas.origin.y),
		1.0f // 1 is the distance between the camera and viewport
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

