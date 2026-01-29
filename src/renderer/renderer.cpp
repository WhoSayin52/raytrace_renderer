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
static constexpr Vector3f background_color = { 0.043f, 0.063f, 0.149f };
static constexpr f32 ESP = 0.001f;

// static global vars
static Collision light_collision{ // TODO: replace with arena allocator and local var
	.position = {},
	.normal = {},
	.sphere = nullptr,
};
static Collision shadow_collision{ // TODO: replace with arena allocator and local var
	.position = {},
	.normal = {},
	.sphere = nullptr,
};
static Camera camera{
	.position = Vector3f{0.0f, 0.0f, -1.0f},	// x, y, z
	.near = 1.0f,
	.far = FLT_MAX,
	.field_of_view = (f32)(80.0 * pi / 180.0),
	.aspect_ratio = 16.0f / 9.0f
};

// functions
static bool ray_cast(Collision* collision, Vector3f origin, Vector3f direction, f32 t_min, f32 t_max);
static Vector3f compute_color(Collision* collision, Vector3f ray_direction, int recursion_depth);
static Vector3f compute_directional_light(Collision* collision, Vector3f view_direction);
static Vector3f compute_point_light(Collision* collision, Vector3f view_direction);
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
			bool is_collision = ray_cast(&light_collision, camera.position, ray_direction, camera.near, camera.far);
			Vector3f color = is_collision ? compute_color(&light_collision, ray_direction, 3) : background_color;
			Vector3 rgb = to_rgb(color);
			set_pixel(canvas, x, y, rgb);
		}
	}
}

static bool ray_cast(Collision* collision, Vector3f origin, Vector3f direction, f32 t_min, f32 t_max) {
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
		return false;
	}
	else {
		collision->sphere = closest_sphere;
		collision->position = origin + direction * closest_t;
		collision->normal = normalize(collision->position - collision->sphere->position);

		return true;
	}
}

static Vector3f compute_color(Collision* collision, Vector3f ray_direction, int recursion_depth) {
	// using phong lighting model;
	Vector3f result{};

	Vector3f view_direction = -ray_direction;
	result = compute_directional_light(collision, view_direction);
	result += compute_point_light(collision, view_direction);

	f32 r = collision->sphere->material.reflectiveness;
	if (recursion_depth > 0 && r > 0) {
		bool has_collision = ray_cast(
			collision, collision->position,
			reflect(ray_direction, collision->normal),
			0.01f, camera.far
		);
		if (has_collision == false) {
			return result * (1 - r) + background_color * r;
		}
		else {
			return result * (1 - r) + compute_color(collision, reflect(ray_direction, collision->normal), recursion_depth - 1) * r;
		}
	}
	return result;
}

static Vector3f compute_directional_light(Collision* collision, Vector3f view_direction) {
	Vector3f result{};

	Vector3f normal = collision->normal;
	Material* material = &collision->sphere->material;
	for (int i = 0; i < ARRAY_COUNT(direct_lights); ++i) {
		DirectionalLight* light = &direct_lights[i];

		// shadow checking
		bool has_collision = ray_cast(
			&shadow_collision,
			collision->position + normal * ESP,
			-light->direction,
			ESP, camera.far);
		if (has_collision) {
			continue;
		}

		// ambient
		Vector3f ambient = material->color.ambient * light->light.ambient;

		// diffuse 
		// make sure light direction is already normalized
		f32 diffuse_strength = maximum(dot(normal, -light->direction), 0.0f);
		Vector3f diffuse = material->color.diffuse * light->light.diffuse * diffuse_strength;

		// specular TODO implement
		Vector3f reflection = reflect(light->direction, normal);
		f32 specular_strength = material->shininess ? pow(maximum(dot(view_direction, reflection), 0.0f), material->shininess) : 0;
		Vector3f specular = material->color.specular * light->light.specular * specular_strength;

		result += ambient + diffuse + specular;
	}
	return result;
}

static Vector3f compute_point_light(Collision* collision, Vector3f view_direction) {
	Vector3f result{};

	Vector3f normal = collision->normal;
	Material* material = &collision->sphere->material;
	for (int i = 0; i < ARRAY_COUNT(point_lights); ++i) {
		PointLight* light = &point_lights[i];

		Vector3f light_direction = collision->position - light->position;
		// shadow checking
		bool has_collision = ray_cast(
			&shadow_collision,
			collision->position + normal * ESP,
			-light_direction,
			ESP, 1.0f
		);
		if (has_collision) {
			continue;
		}
		f32 distance_from_light = length(light_direction);
		light_direction = normalize(light_direction);

		// attenuation
		f32 attenuation = 1.0f / (
			light->quadratic * distance_from_light +
			light->linear * distance_from_light +
			light->constant
			);

		// ambient 
		Vector3f ambient = material->color.ambient * light->light.ambient;

		// diffuse
		// make sure light direction is already normalized
		f32 diffuse_strength = maximum(dot(normal, -light_direction), 0.0f);
		Vector3f diffuse = material->color.diffuse * light->light.diffuse * diffuse_strength;

		// specular
		Vector3f reflection = reflect(light_direction, normal);
		f32 specular_strength = material->shininess ? pow(maximum(dot(view_direction, reflection), 0.0f), material->shininess) : 0;
		Vector3f specular = material->color.specular * light->light.specular * specular_strength;

		result += (ambient + diffuse + specular) * attenuation;
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

	Vector3f z_basis = normalize(-camera.position);
	Vector3f x_basis = normalize(cross(Vector3f{ 0.0f, 1.0f, 0.0f }, z_basis));
	Vector3f y_basis = cross(z_basis, x_basis);

	f32 px = x * ratio.w;
	f32 py = y * ratio.h;
	f32 pz = camera.near;

	return px * x_basis + py * y_basis + pz * z_basis;
	/*
	return Vector3f{
		x * ratio.w,
		y * ratio.h,
		camera.near  // distance between the camera and viewport
	};
	*/
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

