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

struct Canvas {
	int width;
	int height;
	Vector2 origin;
};

constexpr int bytes_per_pixel = 4;

void render(BackBuffer* buffer, Canvas canvas);

#endif // RENDERER_HPP
