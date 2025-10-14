#pragma once
#include "cocos2d.h"
#include "../Utils/Fand.h"
#include "../Utils/Constant.h"
#include "Fenk.h"
using namespace cocos2d;

class Property : public Sprite
{
protected:
	int id;
	Vec2 desiredPosition;
	//Ĭ��ֵ���ı��һ��ʱ����մ˱�׼�ظ�
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
	//ת�����λ��
	Vec2 originPosition;
	//������
	unsigned short  jumpTimes = 0;
	//���û�а�ס��Ծ��ʱ����Ծ��

	//�ƶ�����
	MovingDirection moveDirection = MovingDirection::stayStill;
	//Ԥ�������״̬
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
	const MovingDirection getMovingX() const
	{
		if (moveDirection == MovingDirection::stayStill)
			return MovingDirection::stayStill;

		if (moveDirection == MovingDirection::toLeft || moveDirection == MovingDirection::toBottomLeft || moveDirection == MovingDirection::toTopLeft)
			return MovingDirection::toLeft;

		return MovingDirection::toRight;
	}
	const MovingDirection getMovingY() const
	{
		if (moveDirection == MovingDirection::stayStill)
			return MovingDirection::stayStill;

		if (moveDirection == MovingDirection::toTop || moveDirection == MovingDirection::toTopLeft || moveDirection == MovingDirection::toTopRight)
			return MovingDirection::toTop;

		return MovingDirection::toBottom;
	}
	//��ʼ����͵�ǰ����
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
	//Ŀ�ĵ�
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

	//���ٶ�
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
	//�ָ������Ŀ���
	inline void recoverGravity() { isGravityOn = originGravity; }
	//��������������ʼֵ
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
	bool inAir(const bool = false);
	void setSlopeSteps(const unsigned short& degree = -1)
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