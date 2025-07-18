#pragma once
#include "cocos2d.h"
using namespace cocos2d;
#define ScreenSize Director::getInstance()->getVisibleSize()

//角色和瓦片公用
enum MovingDirection : unsigned short
{
	stayStill = 0,
	toTop = 1,
	toBottom = 2,
	toLeft = 3,
	toRight = 4,
	toTopLeft = 5,
	toTopRight = 6,
	toBottomLeft = 7,
	toBottomRight = 8
};
//碰撞前的位置 用2的n次方来赋值
enum CollisionDirection :short
{
	intersected = 0,
	atLeft,
	atRight,
	atTop,
	atBottom,
};
//包围盒数组下标
/***
5(4)  2(1) 6(5)
3(2)          4(3)
7(6)  1(0) 8(7)
**/
enum BoundPosition : unsigned short
{
	top = 1,
	bottom = 0,
	left = 2,
	right = 3,
	topLeft = 4,
	topRight = 5,
	bottomLeft = 6,
	bottomRight = 7,
	centerSelf = 8
};
static const unsigned short TileBoundNumber = static_cast<unsigned short>(BoundPosition::centerSelf) + 1;
enum RangeType : short
{
	rangeOut = 1,
	rangeOutOfSight = 2,
	rangeSensed = 3,
	rangeThreatened = 4,
	rangeEndangered = 5,
};

enum class SlidedResult
{
	missed,
	left,
	right,
};
inline bool inRange(const Vec2& touched, const Rect& pad)
{
	if (touched.x >= pad.getMinX() && touched.x <= pad.getMaxX() &&
		touched.y >= pad.getMinY() && touched.y <= pad.getMaxY())
		return true;

	return false;
};

struct LineInfo
{
	bool isIntersected = false;
	Vec2 start = Vec2::ZERO;
	Vec2 end = Vec2::ZERO;
};
class RectTools
{
public:
	RectTools();
	~RectTools();
	static RangeType getRange(const Rect& a, const Rect& b);

	//如果是rect& 可能加const
	static Rect RectInset(Rect& rect, const float& dx, const float& dy);
	static Rect RectOffset(Rect &rect, const float& dx, const float& dy);

	static bool  intersectCircle(const Rect&, const Rect&);
	static Rect getIntersectRect(const Rect &rectA, const Rect &rectB); 
	static bool isLineIntersectRect(const Vec2& start, const Vec2& end, const Rect& rect);

	static LineInfo getLineIntersectRect(const Vec2& start, const Vec2& end, const Rect& rect);
 
	static const CollisionDirection getCollsionDirection(const Rect &rectA, const Rect &rectB);
	static const CollisionDirection getCollsionDirection(const int& x1, const int& y1, const int& w1, const int& h1, const int& x2, const int& y2, const int& w2, const int& h2);
	static SlidedResult isSlided(const Vec2& beginDelta, const Vec2& endDelta, const Vec2 & range, const Rect & bound);
};