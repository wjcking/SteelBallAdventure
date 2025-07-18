#pragma once
#include "Role.h"

#include "../../../Common/StateMachine/StateMachine.h"
#include "../Map/MapProperty.h"

class Npc : public Role
{
private:
	MovingDirection patrollDirectionX = facedDirection;
	MovingDirection wanderDirectionX = facedDirection;
	MovingDirection patrollDirectionY = facedDirection;
public:

	Npc();
	~Npc();

	CREATE_FUNC(Npc);
	inline void setAnimation(const LuaRef& ref)override
	{
		Role::setAnimation(ref);
	};
	void update() override;
	inline bool setTowards(const Vec2& playerPos)
	{
		if (getBodyStatus() == BodyStatus::dead)
			return false;

		auto faced = getPosition().x > playerPos.x;
		setFlippedX(faced);

		return faced;
	};
	//如果设置跟随跳跃的话注意range远近 否则会影响效果
	void patrollingX(const bool& = false);
	void patrollingY();
	void wanderingX(const float& distance);
};

