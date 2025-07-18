#pragma once 
#include "Role.h"
#include "../Utils/Constant.h"
#include "../Utils/OBB.h"
#include "../../Common/Misc/FrameCounter.h"
#include "../../Common/StateMachine/StateMachine.h"

class RObject;
class Player : public Role
{
private:
	//受伤延迟
	DelayPhase delayHurting;
	std::vector<EventKeyboard::KeyCode> keyVector;
	//是否能对主角进行控制，对话时或者死亡时
	bool controllable = true;
	Size originSize;
	//这里用role* 会出现内存问题
	RObject* carriedObject = nullptr;
	bool bCarrying = false;
	Vec2 carriedPostion;
	Vec2 carriedOffset = Vec2(8,8);
	void checkKnapsack();
public:

	Player();
	~Player();

	void setAnimation(const LuaRef& ref)override { Role::setAnimation(ref); };
	inline void freeze()
	{
		stop();
		controllable = false;
	};
	inline void unfreeze() { controllable = true; };
	inline bool isFreezed()
	{
		setOnJump(false);
		return  controllable == false ? true : false;
	}
	void update() override;
	void controlByKey(EventKeyboard::KeyCode&, const bool&);
	void setPositionInCamera(const bool&, const bool&, const Vec2&);
	//inline bool isCarrying() { return nullptr != carriedObject; }
	inline bool& isCarrying() { return bCarrying; }
	void carryObject(RObject&);
	inline void setCarriedOffset(const Vec2& offset) { carriedOffset = offset; }
	void dropObject();
	inline bool isDead() override
	{
		auto isDead = Role::isDead();
		if (isDead)
			setOnJump(false);
		return isDead;
	}
	inline void gotoHell(const char* sound = "") override
	{
		setOnJump(false);
		Role::gotoHell(sound);
	}
};