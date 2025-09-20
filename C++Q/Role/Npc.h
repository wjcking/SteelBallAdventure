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
	inline void setAnimation(const LuaRef& ref) override	{		Role::setAnimation(ref);	};
	void update() override;
	bool setTowards(const Vec2& playerPos)
	{
		if (getBodyStatus() == BodyStatus::dead)
			return false;

		auto faced = getPosition().x > playerPos.x;
		setFlippedX(faced);

		return faced;
	};
	//������ø�����Ծ�Ļ�ע��rangeԶ�� �����Ӱ��Ч��
	void patrollingX(const bool& = false);
	void patrollingY();
	void wanderingX(const float& distance);
};

