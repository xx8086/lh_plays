#include "vec3.h"
#include <math.h>
#include <stdlib.h>

vec3::vec3(float e0, float e1, float e2) { 
	e[0] = e0; e[1] = e1; e[2] = e2; 
}

 const vec3& vec3::operator+() const { 
	return *this; 
}
 vec3 vec3::operator-() const { 
	return vec3(-e[0], -e[1], -e[2]); 
}
 float vec3::operator[](int i) const { 
	return e[i]; 
}
 float& vec3::operator[](int i) { 
	return e[i]; 
};

float vec3::length() const {
	return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); 
}
float vec3::squared_length() const { 
	return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; 
}


 vec3& vec3::operator+=(const vec3 &v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

 vec3& vec3::operator*=(const vec3 &v) {
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

 vec3& vec3::operator/=(const vec3 &v) {
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

 vec3& vec3::operator-=(const vec3& v) {
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

 vec3& vec3::operator*=(const float t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

 vec3& vec3::operator/=(const float t) {
	float k = 1.0 / t;

	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}

 void vec3::make_unit_vector() {
	float k = 1.0f / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	e[0] *= k; e[1] *= k; e[2] *= k;
}
