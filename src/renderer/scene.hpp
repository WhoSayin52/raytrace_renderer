#ifndef SCENE_HPP
#define SCENE_HPP

#include "../core.hpp"
#include "../math/math.hpp"

struct LightComponent {
	Vector3f ambient;
	Vector3f diffuse;
	Vector3f specular;
};

struct DirectionalLight {
	Vector3f direction;
	LightComponent light;
};

struct PointLight {
	Vector3f position;
	LightComponent light;
	f32 constant; // always one
	f32 linear;
	f32 quadratic;
};

struct Material {
	LightComponent color;
	f32 shininess;
	f32 reflectiveness;
};

struct Rect {
	int x, y, width, height;
};

struct Sphere {
	Vector3f position;
	f32 r;
	Material material;
};

DirectionalLight direct_lights[] = {
	DirectionalLight{
		.direction = normalize(Vector3f{-1.0f, -2.0f, 4.0f}),
		.light = LightComponent{
			.ambient = {0.2f, 0.2f, 0.2f},
			.diffuse = {0.4f, 0.4f, 0.4f},
			.specular = {0.4f, 0.4f, 0.4f}
		}
	},
};

PointLight point_lights[] = {
	PointLight{
		.position = Vector3f{-2.0f, 1.0f, 0.0f},
		.light = LightComponent{
			.ambient = {0.1f, 0.1f, 0.1f},
			.diffuse = {0.6f, 0.6f, 0.6f},
			.specular = {0.6f, 0.6f, 0.6f}
		},
		.constant = 1.0f,
		.linear = 0.35f,
		.quadratic = 0.44f
	},
};

inline Sphere spheres[] = {
	Sphere{
		.position = Vector3f{0.0f, -1.0f, 3.0f},
		.r = 1.0f,
		.material = Material{
			.color = LightComponent{
				.ambient = {0.0f, 0.0f, 0.2f},
				.diffuse = {0.0f, 0.0f, 1.0f},
				.specular = {0.0f, 0.0f, 1.0f}
			},
			.shininess = 500.0f,
			.reflectiveness = 0.2f
		} // blue
	},
	Sphere{
		.position = Vector3f{2.0f, 0.0f, 4.0f},
		.r = 1.0f,
		.material = Material{
			.color = LightComponent{
				.ambient = {0.2f, 0.0f, 0.0f},
				.diffuse = {1.0f, 0.0f, 0.0f},
				.specular = {1.0f, 0.0f, 0.0f}
			},
			.shininess = 500.0f,
			.reflectiveness = 0.3f
		} // red
	},
	Sphere{
		.position = Vector3f{-2.0f, 0.0f, 4.0f},
		.r = 1.0f,
		.material = Material{
			.color = LightComponent{
				.ambient = {0.0f, 0.2f, 0.0f},
				.diffuse = {0.0f, 1.0f, 0.0f},
				.specular = {0.0f, 1.0f, 0.0f}
			},
			.shininess = 10.0f,
			.reflectiveness = 0.4f
		} // green
	},
	Sphere{
		.position = Vector3f{0.0f, -1001.0f, 0.0f},
		.r = 1000.0f,
		.material = Material{
			.color = LightComponent{
				.ambient = {0.2f, 0.2f, 0.2f},
				.diffuse = {1.0f, 1.0f, 1.0f},
				.specular = {1.0f, 1.0f, 1.0f}
			},
			.shininess = 500.0f,
			.reflectiveness = 0.5f
		} // ground
	},
};


#endif // SCENE_HPP
