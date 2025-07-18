#ifndef TRIGGER_REGION_H
#define TRIGGER_REGION_H
#include "cocos2d.h"
using namespace cocos2d;

class TriggerRegion
{
public:

	virtual ~TriggerRegion() {}
	//returns true if an entity of the given size and position is intersecting
	//the trigger region.
	virtual bool isTouching(const Rect& = Rect::ZERO, double radius = 0)const = 0;
};


class TriggerLine : public TriggerRegion
{
private:
	Vec2 start;
	Vec2 end;
public:

	TriggerLine(const Vec2& start, const Vec2& end) :start(start), end(end)
	{

	}

	~TriggerLine() {}

	bool isTouching(const Rect& pos) const
	{
		return true;
	}
};
class TriggerCircle : public TriggerRegion
{
private:

	//the center of the region
	Rect rect;
	//the radius of the region
	double radius;

public:

	TriggerCircle(const Rect& rect, const float& radius) :radius(radius), rect(rect)
	{}

	bool isTouching(const Rect& pos)const
	{
		//   return Vec2DDistanceSq(pos, pos) < (EntityRadius + m_dRadius)*(EntityRadius + m_dRadius);
		return true;
	}
};
class TriggerRectangle : public TriggerRegion
{
private:
	Rect rect;
public:

	TriggerRectangle(const Rect& rect) :rect(rect)
	{
		//m_pTrigger = new InvertedAABBox2D(TopLeft, BottomRight);
	}

	~TriggerRectangle() {}

	bool isTouching(const Rect& rect)const
	{
		return true;
	}
};


#endif