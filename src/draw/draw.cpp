#include "draw.hpp"

#include "../win32main.hpp"

void set_pixel(Win32BackBuffer* buffer, int x, int y, Vector3 rgb) {
	ASSERT(
		(buffer->width > x) &&
		(buffer->height > y) &&
		(x >= 0) &&
		(y >= 0)
	);

	u32* pixel = (u32*)((byte*)buffer->memory + (y * buffer->pitch) + (x * buffer->bytes_per_pixel));

	*pixel = (rgb.b << 16) | (rgb.g << 8) | rgb.r;
}
