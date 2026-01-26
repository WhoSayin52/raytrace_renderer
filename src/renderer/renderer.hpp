#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../core/core.hpp"

struct Canvas {
	void* memory;
	int width;
	int height;
	int pitch;
	Vector2 origin;
};

constexpr int bytes_per_pixel = 4;

void render(Canvas* canvas);

#endif // RENDERER_HPP
