#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "../core.hpp"

// vectors2 structs
#pragma warning(push)
#pragma warning(disable: 4201)
struct Vector2 {
	union {
		struct { int x, y; };
		struct { int w, h; };
		struct { int min, max; };
		int data[2];
	};
};

struct Vector2f {
	union {
		struct { f32 x, y; };
		struct { f32 w, h; };
		struct { f32 min, max; };
		f32 data[2];
	};
};

// vector3 structs
struct Vector3 {
	union {
		struct { int x, y, z; };
		struct { int w, h, l; };
		struct { int r, g, b; };
		int data[3];
	};
};

struct Vector3f {
	union {
		struct { f32 x, y, z; };
		struct { f32 w, h, l; };
		struct { f32 r, g, b; };
		f32 data[3];
	};
};

// vector4 structs
struct Vector4 {
	union {
		struct { int x, y, z, w; };
		struct { int r, g, b, a; };
		int data[4];
	};
};
#pragma warning(pop)

// vector2 operator overloading
constexpr Vector2 operator+(Vector2 v) { return v; }
constexpr Vector2f operator+(Vector2f v) { return v; }
constexpr Vector2 operator-(Vector2 v) { return { -v.x, -v.y }; }
constexpr Vector2f operator-(Vector2f v) { return { -v.x, -v.y }; }

constexpr Vector2& operator+=(Vector2& v1, Vector2 v2) { (v1.x += v2.x); (v1.y += v2.y); return v1; }
constexpr Vector2f& operator+=(Vector2f& v1, Vector2f v2) { (v1.x += v2.x); (v1.y += v2.y); return v1; }

constexpr Vector2& operator-=(Vector2& v1, Vector2 v2) { (v1.x -= v2.x); (v1.y -= v2.y); return v1; }
constexpr Vector2f& operator-=(Vector2f& v1, Vector2f v2) { (v1.x -= v2.x); (v1.y -= v2.y); return v1; }

constexpr Vector2& operator*=(Vector2& v1, Vector2 v2) { (v1.x *= v2.x); (v1.y *= v2.y); return v1; }
constexpr Vector2f& operator*=(Vector2f& v1, Vector2f v2) { (v1.x *= v2.x); (v1.y *= v2.y); return v1; }

constexpr Vector2 operator+(Vector2 v1, Vector2 v2) { return { (v1.x + v2.x), (v1.y + v2.y) }; }
constexpr Vector2f operator+(Vector2f v1, Vector2f v2) { return { (v1.x + v2.x), (v1.y + v2.y) }; }

constexpr Vector2 operator-(Vector2 v1, Vector2 v2) { return { (v1.x - v2.x), (v1.y - v2.y) }; }
constexpr Vector2f operator-(Vector2f v1, Vector2f v2) { return { (v1.x - v2.x), (v1.y - v2.y) }; }

constexpr Vector2 operator*(Vector2 v1, Vector2 v2) { return { (v1.x * v2.x), (v1.y * v2.y) }; }
constexpr Vector2f operator*(Vector2f v1, Vector2f v2) { return { (v1.x * v2.x), (v1.y * v2.y) }; }

constexpr Vector2 operator*(Vector2 v, int scaler) { return { (v.x * scaler), (v.y * scaler) }; }
constexpr Vector2 operator*(int scaler, Vector2 v) { return v * scaler; }
constexpr Vector2f operator*(Vector2f v, f32 scaler) { return { (v.x * scaler), (v.y * scaler) }; }
constexpr Vector2f operator*(f32 scaler, Vector2f v) { return v * scaler; }

// vector3 operator overloading
constexpr Vector3 operator+(Vector3 v) { return v; }
constexpr Vector3f operator+(Vector3f v) { return v; }
constexpr Vector3 operator-(Vector3 v) { return { -v.x, -v.y, -v.z }; }
constexpr Vector3f operator-(Vector3f v) { return { -v.x, -v.y, -v.z }; }

constexpr Vector3& operator+=(Vector3& v1, Vector3 v2) { (v1.x += v2.x); (v1.y += v2.y); (v1.z += v2.z); return v1; }
constexpr Vector3f& operator+=(Vector3f& v1, Vector3f v2) { (v1.x += v2.x); (v1.y += v2.y); (v1.z += v2.z); return v1; }

constexpr Vector3& operator-=(Vector3& v1, Vector3 v2) { (v1.x -= v2.x); (v1.y -= v2.y); (v1.z -= v2.z); return v1; }
constexpr Vector3f& operator-=(Vector3f& v1, Vector3f v2) { (v1.x -= v2.x); (v1.y -= v2.y); (v1.z -= v2.z); return v1; }

constexpr Vector3& operator*=(Vector3& v1, Vector3 v2) { (v1.x *= v2.x); (v1.y *= v2.y); (v1.z *= v2.z); return v1; }
constexpr Vector3f& operator*=(Vector3f& v1, Vector3f v2) { (v1.x *= v2.x); (v1.y *= v2.y); (v1.z *= v2.z); return v1; }

constexpr Vector3 operator+(Vector3 v1, Vector3 v2) { return { (v1.x + v2.x), (v1.y + v2.y), (v1.z + v2.z) }; }
constexpr Vector3f operator+(Vector3f v1, Vector3f v2) { return { (v1.x + v2.x), (v1.y + v2.y), (v1.z + v2.z) }; }

constexpr Vector3 operator-(Vector3 v1, Vector3 v2) { return { (v1.x - v2.x), (v1.y - v2.y), (v1.z - v2.z) }; }
constexpr Vector3f operator-(Vector3f v1, Vector3f v2) { return { (v1.x - v2.x), (v1.y - v2.y), (v1.z - v2.z) }; }

constexpr Vector3 operator*(Vector3 v1, Vector3 v2) { return { (v1.x * v2.x), (v1.y * v2.y), (v1.z * v2.z) }; }
constexpr Vector3f operator*(Vector3f v1, Vector3f v2) { return { (v1.x * v2.x), (v1.y * v2.y), (v1.z * v2.z) }; }

constexpr Vector3 operator*(Vector3 v, int scaler) { return { (v.x * scaler), (v.y * scaler), (v.z * scaler) }; }
constexpr Vector3 operator*(int scaler, Vector3 v) { return v * scaler; }
constexpr Vector3f operator*(Vector3f v, f32 scaler) { return { (v.x * scaler), (v.y * scaler), (v.z * scaler) }; }
constexpr Vector3f operator*(f32 scaler, Vector3f v) { return v * scaler; }

// vector functions
constexpr int dot(Vector3 v1, Vector3 v2) {
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}
constexpr f32 dot(Vector3f v1, Vector3f v2) {
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}
constexpr Vector3 cross(Vector3 v1, Vector3 v2) {
	return { (v1.y * v2.z - v1.z * v2.y), (v1.z * v2.x - v1.x * v2.z), (v1.x * v2.y - v1.y * v2.x) };
}
constexpr Vector3f cross(Vector3f v1, Vector3f v2) {
	return { (v1.y * v2.z - v1.z * v2.y), (v1.z * v2.x - v1.x * v2.z), (v1.x * v2.y - v1.y * v2.x) };
}

f32 length(Vector2f v);
f32 length(Vector3f v);

Vector2f normalize(Vector2f v);
Vector3f normalize(Vector3f v);

constexpr Vector3f reflect(Vector3f vector, Vector3f normal) {
	return vector - (normal * (2.0f * dot(vector, normal)));
}

#endif // VECTOR_HPP
