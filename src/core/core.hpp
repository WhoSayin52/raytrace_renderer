#ifndef CORE_HPP
#define CORE_HPP

#include <cstdint>
#include <cstddef>

// types
using s32 = int32_t;
using s64 = int64_t;

using uint = unsigned int;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

using uchar = unsigned char;
using wchar = wchar_t;

using bool32 = s32;

using byte = uchar;

// math
#pragma warning(push)
#pragma warning(disable: 4201)
// vectors2
struct Vector2 {
	union {
		struct { int x, y; };
		struct { int w, h; };
		struct { int min, max; };
		int data[2];
	};
};
Vector2 operator+(Vector2 v1, Vector2 v2);
Vector2 operator-(Vector2 v1, Vector2 v2);

struct Vector2f {
	union {
		struct { f32 x, y; };
		struct { f32 w, h; };
		struct { f32 min, max; };
		f32 data[2];
	};
};
Vector2f operator+(Vector2f v1, Vector2f v2);
Vector2f operator-(Vector2f v1, Vector2f v2);

// vector3
struct Vector3 {
	union {
		struct { int x, y, z; };
		struct { int w, h, l; };
		struct { int r, g, b; };
		int data[3];
	};
};
Vector3 operator+(Vector3 v1, Vector3 v2);
Vector3 operator-(Vector3 v1, Vector3 v2);

struct Vector3f {
	union {
		struct { f32 x, y, z; };
		struct { f32 w, h, l; };
		struct { f32 r, g, b; };
		f32 data[3];
	};
};
Vector3f operator+(Vector3f v1, Vector3f v2);
Vector3f operator-(Vector3f v1, Vector3f v2);

struct Vector4 {
	union {
		struct { int x, y, z, w; };
		struct { int r, g, b, a; };
		int data[4];
	};
};
#pragma warning(pop)

// vector functions
int dot(Vector3 v1, Vector3 v2);
f32 dot(Vector3f v1, Vector3f v2);

// math constants
constexpr f64 pi = 3.141592653589793;

// memory
constexpr u64 kilobytes(u64 bytes) { return bytes * 1024LL; }
constexpr u64 megabytes(u64 bytes) { return kilobytes(bytes) * 1024LL; }
constexpr u64 gigabytes(u64 bytes) { return megabytes(bytes) * 1024LL; }
constexpr u64 terabytes(u64 bytes) { return gigabytes(bytes) * 1024LL; }

#define ALIGN4(bytes) ((bytes) + 3) & ~3;

// debugging
#if !defined(NDEBUG)
#	define ASSERT(x)     \
		if(!(x)){         \
			*(int*)0 = 0;  \
		}
#else
#	define ASSERT(x)
#endif // !defined(NDEBUG)

// utility
#define ARRAY_COUNT(arr) (sizeof((arr)) / sizeof((arr)[0]))

#endif // CORE_HPP
