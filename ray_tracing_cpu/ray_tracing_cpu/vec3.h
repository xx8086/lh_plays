#pragma once
#define M_PI (3.1415926f)

class vec3 {
public:
	vec3() {}
	vec3(float e0, float e1, float e2);

	const vec3& operator+() const;
	vec3 operator-() const;
	float operator[](int i) const;
	float& operator[](int i);

	vec3& operator+=(const vec3 &v2);
	vec3& operator-=(const vec3 &v2);
	vec3& operator*=(const vec3 &v2);
	vec3& operator/=(const vec3 &v2);
	vec3& operator*=(const float t);
	vec3& operator/=(const float t);
	float length() const;

	inline float squared_length() const;
	inline void make_unit_vector();

public:
	union {
		float e[3];
		struct {float x; float y; float z;};
		struct {float r; float g; float b;};
	};
};



inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3 operator*(float t, const vec3 &v) {
	return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator/(vec3 v, float t) {
	return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline vec3 operator*(const vec3 &v, float t) {
	return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline float dot(const vec3 &v1, const vec3 &v2) {
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
	return vec3((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
		(-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
		(v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}

inline vec3 unit_vector(vec3 v) {
	return v / v.length();
}