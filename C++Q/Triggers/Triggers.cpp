#include "Trigger.h"
#include "../Utils/Constant.h"
#include "../Role/Role.h"
#include "../Utils/Utils.h"
Trigger::Trigger(const bool& bAct) :toBeRemoved(false),
bActive(bAct)
//graphNodeIndex(-1)
{
	relatedTags.reserve(Reserve_Role);
}
Trigger::Trigger(const Size& size, const MovingDirection& md) :sensorRange(size),
sensorDirection(md),
toBeRemoved(false),
bActive(true)
{
	relatedTags.reserve(Reserve_Role);
}



Trigger::~Trigger() {  }

/******* Ïß¶Î********/
TriggerLine::TriggerLine(const Vec2& start, const Vec2& end) :start(start), end(end)
{

}
TriggerLine::TriggerLine(const Size& range, const MovingDirection& md) : Trigger(range, md) {};
TriggerLine::TriggerLine() {};
TriggerLine::~TriggerLine() {}

bool& TriggerLine::scanTouching(Role& entityRect)
{
	isTouched = false;
	auto bound = entityRect.getCollisionBound();
	distance = RectTools::getRange(Rect(start.x, end.y, end.x - start.x, 0.05f), bound);

	if (distance < RangeType::rangeThreatened)
		return isTouched;
	isTouched = RectTools::isLineIntersectRect(start, end, entityRect.getCollisionBound());

	return isTouched;
}
void TriggerLine::render(DrawNode* draw)
{
	draw->drawLine(start, end, getColor());
};

void TriggerLine::updateRegion(Role& role, const float& radius)
{
	switch (sensorDirection)
	{
	case toLeft:
		start = Vec2(role.getCollisionBound().origin.x - sensorRange.width, role.getCollisionBound().getMidY());
		end = Vec2(role.getCollisionBound().origin.x, role.getCollisionBound().getMidY());
		break;
	case toRight:
		start = Vec2(role.getCollisionBound().getMaxX(), role.getCollisionBound().getMidY());
		end = Vec2(role.getCollisionBound().getMaxX() + sensorRange.width, role.getCollisionBound().getMidY());
		break;
	case toTop:
		start = Vec2(role.getCollisionBound().getMinX(), role.getCollisionBound().getMaxY());
		end = Vec2(role.getCollisionBound().getMinX(), role.getCollisionBound().getMaxY() + sensorRange.height);
		break;
	case toBottom:
		start = Vec2(role.getCollisionBound().getMinX(), role.getCollisionBound().getMinY());
		end = Vec2(role.getCollisionBound().getMinX(), role.getCollisionBound().getMinY() - sensorRange.height);
		break;

	}
}
//Ô²ÐÎ
TriggerCircle::TriggerCircle(const Rect& rect, const float& radius) :radius(radius), circle(rect)
{
}

TriggerCircle::TriggerCircle(const Size& range, const MovingDirection& md) : Trigger(range, md) {};
TriggerCircle::TriggerCircle() {};
TriggerCircle::~TriggerCircle() {};
bool& TriggerCircle::scanTouching(Role& entityRect)
{
	//distance = RectTools::getRange(entityRect, circle);
	//isTouched = entityRect.intersectsCircle(Vec2(circle.getMidX(), circle.getMidY()), radius == 0 ? circle.size.width / 2 : radius);

	return isTouched;
}

void TriggerCircle::render(DrawNode* draw)
{
	draw->drawCircle(Vec2(circle.getMidX(), circle.getMidY()), circle.size.width / 2, 360, 12, true, getColor());
}
void TriggerCircle::updateRegion(Role& role, const float& radius)
{
	switch (sensorDirection)
	{
	case toTop:
		circle = Rect(Vec2(role.getCollisionBound().getMinX(), role.getCollisionBound().getMaxY()), sensorRange);
		break;
	case toBottom:
		circle = Rect(Vec2(role.getCollisionBound().getMinX(), role.getCollisionBound().getMinY() - sensorRange.height), sensorRange);
		break;
	case toLeft:
		circle = Rect(Vec2(role.getCollisionBound().origin.x - sensorRange.width, role.getCollisionBound().getMinY()), sensorRange);
		break;
	case toRight:
		circle = Rect(Vec2(role.getCollisionBound().getMaxX(), role.getCollisionBound().getMinY()), sensorRange);
		break;
	}
};
//¾ØÐÎ

TriggerRect::TriggerRect(const Rect& rect, const bool& isAct) :rect(rect)
{

}

TriggerRect::TriggerRect(const Size& range, const MovingDirection& md) : Trigger(range, md) {};
TriggerRect::TriggerRect() {};
TriggerRect::~TriggerRect() {}

bool& TriggerRect::scanTouching(Role& entityRect)
{
	auto bound = entityRect.getCollisionBound();
	distance = RectTools::getRange(bound, rect);
	isTouched = false;
	//if (distance >= RangeType::rangeSensed)
	if (distance < RangeType::rangeThreatened)
		return isTouched;

	isTouched = bound.intersectsRect(rect);

	return isTouched;
}

void TriggerRect::render(DrawNode* draw)
{
	draw->drawRect(rect.origin, Vec2(rect.getMaxX(), rect.getMaxY()), getColor());
}

void TriggerRect::updateRegion(Role& role, const float& radius)
{
	auto boundBox = role.getCollisionBound();

	switch (sensorDirection)
	{
	case toTop:
		rect = Rect(Vec2(boundBox.getMinX(), boundBox.getMaxY()), sensorRange);
		break;
	case toBottom:
		rect = Rect(Vec2(boundBox.getMinX(), boundBox.getMinY() - sensorRange.height), sensorRange);
		break;
	case toLeft:
		rect = Rect(Vec2(boundBox.origin.x - sensorRange.width, boundBox.getMinY()), sensorRange);
		break;
	case toRight:
		rect = Rect(Vec2(boundBox.getMaxX(), boundBox.getMinY()), sensorRange);
		break;
	}
}