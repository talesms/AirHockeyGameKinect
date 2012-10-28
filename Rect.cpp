#include "Rect.h"

Rect::Rect()
{
}

Rect::Rect(const Vec2& a, const Vec2& b)
: minPt(a), maxPt(b)
{
}

Rect::Rect(float x0, float y0, float x1, float y1)
: minPt(x0, y0), maxPt(x1, y1)
{
}

void Rect::forceInside(Circle& A)
{
	Vec2 p = A.c;
	float r = A.r;
	// Modify coordinates to force inside.
	if(p.mX - r < minPt.mX)
		p.mX = minPt.mX + r;
	if(p.mX + r > maxPt.mX)
		p.mX = maxPt.mX - r;
	if(p.mY - r < minPt.mY)
		p.mY = minPt.mY + r;
	if(p.mY + r > maxPt.mY)
		p.mY = maxPt.mY - r;
	// Save forced position.
	A.c = p;
}

bool Rect::isPtInside(const Vec2& pt)
{
	return pt.mX >= minPt.mX && pt.mY >= minPt.mY &&
		pt.mX <= maxPt.mX && pt.mY <= maxPt.mY;
}