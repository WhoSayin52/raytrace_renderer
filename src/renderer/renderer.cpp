#include "renderer.hpp"



struct Rect {
	int x;
	int y;
	int width;
	int height;
};



void render(BackBuffer buffer) {
	(void)buffer;
}

/*
static void set_pixel(BackBuffer* buffer, int x, int y, Vector3 rgb);

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
