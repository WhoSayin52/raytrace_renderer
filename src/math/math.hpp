#ifndef MATH_HPP
#define MATH_HPP

#include "vector.hpp"

#include "../core.hpp"

// math constants
constexpr f64 pi = 3.141592653589793;

template <typename T>
T minimum(T a, T b) {
	return (a < b) ? a : b;
}

template <typename T>
T maximum(T a, T b) {
	return (a > b) ? a : b;
}

template <typename T>
T clamp(T value, T min, T max) {
	return minimum(maximum(value, min), max);
}

#endif // MATH_HPP
