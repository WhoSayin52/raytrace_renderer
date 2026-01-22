#include "draw.hpp"

#include "../win32main.hpp"

void set_pixel(Win32BackBuffer* buffer, int x, int y, Vector4 rgba) {
	ASSERT(
		(buffer->width > x) &&
		(buffer->height > y) &&
		(x >= 0) &&
		(y >= 0)
	);

	u32* pixel = (u32*)((byte*)buffer->memory + (y * buffer->pitch) + (x * buffer->bytes_per_pixel));

	*pixel = (rgba.a << 24) | (rgba.b << 16) | (rgba.g << 8) | rgba.r;
}
