#pragma once 
#include "Role.h"
#include "../Matric/Constant.h"
#include "../Matric/OBB.h"
#include "../../Common/Misc/FrameCounter.h"
#include "../../Common/StateMachine/StateMachine.h"

class RObject;
class Player : public Role
{
private:
	//�����ӳ�
	DelayPhase delayHurting;
	std::vector<EventKeyboard::KeyCode> keyVector;
	//�Ƿ��ܶ����ǽ��п��ƣ��Ի�ʱ��������ʱ
	bool controllable = true;
	Size originSize;
	//������role* ������ڴ�����
	RObject* carriedObject = nullptr;
	bool bCarrying = false;
	Vec2 carriedPostion;
	Vec2 carriedOffset = Vec2(8,8);
	void checkKnapsack();
public:

	Player();
	~Player();

	void setAnimation(const LuaRef& ref)override { Role::setAnimation(ref); };

	void update() override;
	void controlByKey(EventKeyboard::KeyCode&, const bool&);
	void setPositionInCamera(const bool&, const bool&, const Vec2&);
	//inline bool isCarrying() { return nullptr != carriedObject; }
	inline bool& isCarrying() { return bCarrying; }
	void carryObject(RObject&);
	inline void setCarriedOffset(const Vec2& offset) { carriedOffset = offset; }
	void dropObject();

	inline void unfreeze() { controllable = true; };
	inline void freeze()
	{
		stop();
		controllable = false;
	}

	inline bool isFreezed()	
	{
		setOnJump(false);
		controllable = controllable == false ? true : false;
		return controllable
	}

	bool isDead() override
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