#include "Collapse.h"  
#include "MapManager.h"
#include "../Role/RoleManager.h"
#include "TiledMap.h"
#include "../Role/Player.h"
#include "../Role/RObject.h"
#include "../Utils/GameScriptor.h"
using namespace std;

Collapse::Collapse()
{
	colNames.reserve(Reserve_65534);
}

void Collapse::collapse()
{
	//���س��������������
	if (isDone)
		return;

	Node* node;

	node = MAP_WITHTAG(mapTag)->getChildByName(colNames[0]);
	//1�������ж�����ǰ��
	auto sub = Vec2(abs(node->getPosition().x), abs(node->getPosition().y)) - Vec2(abs(startPosition.x), abs(startPosition.y));
	auto isDestiny = false;
	//(sub.x != 0 && (sub.x >= range.x)) ||
	if ((sub.y != 0 && (sub.y >= range.y)))
	{
		auto robject = dynamic_cast<RObject*>(node);
		isDestiny = true;
	}
	for (auto tag : colNames)
	{
		auto node = MAP_WITHTAG(mapTag)->getChildByName(tag);
		auto robject = dynamic_cast<RObject*>(node);

		//2�� ��ʱ����ͣ��
		if (isDestiny)
		{
			robject->stop();
			robject->setVisible(false);
			robject->disallowCollision();
			isDone = true;
			continue;
		}
		robject->setDirection(md);
	}

}

Collapse::Collapse(const unsigned short &mapTag, const Vec2& rangeStart, const Vec2& rangeEnd, const Vec2& step, const MovingDirection& md,const Vec2& range)
	:mapTag(mapTag),
	md(md), 
	range(range)
{
	RObject* collisionTile = nullptr;

	MAP_WITHTAG(mapTag)->processTileRange(rangeStart, rangeEnd, [&](const Vec2& rangeTile, const short& i) {
		auto tile = MAP_WITHTAG(mapTag)->getWalls().getTileAt(rangeTile);
		if (nullptr == tile)
		{
			log("[Collapse]tile Collapse incorrect settings or no such tile coordinates");
			isDone = true;
			return;
		}

		//bool isOutOfRange = rangeTile.x >= MAP_WITHTAG(mapTag)->getMapSize().width || rangeTile.y >= MAP_WITHTAG(mapTag)->getMapSize().height;
		//auto mapSize = MAP_WITHTAG(mapTag)->getMapSize();
		bool isInRange = rangeTile.x < MAP_WITHTAG(mapTag)->getMapSize().width && rangeTile.y < MAP_WITHTAG(mapTag)->getMapSize().height;
		CCASSERT(isInRange, "collapse��ͼԽ��");
		if (!isInRange)
		{
			log("[Collapse]out of range");
			return;
		}
		auto tileFrame = tile->getSpriteFrame();

		CCASSERT(nullptr != tileFrame, "mapTag tile frameΪ��");
		collisionTile = Role::createWithFrame<RObject>(tileFrame);;
		collisionTile->setName(StringUtils::format("collapse%d", collisionTile->getTag()));
		colNames.push_back(collisionTile->getName());
		collisionTile->setMoveStep(step);
		collisionTile->setPosition(MAP_WITHTAG(mapTag)->getPositionByTileCoordinate(rangeTile));
		if (i == 0)
			startPosition = collisionTile->getPosition();
		//�Ƴ���ǰtiledmap�е�sprite
		MAP_WITHTAG(mapTag)->getWalls().removeTileAt(rangeTile);
		//���ݵ�ͼid���
		MAP_WITHTAG(mapTag)->addChild(collisionTile);
		ROLE_MANAGER->registerRole(collisionTile);
		//ע�ᵽ�ű�
		LUAH->registerRole("collapse","collapse",collisionTile);
	});
}


