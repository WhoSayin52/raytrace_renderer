#ifndef SCENE_HPP
#define SCENE_HPP

#include "../core/core.hpp"

struct AmbientLight {
	f32 intensity;
	Vector3 color;
};

struct PointLight {
	Vector3f position;
	f32 intensity;
	Vector3 color;
};

struct DirectionalLight {
	Vector3f direction;
	f32 intensity;
	Vector3 color;
};

AmbientLight ambient_lights[] = {
	AmbientLight{
		.intensity = 0.20f,
		.color = Vector3{255, 255, 255}
	}
};

PointLight point_lights[] = {
	PointLight{
		.position = Vector3f{2, 1, 0},
		.intensity = 0.60f,
		.color = Vector3{255, 255, 255}
	}
};

DirectionalLight direct_lights[] = {
	DirectionalLight{
		.direction = Vector3f{1, 4, 4},
		.intensity = 0.20f,
		.color = Vector3{255, 255, 255}
	}
};

struct Rect {
	int x, y, width, height;
};

struct Sphere {
	Vector3f position;
	f32 r;
	Vector3 color;
};

inline Sphere spheres[] = {
	Sphere{
		.position = Vector3f{0.0f, -1.0f, 3.0f},
		.r = 1.0f,
		.color = Vector3{255, 0, 0} // red
	},
	Sphere{
		.position = Vector3f{2.0f, 0.0f, 4.0f},
		.r = 1.0f,
		.color = Vector3{0, 0, 255} // blue
	},
	Sphere{
		.position = Vector3f{-2.0f, 0.0f, 4.0f},
		.r = 1.0f,
		.color = Vector3{0, 255, 0} // green
	},
	Sphere{
		.position = Vector3f{0.0f, -5001.0f, 0.0f},
		.r = 5000.0f,
		.color = Vector3{255, 255, 0} // yellow
	},
};

#endif // SCENE_HPP
