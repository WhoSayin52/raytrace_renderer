#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../core/core.hpp"

struct Rect {
	int x, y, width, height;
};

struct BackBuffer {
	void* memory;
	int width;
	int height;
	int pitch;
};

constexpr int bytes_per_pixel = 4;

void render(BackBuffer* buffer);

#endif // RENDERER_HPP
