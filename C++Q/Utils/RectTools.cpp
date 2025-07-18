#include "RectTools.h"
#include "Constant.h"
using namespace std;

RectTools::RectTools()
{
}


RectTools::~RectTools()
{
}

RangeType RectTools::getRange(const Rect & a, const Rect & b)
{
	float distance = a.origin.getDistance(b.origin);
	float threatened = a.size.width + b.size.height;

	if (distance <= threatened / 2)
		return RangeType::rangeEndangered;
	else if (distance <= threatened)
		return RangeType::rangeThreatened;
	else if (distance <= threatened * 2)
		return RangeType::rangeSensed;
	else if (distance <= ScreenWidth)
		return RangeType::rangeOutOfSight;

	return RangeType::rangeOut;
}

Rect RectTools::RectInset(Rect& rect, const float& dx, const float& dy)
{
	rect.origin.x += dx;
	rect.size.width -= dx * 2;
	rect.origin.y -= dy;    //缩小时y轴应该向下，IOS的坐标系与cocos2d-x不一样，y原点是在左下角而非左上角
	rect.size.height -= dy * 2;
	return rect;
}

Rect RectTools::RectOffset(Rect &rect, const float& dx, const float& dy)
{
	rect.origin.x += dx;
	rect.origin.y += dy;
	return rect;
}
const CollisionDirection RectTools::getCollsionDirection(const Rect &rectA, const Rect &rectB)
{
	return getCollsionDirection(rectA.origin.x, rectA.origin.y, rectA.size.width, rectA.size.height,
		rectB.origin.x, rectB.origin.y, rectB.size.width, rectB.size.height);
}
const CollisionDirection RectTools::getCollsionDirection(const int& x1, const int& y1, const int& w1, const int& h1, const int& x2, const int& y2, const int& w2, const int& h2)
{
	//当矩形1 位于矩形2 的左侧  
	if (x1 <= x2 && x1 + w1 <= x2)
	{
		return CollisionDirection::atLeft;

	}	//当矩形1 位于矩形2 的右侧
	else if (x1 >= x2 && x1 >= x2 + w2)
	{
		return CollisionDirection::atRight;
	}//当矩形1 位于矩形2 的上方  
	else if (y1 >= y2 && y1 >= y2 + h2)
	{
		return CollisionDirection::atTop;
	}
	//当矩形1 位于矩形2 的下方  
	else if (y1 <= y2 && y1 + h1 <= y2) {
		return CollisionDirection::atBottom;
	}
	//所有不会发生碰撞都不满足时，肯定就是碰撞了  
	return CollisionDirection::intersected;
}
SlidedResult RectTools::isSlided(const Vec2& beginDelta, const Vec2& endDelta, const Vec2 & range, const Rect & bound)
{
	auto diff = endDelta - beginDelta;
	bool isIntersected = false;
	if (abs(diff.x) > range.x && abs(diff.y) > range.y)
		isIntersected = isLineIntersectRect(beginDelta, endDelta, bound);

	if (!isIntersected)
		return SlidedResult::missed;
	else
		return beginDelta.x <= endDelta.x ? SlidedResult::left : SlidedResult::right;
}
Rect RectTools::getIntersectRect(const Rect &rectA, const Rect &rectB)
{
	Vec2 bottomLeft;    //左下角顶点     cocos2dx的CCRect是以左下角为原点，w,h为参数(宽和高)，并不是左上角是top,右下角为bottom
	Vec2 topRight;	//右上角顶点
	float width = 0.f, height = 0.f;

	if (rectA.intersectsRect(rectB))
	{
		bottomLeft.x = max(rectA.getMinX(), rectB.getMinX()); //左下角x坐标取两个矩形ori顶点x坐标大的
		bottomLeft.y = max(rectA.getMinY(), rectB.getMinY()); //左下角y坐标取两个矩形ori顶点y坐标大的

		topRight.x = min(rectA.getMaxX(), rectB.getMaxX()); //右上角x坐标取两个矩形右上角顶点x坐标小的
		topRight.y = min(rectA.getMaxY(), rectB.getMaxY());

		width = topRight.x - bottomLeft.x;
		height = topRight.y - bottomLeft.y;

		return Rect(bottomLeft.x, bottomLeft.y, width, height);
	}
	return Rect(0, 0, -1.f, -1.f);
}

/*
圆形类hitTest(rect1, circle.x + circle.width * 0.5, circle.y + circle.height * 0.5, circle.width * 0.5).toString();
hitTest(rect:Rectangle, cx:Number, cy:Number, r:Number):Boolean
*/

bool RectTools::intersectCircle(const Rect& rect, const Rect& circle)
{
	Size midRect = rect.size / 2;
	float cx = circle.getMidX();
	float cy = circle.getMidY();
	float radius = circle.size.width / 2;
	//圆心与矩形中心的相对坐标
	float rx = cx - rect.getMidX(); //var rx : Number = cx - (rect.left + rect.width * 0.5);
	float ry = cy - rect.getMidY();//var ry : Number = cy - (rect.top + rect.height * 0.5);

	float dx = min(rx, midRect.width);	//var dx : Number = Math.min(rx, rect.width * 0.5);
	float dx1 = max(dx, -midRect.width);//var dx1 : Number = Math.max(dx, -rect.width * 0.5);

	float dy = min(ry, midRect.height);//var dy : Number = Math.min(ry, rect.height * 0.5);
	float dy1 = max(dy, -midRect.height);//var dy1 : Number = Math.max(dy, -rect.height * 0.5);
	return (dx1 - rx) * (dx1 - rx) + (dy1 - ry) * (dy1 - ry) <= radius * radius;
	//return Rect(0, 0, -1.f, -1.f);
}


/**
*   判断线段是否在矩形内
*
* 先看线段所在直线是否与矩形相交，
* 如果不相交则返回false，
* 如果相交，
* 则看线段的两个点是否在矩形的同一边（即两点的x(y)坐标都比矩形的小x(y)坐标小，或者大）,
* 若在同一边则返回false，
* 否则就是相交的情况。
* @param linePointX1 线段起始点x坐标
* @param linePointY1 线段起始点y坐标
* @param linePointX2 线段结束点x坐标
* @param linePointY2 线段结束点y坐标
* @param rectangleLeftTopX 矩形左上点x坐标
* @param rectangleLeftTopY 矩形左上点y坐标
* @param rectangleRightBottomX 矩形右下点x坐标
* @param rectangleRightBottomY 矩形右下点y坐标
* @return 是否相交
*/

bool RectTools::isLineIntersectRect(const Vec2& start, const Vec2& end, const Rect& rect)
{ 
	return getLineIntersectRect(start, end, rect).isIntersected;
}

LineInfo RectTools::getLineIntersectRect(const Vec2 & start, const Vec2 & end, const Rect & rect)
{
	LineInfo lineInfo;
	int lineWidth = end.x - start.x;
	int lineHeight = start.y- end.y ;
	// 计算叉乘
	int c = start.x *  end.y - end.x * start.y;
	auto rectLeftTopX = rect.origin.x;
	auto rectLeftTopY = rect.getMaxY();
	auto rectRightBottomX = rect.getMaxX();
	auto rectRightBottomY = rect.origin.y;

	lineInfo.start = start;
	lineInfo.end = end;
	if ((lineHeight * rectLeftTopX + lineWidth * rectLeftTopY + c >= 0 && lineHeight * rectRightBottomX + lineWidth * rectRightBottomY + c <= 0)
		|| (lineHeight * rectLeftTopX + lineWidth * rectLeftTopY + c <= 0 && lineHeight * rectRightBottomX + lineWidth * rectRightBottomY + c >= 0)
		|| (lineHeight * rectLeftTopX + lineWidth * rectRightBottomY + c >= 0 && lineHeight * rectRightBottomX + lineWidth * rectLeftTopY + c <= 0)
		|| (lineHeight * rectLeftTopX + lineWidth * rectRightBottomY + c <= 0 && lineHeight * rectRightBottomX + lineWidth * rectLeftTopY + c >= 0))
	{
		if (rectLeftTopX > rectRightBottomX)
		{
			int temp = rectLeftTopX;
			rectLeftTopX = rectRightBottomX;
			rectRightBottomX = temp;
		}
		if (rectLeftTopY < rectRightBottomY)
		{
			int temp = rectLeftTopY;
			rectLeftTopY = rectRightBottomY;
			rectRightBottomY = temp;
		}
		if ((start.x < rectLeftTopX && end.x < rectLeftTopX)
			|| (start.x > rectRightBottomX && end.x > rectRightBottomX)
			|| (start.y > rectLeftTopY && end.y > rectLeftTopY)
			|| (start.y < rectRightBottomY && end.y < rectRightBottomY))
		{
			//CCLOG("[RectTools::getLineIntersectRect]%lf", lineInfo.bottomDiff);
			return lineInfo;
		}
		else
		{
			//CCLOG("[RectTools::getLineIntersectRect]%lf", lineInfo.bottomDiff);
			lineInfo.isIntersected = true;
			return lineInfo;
		}
	}
	else
	{
		//CCLOG("[RectTools::getLineIntersectRect]%lf", lineInfo.bottomDiff);
		return lineInfo;
	}
}

 
