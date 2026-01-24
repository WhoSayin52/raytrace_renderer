#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../core.hpp"

struct BackBuffer {
	void* memory;
	int width;
	int height;
	int pitch;
};

constexpr int bytes_per_pixel = 4;

void render(BackBuffer buffer);

#endif // RENDERER_HPP
