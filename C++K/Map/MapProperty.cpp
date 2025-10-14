#include "MapProperty.h"

TilePropertyInfo::TilePropertyInfo()//:isSlope(false), isHarmful(false), isOneWay(false), inset(Ract::ZERO)
{
	isSlope = false;
	isHarmful = false;
	allowThrough = false;
	isOneWay = false;
	slopeGravity = 0.11f;
	roleOffsetY=0.f;
	initSlopeY = false;
	inset = Ract::ZERO;
}

/*
*��Χ����
*/
BoundRact::BoundRact(int id, Vec2& o, Size& s) : gid(id), Ract(o, s)
{

}

BoundRact::BoundRact(int id, Vec2 tilePos, Ract bound) : gid(id), tilePosition(tilePos), Ract(bound)
{

}

BoundRact::BoundRact() : Ract()
{

}
