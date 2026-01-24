#include "renderer.hpp"

// structs
struct Camera {
	Vector3 position;
};

struct Sphere {
	Vector3 position;
	int r;
};

// vars
Camera camera{ Vector3{0, 0, 0} };

static Sphere scene[1] = {
	Sphere{
		.position = Vector3{0, 1, 1},
		.r = 1
	},
};

// functions
static Vector3 trace_ray(Vector3 from, Vector3 to, Vector2 length);
static Vector3 buffer_to_viewport(Vector2 buffer, Vector2 viewport);
//static void set_pixel(BackBuffer* buffer, int x, int y, Vector3 rgb); // TODO: remove

void render(BackBuffer* buffer, Vector2 viewport_size) {

	int buffer_w = buffer->width;
	int buffer_h = buffer->height;
	int buffer_p = buffer->pitch;
	u32* buffer = (u32*)buffer->memory;


	for (int y = 0; y < buffer_h; ++y) {
		for (int x = 0; x < buffer_w; ++x) {
			Vector3 to = buffer_to_viewport(Vector2{ buffer_w, buffer_h }, viewport_size);
			Vector2 ray_length = { 1, INT_MAX };
			Vector3 color = trace_ray(camera.position, to, ray_length);
		}
	}
}

static Vector3 trace_ray(Vector3 from, Vector3 to, Vector2 length) {

}

static Vector3 buffer_to_viewport(Vector2 buffer, Vector2 viewport) {
	return Vector3{ viewport.w / buffer.w, viewport.h / buffer.h, 1 }; // 1 is the distance between the camera and viewport
}



/*
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
*/
