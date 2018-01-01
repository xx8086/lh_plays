#include "RectPosition.h"


RectPosition::RectPosition()
{
}
RectPosition::RectPosition(long a, long b,long c, long d):iLeftX(a),iLeftY(b),iRightX(c),iRightY(d) 
{
}
RectPosition::~RectPosition()
{
}

void RectPosition::setRect(TERECT & rect) const
{
	rect.left = iLeftX;
	rect.top = iLeftY;
	rect.right = iRightX;
	rect.bottom = iRightY;
}
