#pragma once 
#include "Role.h"


using namespace std;

class RObject : public Role
{
private:

	//方向移动完成个数配合 moves 使用
	unsigned short mobileFinished = 0;
	//碎片颜色
	unsigned short debrisColor;
	vector<MovePhase> moves;
	//顶在物体上
	vector<KnockPhase> knocks;
	//若隐若现,如果为编组的则按照条件以及时常,单个的按照时常和触发rect
	BlinkPhase blink;
	//开关，按钮变换
	FrameSwitch frameSwitchTemplate;
	std::unordered_map<int, FrameSwitch> frameSwitch;
	//bool isLabeled
	LabelPhase labelPhase;
	BlinkStatus  blinkObject();
	Size originSize;
	//弹簧设置
	SpringInfo springInfo;

	//刷新弹簧
	void updateSpring();
	//void proceedKnocks(function<void(KnockPhase&)> knoPhase);
	//是否离开弹簧或开关，区域判断 刚好离开物体的那个节骨眼
	bool isGettingOff(Role&, const Vec2& = Vec2(10.f, 10.f));

public:
	RObject();
	~RObject();
	CREATE_FUNC(RObject);
	/*
	*基础函数
	*/
	void update() override;
	void loadScript() override;
	/*
	*执行动作
	*/

	KnockPhase gotKnocked(Role&);
	void checkKnocks(Role&);
	void blinkSelf();
	void blinkGroup();
	//注意step Vec2 最好大于0.7否则碰撞不准确
	SpringInfo& bounce(Role&);
	std::string pollChar();
	FrameSwitch switchFrame(Role&);
	/*
	*注册脚本
	*/
	void registerSpring(const LuaRef&);

	void registerChar(const LuaRef&);
	void registerText(const LuaRef&);
	void registerKnocks(const LuaRef&);
	void registerSwitch(const LuaRef&);

	void setAnimation(const LuaRef&) override;
	bool checkObb(Role& opponent, const bool& allowFollow);
	inline FrameSwitch& getSwitch(const int& tag) { return frameSwitch[tag]; }
	inline bool hasSwitches() { return frameSwitchTemplate.framePrefix != ""; }
	//敲击, 有敲击设置则发出, 而且还要判断敲击的位置
	inline bool hasKnocks(const CollisionDirection& direction)
	{
		if (knocks.size() <= 0 )
			return false;
		if (knocks.begin()->colliderDirection == CollisionDirection::intersected)
			return true;
		if ( knocks.begin()->colliderDirection != direction)
			return false;

		return true;
	}
	inline bool isFloat() { return moves.size() > 0; }
	inline char& getChar() { return labelPhase.current; };
	//unsigned short resetFramework(const std::string&);
	inline void setBlink(const BlinkPhase& val) { blink = val; };
	inline void setDebrisColor(const unsigned short& val) { debrisColor = val; };
	//在分组内时会调用
	inline BlinkPhase& getBlink() { return blink; };
	inline const unsigned short& getDebrisColor() const { return debrisColor; };

};
