#include "sphere.h"
#include <math.h>

sphere::~sphere(){
	if (nullptr != mat_ptr) {
		delete mat_ptr; 
		mat_ptr = nullptr; 
	}
}

sphere::sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(m) {
}

bool sphere::sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec3 oc = r.origin - center;
	float a = dot(r.direction, r.direction);
	float b = dot(oc, r.direction);
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0) {
#if 1
		float q = sqrt(discriminant);
		float t1 = (-b + q) / a;
		float t2 = (-b - q) / a;
		rec.t = t1 < t2 ? t1 : t2;
		if (rec.t < t_max && rec.t > t_min) {
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}

#else
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
#endif
	}
	return false;
}