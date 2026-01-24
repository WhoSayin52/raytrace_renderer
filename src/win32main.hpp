#ifndef WIN32MAIN_HPP
#define WIN32MAIN_HPP

#include <Windows.h>

struct Win32BackBuffer {
	BITMAPINFO info;
	void* memory;
	int width;
	int height;
	int pitch;
	int bytes_per_pixel;
};

struct Win32ViewportDimension {
	int width;
	int height;
};

#endif // WIN32MAIN_HPP
