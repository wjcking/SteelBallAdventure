#include "MapProperty.h"

TilePropertyInfo::TilePropertyInfo()//:isSlope(false), isHarmful(false), isOneWay(false), inset(Rect::ZERO)
{
	isSlope = false;
	isHarmful = false;
	allowThrough = false;
	isOneWay = false;
	slopeGravity = 0.11f;
	roleOffsetY=0.f;
	initSlopeY = false;
	inset = Rect::ZERO;
}

/*
*°üÎ§ºÐÀà
*/
BoundRect::BoundRect(int id, Vec2& o, Size& s) : gid(id), Rect(o, s)
{

}

BoundRect::BoundRect(int id, Vec2 tilePos, Rect bound) : gid(id), tilePosition(tilePos), Rect(bound)
{

}

BoundRect::BoundRect() : Rect()
{

}
