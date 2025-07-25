#pragma once
#include "cocos2d.h"
#include "../Utils/RectTools.h"
#include "../Utils/Constant.h"
#include "Side.h"
using namespace cocos2d;

class Property : public Sprite
{
protected:
	int id;
	Vec2 desiredPosition;
	//默认值，改变后一段时间后按照此标准回复
	Vec2 defaultMoveStep = Vec2(1.55f, 1.55f);
	Vec2 defaultJumpForce = Vec2(0.0, 6.05f);
	Vec2 defaultJumpCutOff = Vec2(0.0, 6.05f);
	Vec2 defaultGravity = Vec2(0.0, -0.11f);

	//Vec2 defaultWallForce;
	Vec2 jumpForceOnWall = Vec2(0.0f, 3.8f);
	Vec2 jumpForce = defaultJumpForce;
	Vec2 jumpCutoff = defaultJumpCutOff;

	Vec2 gravity = defaultGravity;
	Vec2 moveStep = defaultMoveStep;
	Vec2 velocity = Vec2::ZERO;
	//转换后的位置
	Vec2 originPosition;
	//二段跳
	unsigned short  jumpTimes = 0;
	//玩家没有按住跳跃键时的跳跃力

	//移动方向
	MovingDirection moveDirection = MovingDirection::stayStill;
	//预存的重力状态
	bool originGravity = true;
	bool isGravityOn = true;
	bool isJumped = false;
	bool isJumping = false;
	bool onFalling;
	bool onSlopeBottom = false;
	bool onGround = false;
	bool onObject = false;
	bool onWall = false;
public:
	Property();
	inline const bool& getOnObject() { return this->onObject; }
	inline const Vec2& getMoveStep() { return this->moveStep; }
	inline const MovingDirection& getDirection() const { return this->moveDirection; }
	inline bool isMoving() { return moveDirection != MovingDirection::stayStill; }
	inline void setDirection(const MovingDirection& val)   { moveDirection = val; }
	inline const MovingDirection getMovingX() const
	{
		if (moveDirection == MovingDirection::stayStill)
			return MovingDirection::stayStill;

		if (moveDirection == MovingDirection::toLeft || moveDirection == MovingDirection::toBottomLeft || moveDirection == MovingDirection::toTopLeft)
			return MovingDirection::toLeft;

		return MovingDirection::toRight;
	}
	inline const MovingDirection getMovingY() const
	{
		if (moveDirection == MovingDirection::stayStill)
			return MovingDirection::stayStill;

		if (moveDirection == MovingDirection::toTop || moveDirection == MovingDirection::toTopLeft || moveDirection == MovingDirection::toTopRight)
			return MovingDirection::toTop;

		return MovingDirection::toBottom;
	}
	//起始距离和当前距离
	inline void displace(const Vec2& realPos)
	{
		setPosition(realPos);
		setDesire(realPos);
	};
	inline Vec2 getDistance() { return Vec2(abs(getPositionX() - originPosition.x), abs(getPositionY() - originPosition.y)); }
	inline void setOrigin(const Vec2& val) { this->originPosition = val; };
	inline Vec2& getOrigin() { return this->originPosition; };
	inline void respawn() { displace(originPosition); };
	inline void refreshOrigin() { originPosition = getPosition(); }
	inline void setForward() { moveDirection = MovingDirection::toRight; }
	inline void setBackward() { moveDirection = MovingDirection::toLeft; }

	inline void setUpward() { moveDirection = MovingDirection::toTop; }
	inline void setDownward() { moveDirection = MovingDirection::toBottom; } 
	inline void stop() { moveDirection = MovingDirection::stayStill; }
	inline void setonObject(const bool& onOff) { this->onObject = onOff; }
	inline bool& getGravityStatus() { return isGravityOn; }
	inline bool& getJumpStatus() { return this->isJumping; }
	inline void setOnJump(const bool& jump) { this->isJumped = jump; }
	inline const bool& getOnJump() const { return this->isJumped; }
	//目的地
	inline void setDesire(const Vec2& v) { this->desiredPosition = v; }
	inline void setDesire(const float& x, const float& y)
	{
		this->desiredPosition.x = x;
		this->desiredPosition.y = y;
	}
	inline void addDesired(const float& x, const float& y)
	{
		this->desiredPosition.x += x;
		this->desiredPosition.y += y;
	}
	inline void setDesiredX(const float& x) { this->desiredPosition.y = x; }
	inline void setDesiredY(const float& y) { this->desiredPosition.y = y; }
	inline void addDesiredX(const float& x) { this->desiredPosition.x += x; }
	inline void addDesiredY(const float& y) { this->desiredPosition.y += y; }

	//加速度
	inline void setVelocity(const float& x, const float& y)
	{
		this->velocity.x = x;
		this->velocity.y = y;
	}
	inline void addVelocity(const float& x, const float& y)
	{
		this->velocity.x += x;
		this->velocity.y += y;
	}
	inline void addVelocityX(const float& x) { this->velocity.x += x; }
	inline void addVelocityY(const float& y) { this->velocity.y += y; }
	inline void setVelocityX(const float& x) { this->velocity.x = x; }
	inline void setVelocityY(const float& y = 0.f) { this->velocity.y = y; }
	//恢复重力的开关
	inline void recoverGravity() { isGravityOn = originGravity; }
	//重新设置重力初始值
	inline void resetGravity() { gravity = defaultGravity; }
	inline void setGravity(const Vec2& newGravity) { gravity = newGravity; }
	inline void setGravityX(const float& x) { gravity.x = x; }
	inline void setGravityY(const float& y) { gravity.y = y; }
	//inline void reverseGravity() { }
	inline void setGravityOn() { isGravityOn = true; }
	inline void setGravityOff() { isGravityOn = false; }
	inline void setGravityStatus(const bool& value) { isGravityOn = value; }
	inline void resetVelocity() { this->velocity = Vec2::ZERO; }
	inline void resetMoveSteps() { moveStep = defaultMoveStep; }
	inline void setMoveStep(const Vec2& value) { moveStep = value; }
	inline void setMoveStepX(const float& value) { moveStep.x = value; }
	inline void setMoveStepY(const float& value) { moveStep.y = value; }
	inline void setJumpForce(const Vec2& value) { jumpForce = value; }
	inline void setJumpCut(const Vec2& value) { jumpCutoff = value; }
	inline void setJumpForceOnWall(const Vec2& value) { jumpForceOnWall = value; }
	bool inAir(const bool isClear = false);
	inline void setSlopeSteps(const unsigned short& degree = -1)
	{
		switch (degree)
		{
		case 22:
			return;
		case 45:
			return;
		default:
			return;
		}
	}
};