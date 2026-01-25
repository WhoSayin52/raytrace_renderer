#include "core.hpp"

// Vector 2 
Vector2 operator+(Vector2 v1, Vector2 v2) {
	return { v1.x + v2.x, v1.y + v2.y };
}

Vector2 operator-(Vector2 v1, Vector2 v2) {
	return { v1.x - v2.x, v1.y - v2.y };
}

Vector2f operator+(Vector2f v1, Vector2f v2) {
	return { v1.x + v2.x, v1.y + v2.y };
}

Vector2f operator-(Vector2f v1, Vector2f v2) {
	return { v1.x - v2.x, v1.y - v2.y };
}

// Vector 3
Vector3 operator+(Vector3 v1, Vector3 v2) {
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

Vector3 operator-(Vector3 v1, Vector3 v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

Vector3f operator+(Vector3f v1, Vector3f v2) {
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

Vector3f operator-(Vector3f v1, Vector3f v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

int dot(Vector3 v1, Vector3 v2) {
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

f32 dot(Vector3f v1, Vector3f v2) {
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}
