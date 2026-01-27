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
	bool has_collided; // TODO: remove bool field once replace by arena allocator
};

struct Camera {
	Vector3f position;
	f32 near;			 // distance between camera and viewport. Anything in less gets clipped
	f32 far;				 // distance between camera and furthest object. Anything beyond get clipped
	f32 field_of_view; // in radians
	f32 aspect_ratio;  // viewport/canvas width divided by viewport/canvas height 
};

// static global constants
static constexpr Vector3f background_color = { 1.0f, 1.0f, 1.0f };

// static global vars
static Collision collision{ // TODO: replace with arena allocator
	.position = {},
	.normal = {},
	.sphere = nullptr,
	.has_collided = false
};
static Camera camera{
	.position = Vector3f{0.0f, 0.0f, 0.0f},	// x, y, z
	.near = 1.0f,
	.far = FLT_MAX,
	.field_of_view = (f32)(100.0 * pi / 180.0),
	.aspect_ratio = 16.0f / 9.0f
};

// functions
static Vector3f ray_cast(Vector3f origin, Vector3f direction, f32 t_min, f32 t_max);
static void ray_intersection(Vector3f origin, Vector3f direction, f32 t_min, f32 t_max);
static Vector3f compute_color(Vector3f ray_direction);
static Vector3f compute_directional_light(Vector3f view_direction);
//static Vector3f compute_point_light();
static Vector2 screen_to_canvas(int x, int y, Vector2 origin);
static Vector3f canvas_to_viewport(int x, int y, Vector2f ratio);
static Vector3 to_rgb(Vector3f color);
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
			ray_direction = normalize(ray_direction);
			Vector3f color = ray_cast(camera.position, ray_direction, camera.near, camera.far);
			Vector3 rgb = to_rgb(color);
			set_pixel(canvas, x, y, rgb);
		}
	}
}

static Vector3f ray_cast(Vector3f origin, Vector3f direction, f32 t_min, f32 t_max) {
	ray_intersection(origin, direction, t_min, t_max);
	Vector3f color = compute_color(direction);

	return color;
}

static void ray_intersection(Vector3f origin, Vector3f direction, f32 t_min, f32 t_max) {
	Sphere* closest_sphere = nullptr;
	f32 closest_t = t_max;

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
			closest_sphere = sphere;
		}
		if (t2 > t_min && t2 < closest_t) {
			closest_t = t2;
			closest_sphere = sphere;
		}
	}

	if (closest_sphere == nullptr) {
		collision.has_collided = false;
	}
	else {
		collision.has_collided = true;
		collision.sphere = closest_sphere;
		collision.position = origin + direction * closest_t;
		collision.normal = normalize(collision.position - collision.sphere->position);
	}
}

static Vector3f compute_color(Vector3f ray_direction) {
	// using phong lighting model;
	if (collision.has_collided == false) {
		return background_color;
	}

	Vector3f result = compute_directional_light(-ray_direction);

	return result;
}

static Vector3f compute_directional_light(Vector3f view_direction) {
	Vector3f result{};

	Vector3f normal = collision.normal;
	Material* material = &collision.sphere->material;
	for (int i = 0; i < ARRAY_COUNT(direct_lights); ++i) {
		DirectionalLight* light = &direct_lights[i];

		// ambient
		Vector3f ambient = material->diffuse * light->light.ambient;

		// diffuse 
		// make sure light direction is already normalized
		f32 diffuse_strength = maximum(dot(normal, -light->direction), 0.0f);
		Vector3f diffuse = material->diffuse * light->light.diffuse * diffuse_strength;

		// specular TODO implement
		Vector3f reflection = reflect(light->direction, normal);
		f32 specular_strength = material->shininess ? pow(maximum(dot(view_direction, reflection), 0.0f), material->shininess) : 0;
		Vector3f specular = material->specular * specular_strength;

		result += ambient + diffuse + specular;
	}

	return result;
}

static Vector2 screen_to_canvas(int x, int y, Vector2 origin) {
	return Vector2{
		x - origin.x,
		y - origin.y
	};
}

static Vector3f canvas_to_viewport(int x, int y, Vector2f ratio) {
	return Vector3f{
		x * ratio.w - camera.position.x,
		y * ratio.h - camera.position.y,
		camera.near  // distance between the camera and viewport
	};
}

static Vector3 to_rgb(Vector3f color) {
	return Vector3{
		(int)(clamp(color.r, 0.0f, 1.0f) * 255),
		(int)(clamp(color.g, 0.0f, 1.0f) * 255),
		(int)(clamp(color.b, 0.0f, 1.0f) * 255)
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

