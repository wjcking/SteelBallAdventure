#include "Player.h" 
#include "RObject.h"
#include "../Map/MapManager.h"
#include "../Map/TiledMap.h"
#include "../Utils/Constant.h"
#include "../Scene/StageScene.h"
#include "../Scene/MainScene.h"

Player::Player() :Role(), controllable(true)
{
	isSolid = false;
	type = RoleType::player;
}

Player::~Player()
{

}

void Player::checkKnapsack()
{
	//ûЯ���������˳�,�����±�����Ʒλ��
	if (!bCarrying)	return;

	if (isDead())
	{
		dropObject();
		return;
	}
	//�����ص�
	carriedObject->setGravityOff(); 

	switch (facedDirection)
	{
	case MovingDirection::toLeft:
		carriedPostion.x = getCollisionBound().getMidX() + carriedObject->getCollisionBound().size.width + carriedOffset.x;
		carriedPostion.y = getCollisionBound().getMidY() + carriedOffset.y;
		break;
	case MovingDirection::toRight:
		carriedPostion.x = getCollisionBound().getMidX() - carriedObject->getCollisionBound().size.width - carriedOffset.x;
		carriedPostion.y = getCollisionBound().getMidY() + carriedOffset.y;
		break;
	case MovingDirection::toTop:
		carriedPostion.y = getCollisionBound().getMidY() - carriedObject->getCollisionBound().size.height - carriedOffset.y;
		carriedPostion.x = getCollisionBound().getMidX();
		break;
	case MovingDirection::toBottom:
		carriedPostion.y = getCollisionBound().getMidY() + carriedObject->getCollisionBound().size.height + carriedOffset.y;
		carriedPostion.x = getCollisionBound().getMidX();
		break;
	default:
		break;
	}

	carriedObject->setPosition(carriedPostion);
}
void Player::update()
{
	//�������
	checkKnapsack();
	//һ�������ȥ��������Ծ
	if (isDead())	setOnJump(false);.

	Role::update();
}

//���̿������ڲ���
void Player::controlByKey(EventKeyboard::KeyCode& keyCode, const bool& flag)
{
	if (isFreezed())
		return;

	if (flag)
	{
		switch (keyCode)
		{
		case EventKeyboard::KeyCode::KEY_D:
			setForward();
			break;
		case EventKeyboard::KeyCode::KEY_A:
			setBackward();
			break;
		case EventKeyboard::KeyCode::KEY_W:
			control(MovingDirection::toTop);
			break;
		case EventKeyboard::KeyCode::KEY_S:
			control(MovingDirection::toBottom);
			break;
		case EventKeyboard::KeyCode::KEY_I:
			setOnJump(true);
			break;
		case EventKeyboard::KeyCode::KEY_U:
			getWeaponSystem().shootAt();
			break;
		}
	}
	else
	{
		switch (keyCode)
		{
		case EventKeyboard::KeyCode::KEY_I:
			setOnJump(false);
			break;
		case EventKeyboard::KeyCode::KEY_U:
			onFire = false;
			break;
		case EventKeyboard::KeyCode::KEY_D:
		case EventKeyboard::KeyCode::KEY_A:
		case EventKeyboard::KeyCode::KEY_W:
		case EventKeyboard::KeyCode::KEY_S:
			moveDirection = MovingDirection::stayStill;
			break;
		}
	}
	//if (flag)
	//	keyVector.push_back(keyCode);
	//else
	//{
	//	if (keyCode == EventKeyboard::KeyCode::KEY_I)
	//		setOnJump(false);

	//	auto key = find(keyVector.begin(), keyVector.end(), keyCode);
	//	if (key != keyVector.end())
	//		keyVector.erase(key);
	//}
}


void Player::setPositionInCamera(const bool& isDestiny, const bool& orientation, const Vec2& perDistance)
{
	Vec2 mapPosition = getMap()->getPosition();
	float horizonStart = abs(mapPosition.x - getBoundingBox().size.width / 2);
	float horizonEnd = abs(mapPosition.x) + ScreenWidth - getBoundingBox().size.width / 2;
	float verticalStart = abs(mapPosition.y - getBoundingBox().size.height / 2);
	float verticalEnd = abs(mapPosition.y) + ScreenHeight - getBoundingBox().size.height / 2;

	if (!isDestiny)
	{
		if (getPositionX() <= horizonStart)
			addDesiredX(horizonStart - getPositionX());
		else if (getPositionX() >= horizonEnd)
			addDesiredX(horizonEnd - getPositionX());
		else
		{
			if (orientation)
				addDesiredX(perDistance.x);
		}

		if (getPositionY() <= verticalStart)
			addDesiredY(verticalStart - getPositionY());
		else if (getPositionY() >= verticalEnd)
			addDesiredY(verticalEnd - getPositionY());
		else
		{
			if (!orientation)
				addDesiredY(perDistance.y);
		}
		gotoDesirePosition();
	}
	//else
	//	limiteScreenBorder(orientation ? BorderLimited::limiteHorizon : BorderLimited::limiteVertical);
}

void Player::carryObject(RObject& object)
{
	//���û��Я�����Ҳ��Ǳ���&& object.getTag() != carriedObject->getTag( )
	if (!bCarrying)
	{
		bCarrying = true;
		carriedObject = &object;
	}
}

void Player::dropObject()
{
	if (bCarrying && carriedObject != nullptr)
		carriedObject->recoverGravity();
	//carriedObject = nullptr;
	bCarrying = false;
};