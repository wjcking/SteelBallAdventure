#ifndef TRIGGER_REGION_H
#define TRIGGER_REGION_H
#include "cocos2d.h"
#include "../Utils/RectTools.h"
#include "../../Common/Misc/Utils.h"
using namespace cocos2d;

class TriggerRegion
{
protected:
	BoundPosition boundPosition = BoundPosition::centerSelf;
	bool isTouched = false;
public:

	virtual ~TriggerRegion() {  }
	//returns true if an entity of the given size and position is intersecting
	//the trigger region.
	virtual bool& isTouching(const Rect& = Rect::ZERO, float radius = 0) = 0;
	//用来刷新精灵触发碰撞的位置
	virtual void updateRegion(const Rect&, const float& radius = 0) = 0;
	virtual void render(DrawNode*) = 0;

	inline bool& isKeptTouched() { return isTouched; }
	//trigger=7 len
	inline std::string getString() { return getTypeName(*this).erase(0,7); }
};




#endif