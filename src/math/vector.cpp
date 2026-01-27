#include "vector.hpp"

#include <cmath>

// vector functions
Vector2f normalize(Vector2f v) {
	f32 magnitude = sqrt((v.x * v.x) + (v.y * v.y));
	return (magnitude == 0) ? Vector2f{ 0.0f, 0.0f } : Vector2f{ (v.x / magnitude), (v.y / magnitude) };
}
Vector3f normalize(Vector3f v) {
	f32 magnitude = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	return (magnitude == 0) ? Vector3f{ 0.0f, 0.0f, 0.0f } : Vector3f{ (v.x / magnitude), (v.y / magnitude), (v.z / magnitude) };
}
