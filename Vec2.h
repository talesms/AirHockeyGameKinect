// Vec2.h
#ifndef VEC2_H
#define VEC2_H

#include <iostream>

using namespace std;

class Vec2
{
public:
	Vec2();
	Vec2(float coords[2]);
	Vec2(float x, float y);

	Vec2 operator+(const Vec2& rhs);
	Vec2 operator-(const Vec2& rhs);
	Vec2 operator*(float scalar);
	Vec2 operator/(float scalar);
	Vec2 operator+=(const Vec2& rhs);
	Vec2 operator-=(const Vec2& rhs);
	Vec2 operator*=(float scalar);
	Vec2 operator/=(float scalar);
	float operator*(const Vec2& rhs);
	bool operator==(const Vec2& rhs);
	bool operator!=(const Vec2& rhs);
	float length();
	Vec2 normalize();
	float* toFloatArray();


	float mX;
	float mY;
	float mZ;
};

#endif // VEC2_H