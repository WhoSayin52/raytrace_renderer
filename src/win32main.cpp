#include "win32main.hpp"

#include "core.hpp"
#include "draw/draw.hpp"
#include "logger/logger.hpp"

// name of the window class we will register with the OS
constexpr wchar window_class_name[] = L"raytrace_renderer";
constexpr wchar window_title[] = L"Raytrace Renderer";

// back buffer vars
static constexpr int fixed_buffer_width = 640;
static constexpr int fixed_buffer_height = 360;

// win32 functions
static LRESULT win32_procedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

static bool win32_init_back_buffer(Win32BackBuffer* buffer, int width, int height);

static void win32_draw(HDC device_conext, Win32BackBuffer* buffer, Viewport viewport);

// procedure to be called to handle windowsOS messages loop
static LRESULT win32_procedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {

	static Win32BackBuffer back_buffer{};
	static Viewport viewport{};

	LRESULT result = 0;

	switch (message)
	{
	case WM_CREATE: {
		bool success = win32_init_back_buffer(&back_buffer, fixed_buffer_width, fixed_buffer_height);
		if (success == false) {
			LOG_ERROR("failed to init_back_buffer");
			result = -1;
		}

		// TODO remove
		for (int y = 100; y < 200; ++y) {
			for (int x = 100; x < 300; ++x) {
				set_pixel(&back_buffer, x, y, Vector3{ 0, 255, 0 });
			}
		}

	}break;

	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC device_context = BeginPaint(window, &ps);

		int x = 0;
		int y = 0;
		int width = ps.rcPaint.right - ps.rcPaint.left;
		int height = ps.rcPaint.bottom - ps.rcPaint.top;

		PatBlt(
			device_context,
			x, y, width, height,
			BLACKNESS
		);

		win32_draw(device_context, &back_buffer, viewport);

		EndPaint(window, &ps);
	}break;

	case WM_SIZE: {
		uint width = LOWORD(lparam);
		uint height = HIWORD(lparam);

		viewport.width = (int)width;
		viewport.height = (int)height;
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

static bool win32_init_back_buffer(Win32BackBuffer* buffer, int width, int height) {
	constexpr int bytes_per_pixel = 4;

	buffer->width = width;
	buffer->height = height;
	buffer->bytes_per_pixel = bytes_per_pixel;

	buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
	buffer->info.bmiHeader.biWidth = buffer->width;
	buffer->info.bmiHeader.biHeight = buffer->height;
	buffer->info.bmiHeader.biPlanes = 1;
	buffer->info.bmiHeader.biBitCount = (WORD)(buffer->bytes_per_pixel * bits_per_byte);
	buffer->info.bmiHeader.biCompression = BI_RGB;

	buffer->pitch = ALIGN4(width * bytes_per_pixel);

	int memory_size = buffer->height * buffer->pitch;

	buffer->memory = VirtualAlloc(0, memory_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	return buffer->memory != nullptr;
}

static void win32_draw(HDC device_conext, Win32BackBuffer* buffer, Viewport viewport) {

	StretchDIBits(
		device_conext,
		0, 0, viewport.width, viewport.height,
		0, 0, fixed_buffer_width, fixed_buffer_height,
		buffer->memory, &buffer->info, DIB_RGB_COLORS, SRCCOPY
	);
}

// win32 apps entry point
int WINAPI wWinMain(
	HINSTANCE process,
	HINSTANCE prev_instance,
	PWSTR cmd_args,
	int show_code) {

	// unused vars
	(void)prev_instance;
	(void)cmd_args;

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

	// instantiating registered window class and showing it
	HWND window = CreateWindowEx(
		0,
		window_class_name,
		window_title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, fixed_buffer_width, fixed_buffer_height,
		nullptr, nullptr,
		process,
		nullptr
	);

	if (window == nullptr) {
		LOG_ERROR("failed to create window");
		return 1;
	}

	ShowWindow(window, show_code);

	// handling windowsOS message loop
	MSG message{};
	while (GetMessage(&message, window, 0, 0) > 0) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	DWORD rc = GetLastError();
	if (rc != 0) {
		LOG_ERROR("error code: " + rc);
		return 1;
	}

	return 0;
}
