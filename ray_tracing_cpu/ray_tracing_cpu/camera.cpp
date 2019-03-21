#include "camera.h"

camera::camera() : _ur(0.0, 1.0) {
	_e = new std::mt19937;
	_e->seed(std::random_device()());
}

void camera::update_lookfrom(vec3 lookfrom) {
	_origin = lookfrom;
	_w = unit_vector(_origin - _lookat);
	_u = unit_vector(cross(_vup, _w));
	_v = cross(_w, _u);
	_lower_left_corner = _origin - _half_width * _focus_dist*_u - _half_height * _focus_dist*_v - _focus_dist * _w;
	_horizontal = 2 * _half_width*_focus_dist*_u;
	_vertical = 2 * _half_height*_focus_dist*_v;
}

void camera::set_camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist){
	_lookat = lookat;
	_vup = vup;
	_focus_dist = focus_dist;
	_lens_radius = aperture / 2;
	_theta = vfov * M_PI / 180;
	_half_height = tan(_theta / 2);
	_half_width = aspect * _half_height;

	update_lookfrom(lookfrom);
}

camera::~camera(){
	if (nullptr != _e) {
		delete _e;
	}
}

ray camera::get_ray(float s, float t) {
	vec3 rd = _lens_radius * random_in_unit_disk();
	vec3 offset = _u * rd.x + _v * rd.y;
	return ray(_origin + offset, _lower_left_corner + s * _horizontal + t * _vertical - _origin - offset);
}

vec3 camera::random_in_unit_disk() {
	vec3 p;
	do {
		p = 2.0*vec3(_ur(*_e), _ur(*_e), 0) - vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}
