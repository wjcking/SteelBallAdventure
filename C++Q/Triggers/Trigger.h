#ifndef TRIGGER_H
#define TRIGGER_H

struct Telegram;
class Role;
#include "cocos2d.h"
#include "../Utils/RectTools.h"
#include "../Utils/Utils.h"
class Trigger
{
protected:
	float distance;
	bool toBeRemoved;
	bool bActive = true;
	bool isTouched = false;
	bool includedTag = false;
	//int graphNodeIndex;
	MovingDirection sensorDirection = MovingDirection::stayStill;
	std::vector<short> relatedTags;
	Size sensorRange;

	unsigned short tag;
	std::string name;
	//根据情况显示边框颜色
	inline cocos2d::Color4F getColor()
	{
		if (bActive)
			return isTouched ? Color4F::RED : Color4F::GREEN;
		else
			return Color4F::BLACK;
	}
public:
	//inline void setGraphNodeIndex(int idx) { graphNodeIndex = idx; }
	inline void remove() { toBeRemoved = true; }
	inline void setTag(const unsigned short& val) { tag = val; }
	inline void setName(const std::string& val) { name = val; }
	//根据是否激活画图
	inline void setActivation(const bool& val) { bActive = val; }

	virtual bool& scanTouching(Role& role) = 0;

	inline float& getDistance() { return distance; } ;
	inline bool& isTouching() { return isTouched; }

	inline bool isRemoved()const { return toBeRemoved; }
	inline bool isActive() { return bActive; }
	//trigger=7 len
	virtual std::string getType() { return getTypeName(*this).erase(0, 7); }
	//inline int  getNodeIndex()const { return graphNodeIndex; }
	inline MovingDirection& getSensorDirection() { return sensorDirection; }
	inline unsigned short& getTag() { return tag; }
	inline std::string& getName() { return name; }
	//如果bActive = true扫描该区域是否发生碰撞，
 

	//[Sensor]用来刷新精灵触发碰撞的位置
	virtual void updateRegion(Role&, const float& radius = 0) = 0;
	virtual void render(DrawNode*) = 0;
	Trigger(const bool& bAct = true);
	Trigger(const Size& size, const MovingDirection& md);
	virtual ~Trigger();
};

class TriggerLine : public Trigger
{
private:
	Vec2 start;
	Vec2 end;
public:

	TriggerLine(const Vec2& start, const Vec2& end);
	TriggerLine(const Size& range, const MovingDirection& md);
	TriggerLine();
	~TriggerLine();

	bool& scanTouching(Role& entityRect)   override;
	void render(DrawNode* draw) override;
	void updateRegion(Role& role, const float& radius = 0) override;
};
class TriggerCircle : public Trigger
{
private:
	//the center of the region
	Rect circle;
	float radius;

public:

	TriggerCircle(const Rect& rect, const float& radius = 0);
	TriggerCircle(const Size& range, const MovingDirection& md);
	TriggerCircle();
	~TriggerCircle();
	virtual bool& scanTouching(Role& entityRect) override;

	void render(DrawNode* draw)override;
	void updateRegion(Role& role, const float& radius = 0) override;
};
class TriggerRect : public Trigger
{
private:
	Rect rect;
public:

	TriggerRect(const Rect& rect, const bool& isAct = true);

	TriggerRect(const Size& range, const MovingDirection& md);
	TriggerRect();
	~TriggerRect();

	virtual bool& scanTouching(Role& rect)  override;

	void render(DrawNode* draw)override;
	void updateRegion(Role& role, const float& radius = 0) override;
};
#endif