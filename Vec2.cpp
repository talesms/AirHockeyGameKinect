#include "Vec2.h"

Vec2::Vec2()
	: mX(0.0f), mY(0.0f)
{
}

Vec2::Vec2(float coords[2])
	: mX(coords[0]), mY(coords[1])
{
}

Vec2::Vec2(float x, float y)
	: mX(x), mY(y)
{
}

Vec2 Vec2::operator+(const Vec2& rhs)
{
	Vec2 sum;
	sum.mX = mX + rhs.mX;
	sum.mY = mY + rhs.mY;

	return sum;
}

Vec2 Vec2::operator-(const Vec2& rhs)
{
	Vec2 dif;
	dif.mX = mX - rhs.mX;
	dif.mY = mY - rhs.mY;

	return dif;
}

Vec2 Vec2::operator*(float scalar)
{
	Vec2 p;
	p.mX = mX * scalar;
	p.mY = mY * scalar;

	return p;
}

Vec2 Vec2::operator/(float scalar)
{
	Vec2 p;
	p.mX = mX / scalar;
	p.mY = mY / scalar;

	return p;
}

Vec2 Vec2::operator+=(const Vec2& rhs)
{
	Vec2 sum;
	mX = sum.mX = mX + rhs.mX;
	mY = sum.mY = mY + rhs.mY;

	return sum;
}

Vec2 Vec2::operator-=(const Vec2& rhs)
{
	Vec2 dif;
	dif.mX = mX - rhs.mX;
	dif.mY = mY - rhs.mY;

	return dif;
}

Vec2 Vec2::operator*=(float scalar)
{
	Vec2 p;
	mX = p.mX = mX * scalar;
	mY = p.mY = mY * scalar;

	return p;
}

Vec2 Vec2::operator/=(float scalar)
{
	Vec2 p;
	mX = p.mX = mX / scalar;
	mY = p.mY = mY / scalar;

	return p;
}

float Vec2::operator*(const Vec2& rhs)
{
	float dotP = mX*rhs.mX + mY*rhs.mY;

	return dotP;
}

bool Vec2::operator==(const Vec2& rhs)
{
	// Return true if the corresponding components are equal.
	return	mX == rhs.mX &&	mY == rhs.mY;
}

bool Vec2::operator!=(const Vec2& rhs)
{
	// Return true if any one corresponding components
	// are _not_ equal.
	return mX != rhs.mX || mY != rhs.mY;
}

float Vec2::length()
{
	return sqrtf(mX*mX + mY*mY);
}

Vec2 Vec2::normalize()
{
	Vec2 n;

	// Get 'this' vector's length.
	float len = length();

	// Divide each component by the length.
	n.mX = mX/len;
	n.mY = mY/len;

	return n;
}

float* Vec2::toFloatArray()
{
	return &mX;
}