#include "renderer.hpp"

#include"./scene.hpp"
#include "../utility/utility.hpp"

#include <cmath>
#include <climits>
#include <cfloat>

// structs
struct Collision {
	Vector3f position;
	Vector3f normal;
	Sphere* sphere;
};

struct Camera {
	Vector3f position;
	f32 near;			 // distance between camera and viewport. Anything in less gets clipped
	f32 far;				 // distance between camera and furthest object. Anything beyond get clipped
	f32 field_of_view; // in radians
	f32 aspect_ratio;  // viewport/canvas width divided by viewport/canvas height 
};

// static global constants
static constexpr Vector3 background_color = { 255, 255, 255 };

// static global vars
static Camera camera{
	.position = Vector3f{0.0f, 0.0f, 0.0f},	// x, y, z
	.near = 1.0f,
	.far = FLT_MAX,
	.field_of_view = (f32)(100.0 * pi / 180.0),
	.aspect_ratio = 16.0f / 9.0f
};

// functions
static Vector3 ray_cast(Vector3f origin, Vector3f direction, f32 t_min, f32 t_max);
static Collision* ray_intersection(Vector3f origin, Vector3f direction, f32 t_min, f32 t_max);
static Vector3 compute_color(Collision* collision, Vector3f direction);
static Vector2 screen_to_canvas(int x, int y, Vector2 origin);
static Vector3f canvas_to_viewport(int x, int y, Vector2f ratio);
static void set_pixel(Canvas* canvas, int x, int y, Vector3 rgb);

void render(Canvas* canvas) {
	Vector2f viewport;
	viewport.w = 2.0f * tan(camera.field_of_view / 2.0f);
	viewport.h = viewport.w / camera.aspect_ratio;

	Vector2f viewport_canvas_ratio;
	viewport_canvas_ratio.w = viewport.w / canvas->width;
	viewport_canvas_ratio.h = viewport.h / canvas->height;

	for (int y = 0; y < canvas->height; ++y) {
		for (int x = 0; x < canvas->width; ++x) {
			Vector2 canvas_position = screen_to_canvas(x, y, canvas->origin);
			Vector3f ray_direction = canvas_to_viewport(canvas_position.x, canvas_position.y, viewport_canvas_ratio);
			Vector3 color = ray_cast(camera.position, ray_direction, camera.near, camera.far);
			set_pixel(canvas, x, y, color);
		}
	}
}

static Vector3 ray_cast(Vector3f origin, Vector3f direction, f32 t_min, f32 t_max) {

	Collision* collision = ray_intersection(origin, direction, t_min, t_max);
	Vector3 color = compute_color(collision, direction);
}

static Collision* ray_intersection(Vector3f origin, Vector3f direction, f32 t_min, f32 t_max) {
	Collision* result = nullptr;

	f64 closest_t = t_max;
	Sphere* sphere;
	for (s64 i = 0; i < ARRAY_COUNT(spheres); ++i) {
		sphere = &spheres[i];

		// looking for the intersections between the ray and the sphere by
		// calculating the roots for the quadratic equation representing the intersection between 
		// the parametric line: point = camera_position + ray_direction * t and the sphere: point = center + radius
		Vector3f co = origin - sphere->position;
		f32 r = sphere->r;

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
			result->sphere = sphere;
		}
		if (t2 > t_min && t2 < closest_t) {
			closest_t = t2;
			result->sphere = sphere;
		}
	}

	if (result != nullptr) {
		result->position = origin + direction * closest_t;
		result->normal = normalize(result->position - result->sphere->position);
	}
	return result;
}

static Vector3 compute_color(Collision* collision, Vector3f direction) {
	if (collision == nullptr) {
		return background_color;
	}

	Vector3 sphere_color = collision->sphere->color;

	f32 diffuse = dot(collision->normal, -direction);
}

static Vector2 screen_to_canvas(int x, int y, Vector2 origin) {
	return Vector2{
		x - origin.x,
		y - origin.y
	};
}

static Vector3f canvas_to_viewport(int x, int y, Vector2f ratio) {
	return Vector3f{
		x * ratio.w,
		y * ratio.h,
		camera.near  // distance between the camera and viewport
	};
}

static void set_pixel(Canvas* canvas, int x, int y, Vector3 rgb) {
	ASSERT(
		(canvas->width > x) &&
		(canvas->height > y) &&
		(x >= 0) &&
		(y >= 0)
	);

	u32* pixel = (u32*)((byte*)canvas->memory + (y * canvas->pitch) + (x * bytes_per_pixel));

	*pixel = (rgb.r << 16) | (rgb.g << 8) | rgb.b;
}

