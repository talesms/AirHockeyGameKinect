#include "Circle.h"

Circle::Circle()
: r(0.0f), c(0.0f, 0.0f)
{
}

Circle::Circle(float R, const Vec2& center)
: r(R), c(center)
{
}

bool Circle::hits(Circle& A, Vec2& normal)
{
	Vec2 u = A.c - c;
	if( u.length() <= r + A.r )
	{
		normal = u.normalize();
		// Make sure circles never overlap--at most
		// they can be tangent.
		A.c = c + (normal * (r + A.r));

		return true;
	}
	return false;
}