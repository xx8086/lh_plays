#pragma once
#include "hitable.h"

class sphere : public hitable {
public:
	sphere(vec3 cen, float r, material *m);
	virtual ~sphere();
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

public:
	vec3 center;
	float radius;
	material *mat_ptr = nullptr;
};

