#include "Npc.h"
#include "../Map/TiledMap.h"
#include "../Map/MapManager.h"
#include "../Utils/Fand.h"
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
	auto boundRacts = getMap()->getBoundTiles(*this);
 
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
		auto boundRact = boundRacts[boundIndex];

		if (allowWalls)
			canMove = (boundRact.getProperty().gid == 0 || boundRact.getProperty().allowThrough);
		else
			canMove = boundRact.getProperty().gid != 0 && getPositionX() >= getMap()->getTileSize().width;

		if (canMove)
			setBackward();
		else
		{
			//����sprite�ߴ��жϣ��������tile����Ҫ
			auto realMapPostion = getMap()->getPositionByTileCoordinate(boundRact.tilePosition);
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
		auto boundRact = boundRacts[boundIndex];

		if (allowWalls)
			canMove = (boundRact.getProperty().gid == 0 || boundRact.getProperty().allowThrough);
		else
			canMove = boundRact.getProperty().gid != 0 && getPositionX() <= getMap()->getTotalSize().width;

		if (canMove)
			setForward();
		else
		{
			//����sprite�ߴ��жϣ��������tile����Ҫ
			auto realMapPostion = getMap()->getPositionByTileCoordinate(boundRact.tilePosition);
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
	if (nullptr == boundRacts)	return;
	//�������ص�
	if (isGravityOn)	isGravityOn = false;

	BoundRact boundRact;
	if (patrollDirectionY == MovingDirection::toTop)
	{
		boundRact = boundRacts[static_cast<unsigned short>(BoundPosition::top)];

		if (boundRact.getProperty().gid == 0)
			setUpward();
		else
		{
			auto realMapPostion = MAP_STAGE->getPositionByTileCoordinate(boundRact.tilePosition);
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
		boundRact = boundRacts[static_cast<unsigned short>(BoundPosition::bottom)];

		if (boundRact.getProperty().gid == 0)
			setDownward();
		else
		{
			auto realMapPostion = MAP_STAGE->getPositionByTileCoordinate(boundRact.tilePosition);
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
