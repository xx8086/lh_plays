#pragma once

#include "ray.h"
#include <random>


class camera{
public:
	camera();
	~camera();
public:
	void update_lookfrom(vec3 lookfrom);
	void set_camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist);
	ray get_ray(float s, float t);
private:
	vec3 random_in_unit_disk();

private:
	vec3 _lower_left_corner;
	vec3 _horizontal;
	vec3 _vertical;
	vec3 _origin;
	vec3 _lookat;
	vec3 _vup;
	vec3 _u;
	vec3 _v;
	vec3 _w;
	float _theta;
	float _focus_dist;
	float _lens_radius;
	float _half_height = 0.0f;
	float _half_width = 0.0f;
	std::mt19937* _e;
	std::uniform_real_distribution<double> _ur;
};

