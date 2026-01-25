#include "win32main.hpp"

#include "./core/core.hpp"
#include "./logger/logger.hpp"
#include "./renderer/renderer.hpp"

// TODO: try to remove from global space
// static global constants
static constexpr wchar window_class_name[] = L"raytrace_renderer";
static constexpr wchar window_title[] = L"Raytrace Renderer";

// win32 functions
static LRESULT win32_procedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
static void win32_update_window(HDC device_conext, Win32BackBuffer* buffer, Canvas canvas);
static bool win32_init_back_buffer(Win32BackBuffer* buffer, int width, int height);

// win32 apps entry point
int WINAPI wWinMain(
	HINSTANCE process,
	HINSTANCE prev_instance,
	PWSTR cmd_args,
	int show_code) {

	// unused vars
	(void)prev_instance;
	(void)cmd_args;
	(void)show_code;

	// creating and registering a window class
	WNDCLASS window_class{};
	window_class.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	window_class.lpfnWndProc = win32_procedure;
	window_class.hInstance = process;
	window_class.lpszClassName = window_class_name;

	ATOM window_class_id = RegisterClass(&window_class);
	if (window_class_id == 0) {
		LOG_ERROR("failed to register window class");
		return 1;
	}

	Canvas canvas{
		.width = 1280,
		.height = 720,
		.origin = Vector2{ 1280 / 2, 720 / 2 }
	};

	RECT client_rect = { 0, 0, canvas.width, canvas.height };
	DWORD style = WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION;
	AdjustWindowRect(&client_rect, style, 0);

	// instantiating registered window class and showing it
	HWND window = CreateWindowEx(
		0,
		window_class_name,
		window_title,
		style,
		CW_USEDEFAULT, 								// x
		CW_USEDEFAULT, 								// y
		client_rect.right - client_rect.left, 	// w
		client_rect.bottom - client_rect.top,	// h
		nullptr, nullptr,
		process,
		nullptr
	);

	if (window == nullptr) {
		LOG_ERROR("failed to create window");
		return 1;
	}

	ShowWindow(window, SW_SHOWNORMAL);

	// vars required for main loop
	Win32BackBuffer win32_back_buffer{};
	bool success = win32_init_back_buffer(&win32_back_buffer, canvas.width, canvas.height);
	if (success == false) {
		LOG_ERROR("failed to init_back_buffer");
		return 1;
	}

	BackBuffer buffer{
		.memory = win32_back_buffer.memory,
		.width = win32_back_buffer.width,
		.height = win32_back_buffer.height,
		.pitch = win32_back_buffer.pitch
	};

	// main loop
	MSG message{};
	bool is_running = true;
	while (is_running) {
		// handling windowsOS message loop
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		// rendering
		render(&buffer, canvas);

		HDC device_conext = GetDC(window);
		win32_update_window(device_conext, &win32_back_buffer, canvas);
		ReleaseDC(window, device_conext);
	}

	DWORD rc = GetLastError();
	if (rc != 0) {
		LOG_ERROR("error code: " + rc);
		return 1;
	}
	return 0;
}

// procedure to be called to handle windowsOS messages loop
static LRESULT win32_procedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {

	LRESULT result = 0;

	switch (message)
	{
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC device_context = BeginPaint(window, &ps);
		PatBlt(device_context, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, BLACKNESS);
		EndPaint(window, &ps);
	}break;

	case WM_CLOSE: {
		if (MessageBox(window, L"Are you sure you want to quit?", window_title, MB_OKCANCEL) == IDOK) {
			DestroyWindow(window);
		}
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}break;

	default: {
		result = DefWindowProc(window, message, wparam, lparam);
	}break;
	}

	return result;
}

static void win32_update_window(HDC device_context, Win32BackBuffer* buffer, Canvas canvas) {
	StretchDIBits(
		device_context,
		0, 0, canvas.width, canvas.height,  	//dest
		0, 0, buffer->width, buffer->height, 	// src
		buffer->memory, &buffer->info, DIB_RGB_COLORS, SRCCOPY
	);
}

static bool win32_init_back_buffer(Win32BackBuffer* buffer, int width, int height) {
	ASSERT(width >= 0 && height >= 0 && buffer->memory == nullptr);
	int bits_per_byte = 8;

	buffer->width = width;
	buffer->height = height;

	buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
	buffer->info.bmiHeader.biWidth = buffer->width;
	buffer->info.bmiHeader.biHeight = buffer->height;
	buffer->info.bmiHeader.biPlanes = 1;
	buffer->info.bmiHeader.biBitCount = (WORD)(bytes_per_pixel * bits_per_byte);
	buffer->info.bmiHeader.biCompression = BI_RGB;

	buffer->pitch = ALIGN4(width * bytes_per_pixel);

	int memory_size = buffer->height * buffer->pitch;

	buffer->memory = VirtualAlloc(0, memory_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	return buffer->memory != nullptr;
}


