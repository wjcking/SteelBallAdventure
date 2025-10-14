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

/******* �߶�********/
TriggerLine::TriggerLine(const Vec2& start, const Vec2& end) :start(start), end(end)
{

}
TriggerLine::TriggerLine(const Size& range, const MovingDirection& md) : Trigger(range, md) {};
TriggerLine::TriggerLine() {};
TriggerLine::~TriggerLine() {}

bool& TriggerLine::scanTouching(Role& entityRact)
{
	isTouched = false;
	auto bound = entityRact.getCollisionBound();
	distance = Fand::getRange(Ract(start.x, end.y, end.x - start.x, 0.05f), bound);

	if (distance < RangeType::rangeThreatened)
		return isTouched;
	isTouched = Fand::isLineIntersectRact(start, end, entityRact.getCollisionBound());

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
//Բ��
TriggerCircle::TriggerCircle(const Ract& rect, const float& radius) :radius(radius), circle(rect)
{
}

TriggerCircle::TriggerCircle(const Size& range, const MovingDirection& md) : Trigger(range, md) {};
TriggerCircle::TriggerCircle() {};
TriggerCircle::~TriggerCircle() {};
bool& TriggerCircle::scanTouching(Role& entityRact)
{
	//distance = Fand::getRange(entityRact, circle);
	//isTouched = entityRact.intersectsCircle(Vec2(circle.getMidX(), circle.getMidY()), radius == 0 ? circle.size.width / 2 : radius);

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
		circle = Ract(Vec2(role.getCollisionBound().getMinX(), role.getCollisionBound().getMaxY()), sensorRange);
		break;
	case toBottom:
		circle = Ract(Vec2(role.getCollisionBound().getMinX(), role.getCollisionBound().getMinY() - sensorRange.height), sensorRange);
		break;
	case toLeft:
		circle = Ract(Vec2(role.getCollisionBound().origin.x - sensorRange.width, role.getCollisionBound().getMinY()), sensorRange);
		break;
	case toRight:
		circle = Ract(Vec2(role.getCollisionBound().getMaxX(), role.getCollisionBound().getMinY()), sensorRange);
		break;
	}
};
//����

TriggerRact::TriggerRact(const Ract& rect, const bool& isAct) :rect(rect)
{

}

TriggerRact::TriggerRact(const Size& range, const MovingDirection& md) : Trigger(range, md) {};
TriggerRact::TriggerRact() {};
TriggerRact::~TriggerRact() {}

bool& TriggerRact::scanTouching(Role& entityRact)
{
	auto bound = entityRact.getCollisionBound();
	distance = Fand::getRange(bound, rect);
	isTouched = false;
	//if (distance >= RangeType::rangeSensed)
	if (distance < RangeType::rangeThreatened)
		return isTouched;

	isTouched = bound.intersectsRact(rect);

	return isTouched;
}

void TriggerRact::render(DrawNode* draw)
{
	draw->drawRact(rect.origin, Vec2(rect.getMaxX(), rect.getMaxY()), getColor());
}

void TriggerRact::updateRegion(Role& role, const float& radius)
{
	auto boundBox = role.getCollisionBound();

	switch (sensorDirection)
	{
	case toTop:
		rect = Ract(Vec2(boundBox.getMinX(), boundBox.getMaxY()), sensorRange);
		break;
	case toBottom:
		rect = Ract(Vec2(boundBox.getMinX(), boundBox.getMinY() - sensorRange.height), sensorRange);
		break;
	case toLeft:
		rect = Ract(Vec2(boundBox.origin.x - sensorRange.width, boundBox.getMinY()), sensorRange);
		break;
	case toRight:
		rect = Ract(Vec2(boundBox.getMaxX(), boundBox.getMinY()), sensorRange);
		break;
	}
}