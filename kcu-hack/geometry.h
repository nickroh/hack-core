#pragma once
#include <algorithm>
#include <iostream>

class Vector3 {
public:
	float x, y, z;

	Vector3() {};
	Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

    // Addition
    Vector3 operator + (const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }

    // Subtraction
    Vector3 operator - (const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }

    // Scalar multiplication
    Vector3 operator * (float rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }

    // Scalar division
    Vector3 operator / (float rhs) const {
        if (rhs != 0) return Vector3(x / rhs, y / rhs, z / rhs);
        throw std::invalid_argument("Division by zero.");
    }

    // Compound assignment operators
    Vector3& operator += (const Vector3& rhs) {return *this = *this + rhs;}
    Vector3& operator -= (const Vector3& rhs) { return *this = *this - rhs; }
    Vector3& operator *= (float rhs) { return *this = *this * rhs; }
    Vector3& operator /= (float rhs) {
        if (rhs != 0) { return *this = *this / rhs; }
        throw std::invalid_argument("Division by zero.");
    }

};

struct Vec4 {
    float x, y, z, w;
};

// type alias
using Vec3 = Vector3;
using vec3 = Vector3;
using Vec = Vector3;
using vec = Vector3;