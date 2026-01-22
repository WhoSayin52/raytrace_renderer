#ifndef WIN32MAIN_HPP
#define WIN32MAIN_HPP

#include <Windows.h>

struct Win32BackBuffer {
	BITMAPINFO info;
	void* memory;
	int width;
	int height;
	int bytes_per_pixel;
	int pitch;
};

#endif // WIN32MAIN_HPP
