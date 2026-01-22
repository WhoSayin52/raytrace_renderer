#ifndef DRAW_HPP
#define DRAW_HPP

#include "../core.hpp"

struct Win32BackBuffer;

struct Rect {
	int x;
	int y;
	int width;
	int height;
};

void set_pixel(Win32BackBuffer* buffer, int x, int y, Vector4 rgba);

#endif // DRAW_HPP
