#pragma once
#include <algorithm>
#include <iostream>

class Vector3 {
public:
    float x, y, z;

    Vector3() { Vector3(0, 0, 0); }
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
    Vector3& operator += (const Vector3& rhs) { return *this = *this + rhs; }
    Vector3& operator -= (const Vector3& rhs) { return *this = *this - rhs; }
    Vector3& operator *= (float rhs) { return *this = *this * rhs; }
    Vector3& operator /= (float rhs) {
        if (rhs != 0) { return *this = *this / rhs; }
        throw std::invalid_argument("Division by zero.");
    }

    float Length() const { return sqrtf(x * x + y * y + z * z); }
    Vector3 Normalize() const {return *this * (1 / Length());}
    float Distance(const Vector3& rhs) const { return (*this - rhs).Length(); }

};


struct Vec4 {
    float x, y, z, w;
};

struct Vector2 {
    float x, y;

    Vector2() : Vector2(0.0f, 0.0f) {};
    Vector2(float x, float y) { this->x = x; this->y = y; };

    Vector2 operator + (const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); };
    Vector2& operator += (const Vector2& rhs) { return *this = *this + rhs; };
    Vector2 operator + (const float& rhs) const { return *this + Vector2(rhs, rhs); };
    Vector2& operator += (const float& rhs) { return *this = *this + Vector2(rhs, rhs); };
    Vector2 operator - (const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); };
    Vector2& operator -= (const Vector2& rhs) { return *this = *this - rhs; };
    Vector2 operator - (const float& rhs) const { return *this - Vector2(rhs, rhs); };
    Vector2& operator -= (const float& rhs) { return *this = *this - Vector2(rhs, rhs); };
    Vector2 operator * (const float& rhs) const { return Vector2(x * rhs, y * rhs); };
    Vector2& operator *= (const float& rhs) { return *this = *this * rhs; };
    Vector2 operator / (const float& rhs) const { return Vector2(x / rhs, y / rhs); };
    Vector2& operator /= (const float& rhs) { return *this = *this / rhs; };

    float Distance(Vector2& rhs) { return sqrtf((this->x - rhs.x) * (this->x - rhs.x) + (this->y - rhs.y) * (this->y - rhs.y)); }
};

using Point = Vector2;

struct Rect {
    float x, y;
    float width, height;
    const Point topLeft() { return Point{ x,y }; }
    const Point bottomRight() { return Point{ x + width,y + height }; }

    Rect() : Rect(0.0f, 0.0f, 0.0f, 0.0f) {};
    Rect(Point topLeft, float width, float height) : Rect(topLeft.x, topLeft.y, width, height) {};
    Rect(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {};

    Rect operator * (const float& rhs) const { return Rect(x, y, width * rhs, height * rhs); };
    Rect& operator *= (const float& rhs) { return *this = *this * rhs; };
    Rect operator * (const Point& rhs) const { return Rect(x, y, width * rhs.x, height * rhs.y); };
    Rect& operator *= (const Point& rhs) { return *this = *this * rhs; };
    Rect operator / (const float& rhs) const { return Rect(x, y, width / rhs, height / rhs); };
    Rect& operator /= (const float& rhs) { return *this = *this / rhs; };
    Rect operator / (const Point& rhs) const { return Rect(x, y, width / rhs.x, height / rhs.y); };
    Rect& operator /= (const Point& rhs) { return *this = *this * rhs; };
};

struct Line {
    Point a, b;

    Line() : Line(Point(), Point()) {};
    Line(Point a, Point b) : a(a), b(b) {};
};


// type alias
using Vec3 = Vector3;
using vec3 = Vector3;
using Vec = Vector3;
using vec = Vector3;
