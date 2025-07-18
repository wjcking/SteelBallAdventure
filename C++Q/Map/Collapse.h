#ifndef  COLLIPASE_H
#define COLLIPASE_H

#include "cocos2d.h"
#include "../Utils/RectTools.h"
#include "../Utils/Constant.h"
#include "../../Common/Misc/FrameCounter.h"

/*
移动瓦片,移动方向 上下左右
*/
class RObject;
class Collapse
{
private:
	unsigned short mapTag = 1;
	MovingDirection md;

	std::vector<std::string> colNames;
 
	bool isDone = false;
	bool triggered = false;
	Vec2 startPosition = Vec2::ZERO;
	Vec2 range;
public:
	/**
	* 选择范围
	* @param rangeStart
	* @param rangeEnd
	* @param 移动偏移坐标
	* @param 移动时间
	*/
	Collapse(const unsigned short &mapTag = ID_Map, const Vec2& rangeStart= Vec2::ZERO, const Vec2& = Vec2::ZERO, const Vec2& step= Vec2(2.f,2.f), const MovingDirection& md = MovingDirection::stayStill, const Vec2& range=ScreenSize);
	Collapse();
	void collapse();
	inline bool& isFinsihed() { return isDone; }
	inline bool& isTriggered() { return triggered; }
	inline void setTriggered() { triggered = true; }
};

#endif