#include "Npc.h"
#include "../Map/TiledMap.h"
#include "../Map/MapManager.h"
#include "../Utils/RectTools.h"
#include "../Utils/Constant.h"

Npc::Npc() :Role()
{
	setHarmful();
	type = RoleType::npc;
}
Npc::~Npc()
{

}
void Npc::update()
{
	Role::update();
}

void Npc::patrollingX(const bool& allowWalls)
{
	//ÿ��ˢ֡����ȡ������Android����
	auto boundRects = getMap()->getBoundTiles(*this);
 
	bool canMove = false;

	if (patrollDirectionX == MovingDirection::toLeft)
	{
		//�ڿ��е�ʱ��ֱ���ƶ�
		//if (inAir(false))
		//{
		//	setBackward();
		//	return;
		//}
		auto boundIndex = allowWalls ? static_cast<unsigned short>(BoundPosition::left) : static_cast<unsigned short>(BoundPosition::bottomLeft);
		auto boundRect = boundRects[boundIndex];

		if (allowWalls)
			canMove = (boundRect.getProperty().gid == 0 || boundRect.getProperty().allowThrough);
		else
			canMove = boundRect.getProperty().gid != 0 && getPositionX() >= getMap()->getTileSize().width;

		if (canMove)
			setBackward();
		else
		{
			//����sprite�ߴ��жϣ��������tile����Ҫ
			auto realMapPostion = getMap()->getPositionByTileCoordinate(boundRect.tilePosition);
			//��ǽ�ľ���
			auto distance = abs(realMapPostion.x - getPositionX());
			if (distance <= getContentSize().width)
			{
				patrollDirectionX = MovingDirection::toRight;
				setForward();
			}
		}
	}
	else if (patrollDirectionX == MovingDirection::toRight)
	{
		//�ڿ��е�ʱ��ֱ���ƶ�
		//if (inAir(false))
		//{
		//	setForward();
		//	return;
		//}
		auto boundIndex = allowWalls ? static_cast<unsigned short>(BoundPosition::right) : static_cast<unsigned short>(BoundPosition::bottomRight);
		auto boundRect = boundRects[boundIndex];

		if (allowWalls)
			canMove = (boundRect.getProperty().gid == 0 || boundRect.getProperty().allowThrough);
		else
			canMove = boundRect.getProperty().gid != 0 && getPositionX() <= getMap()->getTotalSize().width;

		if (canMove)
			setForward();
		else
		{
			//����sprite�ߴ��жϣ��������tile����Ҫ
			auto realMapPostion = getMap()->getPositionByTileCoordinate(boundRect.tilePosition);
			//��ǽ�ľ���
			auto distance = abs(realMapPostion.x - getPositionX());
			if (distance <= getContentSize().width)
			{
				patrollDirectionX = MovingDirection::toLeft;
				setBackward();
			}
		}
	}
}

void Npc::patrollingY()
{
	if (nullptr == boundRects)	return;
	//�������ص�
	if (isGravityOn)	isGravityOn = false;

	BoundRect boundRect;
	if (patrollDirectionY == MovingDirection::toTop)
	{
		boundRect = boundRects[static_cast<unsigned short>(BoundPosition::top)];

		if (boundRect.getProperty().gid == 0)
			setUpward();
		else
		{
			auto realMapPostion = MAP_STAGE->getPositionByTileCoordinate(boundRect.tilePosition);
			//��ǽ�ľ���
			auto distance = abs(realMapPostion.y - getPosition().y);
			if (distance <= MAP_STAGE->getTileSize().height)
			{
				patrollDirectionY = MovingDirection::toBottom;
				setDownward();
			}

		}
	}
	else if (patrollDirectionY == MovingDirection::toBottom)
	{
		boundRect = boundRects[static_cast<unsigned short>(BoundPosition::bottom)];

		if (boundRect.getProperty().gid == 0)
			setDownward();
		else
		{
			auto realMapPostion = MAP_STAGE->getPositionByTileCoordinate(boundRect.tilePosition);
			//��ǽ�ľ���
			auto distance = abs(realMapPostion.y - getPosition().y);
			if (distance <= MAP_STAGE->getTileSize().height)
			{
				patrollDirectionY = MovingDirection::toTop;
				setUpward();
			}
		}
	}
}

void Npc::wanderingX(const float & distance)
{
	if (wanderDirectionX == MovingDirection::toLeft)
	{
		setBackward();	
		if (getDistance().x <= 0)
		{
			setForward();
			wanderDirectionX = MovingDirection::toRight;
		}

	}
	else if (wanderDirectionX == MovingDirection::toRight)
	{
		setForward();
		if (getDistance().x > distance)
		{
			setBackward();
			wanderDirectionX = MovingDirection::toLeft;
		}

	}
}
