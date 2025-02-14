#pragma once

#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <cfloat>
#include <iostream>


#define CMP(x, y) \
 (fabsf((x)-(y)) <= FLT_EPSILON * \
 fmaxf(1.0f, \
 fmaxf(fabsf(x), fabsf(y))) \
 )

typedef struct vec3 {
	union {
		struct {
			float x;
			float y;
			float z;
		};
		float asArray[3];
	};
	float& operator[](int i) {
		return asArray[i];
	}
	inline vec3(float xx = 0.0f, float yy = 0.0f, float zz = 0.0f) {
		x = xx;
		y = yy;
		z = zz;
	}

	float length() const {
		return std::sqrt(length_squared());
	}

	float length_squared() const {
		return x * x + y * y + z * z;
	}

	vec3 operator-() const { return vec3(-x, -y, -z); }

	vec3& operator +=(const vec3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}

	vec3& operator *=(const vec3& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
	}

	vec3& operator /=(const vec3& v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
	}

} vec3;


using point3 = vec3;

// vector utility functions

inline std::ostream& operator <<(std::ostream& out, const vec3& v) {
	return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

vec3 operator+(const vec3& l, const vec3& r) {
	return vec3(l.x + r.x, l.y + r.y, l.z + r.z);
}

vec3 operator+=(vec3& l, const vec3& r) {
	return l = vec3(l.x + r.x, l.y + r.y, l.z + r.z);
}

vec3 operator-(const vec3& l, const vec3& r) {
	return vec3(l.x - r.x, l.y - r.y, l.z - r.z);
}

vec3 operator-=(vec3& l, const vec3& r) {
	return l = vec3(l.x - r.x, l.y - r.y, l.z - r.z);
}

vec3 operator*(const vec3& l, const vec3& r) {
	return vec3(l.x * r.x, l.y * r.y, l.z * r.z);
}
vec3 operator*=(vec3& l, const vec3& r) {
	return l = vec3(l.x * r.x, l.y * r.y, l.z * r.z);
}

vec3 operator*(const vec3& l, float r) {
	return vec3(l.x * r, l.y * r, l.z * r);
}
vec3 operator*=(vec3& l, float r) {
	return l = vec3(l.x * r, l.y * r, l.z * r);
}

vec3 operator/(const vec3& l, const vec3& r) {
	return vec3(l.x / r.x, l.y / r.y, l.z / r.z);
}
vec3 operator/=(vec3& l, const vec3& r) {
	return l = vec3(l.x / r.x, l.y / r.y, l.z / r.z);
}

bool operator==(const vec3& l, const vec3& r) {
	return CMP(l.x, r.x) && CMP(l.y, r.y) && CMP(l.z, r.z);
}

bool operator!=(const vec3& l, const vec3& r) {
	return !(l == r);
}

bool operator<(const vec3& l, const vec3& r) {
	if (l.x < r.x && r.y < r.y && r.z < r.z)
		return true;
	else
		return false;
}
bool operator<=(const vec3& l, const vec3& r) {
	if (l.x <= r.x && r.y <= r.y && r.z <= r.z)
		return true;
	else
		return false;
}

bool operator>(const vec3& l, const vec3& r) {
	if (l.x > r.x && r.y > r.y && r.z > r.z)
		return true;
	else
		return false;
}
bool operator>=(const vec3& l, const vec3& r) {
	if (l.x >= r.x && r.y >= r.y && r.z >= r.z)
		return true;
	else
		return false;
}

#endif