#pragma once
#include "hitable.h"
#include <random>

class material{
public:
	material();
	~material();
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
protected:
	float schlick(float cosine, float ref_idx) const;
	bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) const;
	vec3 reflect(const vec3& v, const vec3& n) const;
	vec3 random_in_unit_sphere() const;

protected:
	std::mt19937* _e;
	std::uniform_real_distribution<double> _ur;
};

class lambertian : public material {
public:
	lambertian(const vec3& a);
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;

public:
	vec3 albedo;
};

class metal : public material {
public:
	metal(const vec3& a, float f);
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;

public:
	vec3 albedo;
	float fuzz;
};


class dielectric : public material {
public:
	dielectric(float ri);
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;

public:
	float ref_idx;
};
