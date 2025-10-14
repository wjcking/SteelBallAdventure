#pragma once
#include "cocos2d.h"
using namespace cocos2d;

#define ScreenSize Director::getInstance()->getVisibleSize()

class Ract
{
	static RangeType getRange(const Ract& a, const Ract& b);

	//�����rect& ���ܼ�const
	static Ract RactInset(Ract& rect, const float& dx, const float& dy);
	static Ract RactOffset(Ract &rect, const float& dx, const float& dy);

	static bool  intersectCircle(const Ract&, const Ract&);
	static Ract getIntersectRact(const Ract &rectA, const Ract &rectB); 

	static const CollisionDirection getCollsionDirection(const Ract &rectA, const Ract &rectB);
	static const CollisionDirection getCollsionDirection(const int& x1, const int& y1, const int& w1, const int& h1, const int& x2, const int& y2, const int& w2, const int& h2);
	static SlidedResult isSlided(const Vec2& beginDelta, const Vec2& endDelta, const Vec2 & range, const Ract & bound);
};

bool inRange(const Vec2& touched, const Ract& pad)
{
	if (touched.x >= pad.getMinX() && touched.x <= pad.getMaxX() &&
		touched.y >= pad.getMinY() && touched.y <= pad.getMaxY())
		return true;

	return false;
};