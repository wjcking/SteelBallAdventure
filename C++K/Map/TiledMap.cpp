#include "TiledMap.h"
#include "MapManager.h"
#include "../Role/RoleManager.h"
#include "../Role/Role.h"
#include "../Utils/Constant.h"
#include "../Triggers/Trigger.h"
#include "../Utils/GameScriptor.h"
#include "../Role/RObject.h"
int TiledMap::nextTag = 1;
TiledMap::TiledMap() :secondMap(nullptr), inSecondMap(false), lastRoleX(0.f), isStartOff(true)
{
	drawCollision = DrawNode::create();
	addChild(drawCollision);
}
TiledMap*TiledMap::create(const std::string& tmxFile)
{
	assert(!tmxFile.empty() && "Invalid filename for TiledMap");

	auto map = new TiledMap();

	map->setTag(nextTag++);
	if (map && map->initWithTMXFile(tmxFile))
	{
		map->autorelease();

		map->tileProperties[0].isSlope = false;
		map->tileProperties[0].isHarmful = false;
		//tileSize = map->getTileSize();
		const char BoolTrue[5] = "true";
		for (auto tp : map->_tileProperties)
		{
			auto& valueMap = tp.second.asValueMap();

			auto ent = valueMap.find(PIsSlope);
			map->tileProperties[tp.first].gid = tp.first;
			map->tileProperties[tp.first].isSlope = ent != valueMap.end() ? (valueMap.at(PIsSlope).asString() == BoolTrue ? true : false) : false;
			ent = valueMap.find(PSlopeDegree);
			map->tileProperties[tp.first].slopeDegree = ent != valueMap.end() ? valueMap.at(PSlopeDegree).asInt() : 0;
			ent = valueMap.find(PSlopeTowards);
			map->tileProperties[tp.first].slopeTowards = ent != valueMap.end() ? valueMap.at(PSlopeTowards).asInt() : SlopeTowards::faceNothing;
			ent = valueMap.find(PSlopeGravity);
			map->tileProperties[tp.first].slopeGravity = ent != valueMap.end() ? valueMap.at(PSlopeGravity).asFloat() : -3.5f;
			ent = valueMap.find(PRoleOffsetY);
			map->tileProperties[tp.first].roleOffsetY = ent != valueMap.end() ? valueMap.at(PRoleOffsetY).asFloat() : 0.f;
			ent = valueMap.find(PAllowThrough);
			map->tileProperties[tp.first].allowThrough = ent != valueMap.end() ? (valueMap.at(PAllowThrough).asString() == BoolTrue ? true : false) : false;
			ent = valueMap.find(PInitSlopeY);
			map->tileProperties[tp.first].initSlopeY = ent != valueMap.end() ? (valueMap.at(PInitSlopeY).asString() == BoolTrue ? true : false) : false;
			ent = valueMap.find(PIsOneWay);
			map->tileProperties[tp.first].isOneWay = ent != valueMap.end() ? (valueMap.at(PIsOneWay).asString() == BoolTrue ? true : false) : false;
			ent = valueMap.find(PIsHarmful);
			map->tileProperties[tp.first].isHarmful = ent != valueMap.end() ? (valueMap.at(PIsHarmful).asString() == BoolTrue ? true : false) : false;
			//	auto a = ent != valueMap.end() ? (valueMap.at(PIsHarmful).asString() == BoolTrue ? true : false) : false;
				//inset
			ent = valueMap.find(PInsetX);
			map->tileProperties[tp.first].inset.origin.x = ent != valueMap.end() ? valueMap.at(PInsetX).asFloat() : 0.0f;
			ent = valueMap.find(PInsetY);
			map->tileProperties[tp.first].inset.origin.y = ent != valueMap.end() ? valueMap.at(PInsetY).asFloat() : 0.0f;
			ent = valueMap.find(PInsetWidth);
			map->tileProperties[tp.first].inset.size.width = ent != valueMap.end() ? valueMap.at(PInsetWidth).asFloat() : 0.0f;
			ent = valueMap.find(PInsetHeight);
			map->tileProperties[tp.first].inset.size.height = ent != valueMap.end() ? valueMap.at(PInsetHeight).asFloat() : 0.0f;

		}

		return map;
	}
	CC_SAFE_DELETE(map);
	return nullptr;
}

void TiledMap::createRepeat(const std::string & tmxFile, const bool&)
{
	secondMap = TiledMap::create(tmxFile); //���ڶ��ŵ�ͼ��λ�����õ��ڶ��ŵ�ͼ�ĺ���
	secondMap->setPositionX(getPositionX() + getContentSize().width - mapOffsetX);
}


TiledMap::TiledMap(const string& tmxFile)
{

}

TiledMap::~TiledMap()
{
	triggerSystem.clear();
}

bool TiledMap::exchangeMap(Role* role)
{
	////����ڵ�һ�ŵ�ͼ
	////if (!inSecondMap)
	////{
	//	//������ ����Ѿ�����ڶ��ŵ�ͼ
	//	if (role->getPositionX() >= getTotalSize().width)
	//	{
	//		auto px = role->getPositionX();
	//		//���������һ�ź͵ڶ��ŵ�ͼ�Ľ��紦
	//		inSecondMap = true;
	//		auto diff = role->getPositionX() - getTotalSize().width;
	//		secondMap->addChild(role);
	//		role->setPositionX(-role->getMoveStep().x / 2);
	//		//�мǼ�¼�ڶ��ŵ�ͼ����һ��X����
	//		lastRoleX = 0;
	//		secondMap->lastRoleX = role->getPositionX();
	//		removeChild(role, false);
	//		return true;
	//	}
	////}
	////else
	////{
	//	//������ ����Ѿ������һ�ŵ�ͼ
	//	if (role->getPositionX() >= secondMap->getTotalSize().width)
	//	{
	//		inSecondMap = false;
	//		auto diff = role->getPositionX() - secondMap->getTotalSize().width;
	//		addChild(role);
	//		//�мǼ�¼��һ�ŵ�ͼ����һ��X����
	//		lastRoleX = role->getPositionX();
	//		secondMap->lastRoleX = 0;
	//		role->setPositionX(-role->getMoveStep().x / 2);
	//		secondMap->removeChild(role, false);
	//		return true;
	//	}
	////}
	return false;
}

TilePropertyInfo&  TiledMap::getProperty(const int& gid)
{
	auto propertyValue = tileProperties.find(gid);

	if (propertyValue != tileProperties.end())
		return propertyValue->second;

	return  tileProperties.at(0);
}

\\ITF~F/\D
\\?|/***AFD**XTS******T**STATE*/bandrect|\/T
BoundRact* TiledMap::getBoundTiles(Role& role, const std::string& layerName)
{
	auto position = role.getPosition();

	if (inSecondMap[role.getTag()])
		return secondMap->getBoundTiles(role, layerName);
	//  ���ش˴���tile����
	Vec2 tilePosition = this->getTileCoordinateByPosition(position);
	const Vec2 borderMax = Vec2(getMapSize().width, getMapSize().height);
	const Vec2 borderMin = Vec2::ZERO;

	if (position.y <= 0 || position.y < getTileSize().height || position.x > getTotalSize().width || position.y > getTotalSize().height)
		return nullptr;

	auto layer = getLayer(layerName);
	//CCLOG("[ TiledMap::getBoundTiles]null of the map or no tiles");
	if (nullptr == layer)
		return nullptr; 

	Sprite* movingTile;

	//��=0
	//����ʱ����������0xcccccccc
	auto boundRacts = new BoundRact[BandRact];
	Vec2 tilePos = Vec2(tilePosition.x, tilePosition.y + 1);
	int gid = gid = getGidAt(tilePos);
	Ract tileRact = this->getRactByTileCoordinate(tilePos, gid);  //��Χ��

	auto tileProperty = getProperty(gid);

	boundRacts[BoundPosition::bottom] = BoundRact(gid, tilePos, tileRact);
	boundRacts[BoundPosition::bottom].setTileProperty(tileProperty);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);

		if (nullptr != movingTile)
			boundRacts[BoundPosition::bottom].tileName = movingTile->getName();
	}

	//��=1
	//��ʱ�ж�����Y�����-1�����
	tilePos = Vec2(tilePosition.x, tilePosition.y <= 0 ? 0 : (tilePosition.y - 1));
	//gid = layer->getTileGIDAt(tilePos);
	gid = getGidAt(tilePos);
	tileRact = this->getRactByTileCoordinate(tilePos, gid);

	tileProperty = getProperty(gid);
	boundRacts[BoundPosition::top] = BoundRact(gid, tilePos, tileRact);
	boundRacts[BoundPosition::top].setTileProperty(tileProperty);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRacts[BoundPosition::top].tileName = movingTile->getName();
	}
	//��=2
	tilePos = Vec2(tilePosition.x - 1, tilePosition.y);
	gid = getGidAt(tilePos);
	tileRact = this->getRactByTileCoordinate(tilePos, gid);
	tileProperty = getProperty(gid);

	boundRacts[BoundPosition::left] = BoundRact(gid, tilePos, tileRact);
	boundRacts[BoundPosition::left].setTileProperty(tileProperty);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRacts[BoundPosition::left].tileName = movingTile->getName();
	}

	//��=3
	tilePos = Vec2(tilePosition.x + 1, tilePosition.y);

	//gid = layer->getTileGIDAt(tilePos);
	//gid = tilePos.x == getMapSize().width ? 0 : layer->getTileGIDAt(tilePos); //����ͼ���ұߣ�gidȴ��ǰһ��x��tile ����
	gid = getGidAt(tilePos);
	tileRact = getRactByTileCoordinate(tilePos, gid);
	tileProperty = getProperty(gid);

	boundRacts[BoundPosition::right] = BoundRact(gid, tilePos, tileRact);
	boundRacts[BoundPosition::right].setTileProperty(tileProperty);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRacts[BoundPosition::right].tileName = movingTile->getName();
	}

	//����=4
	tilePos = Vec2(tilePosition.x - 1, tilePosition.y <= 0 ? 0 : (tilePosition.y - 1));
	//gid = layer->getTileGIDAt(tilePos);
	gid = getGidAt(tilePos);
	tileRact = getRactByTileCoordinate(tilePos, gid);
	tileProperty = getProperty(gid);
	boundRacts[BoundPosition::topLeft] = BoundRact(gid, tilePos, tileRact);
	boundRacts[BoundPosition::topLeft].setTileProperty(tileProperty);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRacts[BoundPosition::topLeft].tileName = movingTile->getName();
	}

	//����=5
	tilePos = Vec2(tilePosition.x + 1, tilePosition.y <= 0 ? 0 : (tilePosition.y - 1));
	//	gid = layer->getTileGIDAt(tilePos);
	gid = getGidAt(tilePos);
	tileRact = getRactByTileCoordinate(tilePos, gid);
	tileProperty = getProperty(gid);
	boundRacts[BoundPosition::topRight] = BoundRact(gid, tilePos, tileRact);
	boundRacts[BoundPosition::topRight].setTileProperty(tileProperty);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRacts[BoundPosition::topRight].tileName = movingTile->getName();
	}
	//����=6
	tilePos = Vec2(tilePosition.x - 1, tilePosition.y + 1);
	//	gid = layer->getTileGIDAt(tilePos);
	gid = getGidAt(tilePos);
	tileRact = getRactByTileCoordinate(tilePos, gid);
	tileProperty = getProperty(gid);

	boundRacts[BoundPosition::bottomLeft] = BoundRact(gid, tilePos, tileRact);
	boundRacts[BoundPosition::bottomLeft].setTileProperty(tileProperty);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRacts[BoundPosition::bottomLeft].tileName = movingTile->getName();
	}

	//����=7
	tilePos = Vec2(tilePosition.x + 1, tilePosition.y + 1);
	//gid = tilePos.x == getMapSize().width ? 0 : layer->getTileGIDAt(tilePos); //����ͼ���ұߣ�gidȴ��ǰһ��x��tile ����
	gid = getGidAt(tilePos);
	tileRact = getRactByTileCoordinate(tilePos, gid);
	tileProperty = getProperty(gid);
	boundRacts[BoundPosition::bottomRight] = BoundRact(gid, tilePos, tileRact);
	boundRacts[BoundPosition::bottomRight].setTileProperty(tileProperty);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRacts[BoundPosition::bottomRight].tileName = movingTile->getName();
	}
	//���Ĳ�λ�Լ�
	tilePos = Vec2(tilePosition.x, tilePosition.y);
	//gid = layer->getTileGIDAt(tilePos);
	gid = getGidAt(tilePos);
	tileRact = getRactByTileCoordinate(tilePos, gid);
	tileProperty = getProperty(gid);

	boundRacts[BoundPosition::centerSelf] = BoundRact(gid, tilePos, tileRact);
	boundRacts[BoundPosition::centerSelf].setTileProperty(tileProperty);
	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRacts[BoundPosition::centerSelf].tileName = movingTile->getName();
	}

	return boundRacts;
}

BoundRact TiledMap::getCenterTile(const Vec2& rolePosition)
{
	auto layer = getLayer(LayerWalls);
	assert(layer != nullptr && "ͼ�㲻��Ϊ��");

	if (rolePosition.y <= 0 || rolePosition.y < getTileSize().height || rolePosition.x > getTotalSize().width || rolePosition.y > getTotalSize().height)
		return BoundRact(0, Vec2::ZERO, Ract::ZERO);
	//  ���ش˴���tile����
	Vec2 tilePosition = this->getTileCoordinateByPosition(rolePosition);
	//���Ĳ�λ�Լ�

	auto gid = getGidAt(tilePosition);
	auto tileRact = getRactByTileCoordinate(tilePosition, gid);
	auto tileProperty = getProperty(gid);

	auto boundRact = BoundRact(gid, tilePosition, tileRact);
	boundRact.setTileProperty(tileProperty);

	return boundRact;

}

Vec2 TiledMap::getTileCoordinateByPosition(Vec2 position)
{
	float x = floor(position.x / getTileSize().width); //λ��xֵ/��ͼһ��tile�Ŀ��ȼ��ɵõ�x����
	float levelHeightInPixels = getMapSize().height * getTileSize().height; //��ͼ��ʵ�ʸ߶�
	float y = floor((levelHeightInPixels - position.y) / getTileSize().height);  //��ͼ��ԭ�������Ͻǣ���cocos2d-x�ǲ�ͬ��(2dxԭ�������½�)
	return Vec2(x, y);
}

Ract TiledMap::getRactByTileCoordinate(Vec2 tileCoords, const int& gid)
{
	float levelHeightInPixels = getMapSize().height * getTileSize().height; //��ͼ��ʵ�ʸ߶�
	//�ѵ�ͼ����tileCoordsת��Ϊʵ����Ϸ�е�����
	auto origin = Vec2(tileCoords.x *  getTileSize().width, levelHeightInPixels - ((tileCoords.y + 1)* getTileSize().height));
	if (0 == gid)
		return Ract(origin.x, origin.y, getTileSize().width, getTileSize().height);

	auto inset = tileProperties[gid].inset;

	return Ract(origin.x - inset.origin.x, origin.y - inset.origin.y, getTileSize().width - inset.size.width, getTileSize().height - inset.size.height);
}
TMXLayer& TiledMap::getWalls(const std::string& layerName) const
{
	auto layer = getLayer(layerName);
	assert(layer != nullptr && "û�иò�");
	return *layer;
}

void TiledMap::setGidRange(const int & gid, const Vec2& rangeStart, const Vec2& rangeEnd)
{
	processTileRange(rangeStart, rangeEnd, [&](const Vec2& rangeTile, const short& i) {
		getWalls().setTileGID(gid, rangeTile);
	});
}

void TiledMap::setLayerGid(const std::string & layerName, const int & gid, const Vec2 & rangeStart, const Vec2 &rangeEnd)
{
	processTileRange(rangeStart, rangeEnd, [&](const Vec2& rangeTile, const short& i) {
		getWalls(layerName).setTileGID(gid, rangeTile);
	});
}

//�߽��жϷ���ios�˳�����
uint32_t TiledMap::getGidAt(Vec2  tilePos, const std::string& layerName)
{
	//	CCASSERT(pos.x < _layerSize.width && pos.y < _layerSize.height && pos.x >= 0 && pos.y >= 0, "TMXLayer: invalid position");
	if (tilePos.x > getWalls(layerName).getLayerSize().width - 1)
		tilePos.x = getWalls(layerName).getLayerSize().width - 1;
	if (tilePos.y > getWalls(layerName).getLayerSize().height - 1)
		tilePos.y = getWalls(layerName).getLayerSize().height - 1;

	if (tilePos.x <= 0)
		tilePos.x = 0;

	if (tilePos.y <= 0)
		tilePos.y = 0;

	return getWalls(layerName).getTileGIDAt(tilePos);
}

void TiledMap::removeTileRange(const Vec2 &rangeStart, const Vec2 &rangeEnd)
{
	processTileRange(rangeStart, rangeEnd, [&](const Vec2& rangeTile, const short& i) {
		getWalls().removeTileAt(rangeTile);
	});
}

MovingDirection& TiledMap::getRoleDirection(const Vec2 & rolePos)
{
	//�ŵ�����ǰ����������ƶ���λ�� ���������� 
	if (rolePos.x != lastRolePosition.x && rolePos.y == lastRolePosition.y)
		moveDirection = rolePos.x > lastRolePosition.x ? MovingDirection::toRight : MovingDirection::toLeft;

	if (rolePos.x == lastRolePosition.x && rolePos.y != lastRolePosition.y)
		moveDirection = rolePos.y < lastRolePosition.y ? MovingDirection::toBottom : MovingDirection::toTop;

	if (rolePos.x > lastRolePosition.x && rolePos.y > lastRolePosition.y)
		moveDirection = MovingDirection::toTopRight;

	if (rolePos.x < lastRolePosition.x && rolePos.y < lastRolePosition.y)
		moveDirection = MovingDirection::toBottomLeft;

	if (rolePos.x > lastRolePosition.x && rolePos.y < lastRolePosition.y)
		moveDirection = MovingDirection::toBottomRight;

	if (rolePos.x < lastRolePosition.x && rolePos.y > lastRolePosition.y)
		moveDirection = MovingDirection::toTopLeft;

	if (lastRolePosition == Vec2::ZERO || (rolePos.x == lastRolePosition.x && rolePos.y == lastRolePosition.y))
		moveDirection = MovingDirection::stayStill;

	lastRolePosition = rolePos;
	return moveDirection;
}


void TiledMap::setCameraY(const Vec2 & rolePos)
{
	float absoluteY = abs(getPositionY());
	float triggerTopY = absoluteY + limitedSize.y*0.8f; //��Ļ�߶ȵ�5��֮2
	float triggerBottomY = absoluteY + limitedSize.y*0.18f; //��Ļ�߶ȵ�10��֮2
	float limitedHeight = getTotalSize().height - ScreenSize.height;
	auto md = getRoleDirection(rolePos);

	switch (md)
	{
	case MovingDirection::toTop:
	case MovingDirection::toTopLeft:
	case MovingDirection::toTopRight:
		if (rolePos.y >= triggerTopY && absoluteY <= getTotalSize().height)
			setPositionY(getPositionY() - (rolePos.y - triggerTopY));
		if (absoluteY >= limitedHeight)
			setPositionY(-limitedHeight);
		break;
	case MovingDirection::toBottom:
	case MovingDirection::toBottomLeft:
	case MovingDirection::toBottomRight:
		if (rolePos.y <= triggerBottomY && rolePos.y > getTileSize().height * 2)
			setPositionY(getPositionY() + (triggerBottomY - rolePos.y));
		//else if (getPositionY() >  getTileSize().height)
		//{
		//	setPositionY(getPositionY() + 4.f);
		//	//Y����Ϊ0�������©������ͼƬ ���� ��
		//	if (getPositionY()>0.f)
		//		setPositionY(0.f);
		//}
		break;
	case MovingDirection::stayStill:
	case MovingDirection::toLeft:
	case MovingDirection::toRight:
		//����ǵ�ͼ��ʼλ�ã����������Ƹ߶� 1/4�� ���Զ��ƶ�����ʼλ��
		if (absoluteY < limitedSize.y / 3.5f && getPositionY() < 0)
			setPositionY(getPositionY() + 4.f);
		//Y����Ϊ0�������©������ͼƬ ���� ��
		if (getPositionY() > 0.f)
			setPositionY(0.f);
		break;
	default:
		break;
	}
}
void TiledMap::setCameraSlide(const Vec2& rolePos, const CameraView& slideStyle)
{
	if (rolePos.x <= 1 || rolePos.x >= getTotalSize().width || rolePos.y <= 1 || rolePos.y >= getTotalSize().height || lastRolePosition == Vec2::ZERO)
	{
		lastRolePosition = rolePos;
		return;
	}
	if (Vec2::ZERO == per10msDistance)
		per10msDistance = Vec2(ScreenSize.width *0.014, ScreenSize.height *0.014);
	//�ƶ���Ļ��return û���ƶ������ִ������Ĳ����Է����� �ƶ�λ�ü������
	switch (moveDirection)
	{
	case MovingDirection::toLeft:

		if (per10msSteps.x <= limitedSize.x && slideStyle != CameraView::vertical)
		{
			per10msSteps.x += per10msDistance.x;
			setPositionX(getPositionX() + per10msDistance.x);
			return;
		}
		break;
	case MovingDirection::toRight:
		if (per10msSteps.x <= limitedSize.x && slideStyle != CameraView::vertical)
		{
			per10msSteps.x += per10msDistance.x;
			setPositionX(getPositionX() - per10msDistance.x);
			return;
		}
		break;

	case MovingDirection::toTop:

		if (per10msSteps.y <= limitedSize.y && slideStyle != CameraView::horizontal)
		{
			per10msSteps.y += per10msDistance.y;
			setPositionY(getPositionY() - per10msDistance.y);
			return;
		}
		break;
	case MovingDirection::toBottom:

		if (per10msSteps.y <= limitedSize.y && slideStyle != CameraView::horizontal)
		{
			per10msSteps.y += per10msDistance.y;
			setPositionY(getPositionY() + per10msDistance.y);
			return;
		}
		break;

	default:
		break;
	}

	if (rolePos.x > lastRolePosition.x)
	{
		float xPos = rolePos.x + getPositionX();
		if (xPos > limitedSize.x)
		{
			//setPositionX(getPositionX() - ScreenSize.width); //��ɫֱ�ӵ�λ
			moveDirection = MovingDirection::toRight;
			per10msSteps.x = 0.f;
		}
	}
	else if (rolePos.x < lastRolePosition.x)
	{
		float absMapPosX = abs(getPositionX());
		if (absMapPosX > rolePos.x)
		{
			//setPositionX(getPositionX() + ScreenSize.width);//��ɫֱ�ӵ�λ
			moveDirection = MovingDirection::toLeft;
			per10msSteps.x = 0.f;
		}
	}

	//�����ͼ����Ļ��
	if (getTotalSize().height > ScreenSize.height)
	{
		if (rolePos.y > lastRolePosition.y)
		{
			float yPos = rolePos.y + getPositionY();
			if (yPos > limitedSize.y)
			{
				//setPositionX(getPositionY() - limitedHeight);
				moveDirection = MovingDirection::toTop;
				per10msSteps.y = 0.f;
			}
		}
		else if (rolePos.y < lastRolePosition.y)
		{
			float absMapPosY = abs(getPositionY());

			if (absMapPosY > rolePos.y)
			{
				//setPositionX(getPositionY() +limitedHeight);
				moveDirection = MovingDirection::toBottom;
				per10msSteps.y = 0.f;
			}
		}
	}

	lastRolePosition = rolePos;
}

void TiledMap::setCameraCenter(Role& role, const CameraView& cameraView, const bool& isAutoFocusY)
{
	const float divded = 2.f;

	if (cameraView == CameraView::horizontal || cameraView == CameraView::both)
	{
		/* �����������С����Ļ��һ�룬��ȡ��Ļ�е����꣬����ȡ���ǵ����� */
		float x = std::max(role.getPositionX(), ScreenSize.width / divded);
		/* ���X��Y������������Ͻǵļ���ֵ����ȡ����ֵ�����꣨����ֵ��ָ���õ�ͼ���� ��Ļ��ɳ��ֺڱߵļ������꣩ */
		x = std::min(x, getTotalSize().width - ScreenSize.width / divded);
		/* ������Ļ�е����Ҫ�ƶ���Ŀ�ĵ�֮��ľ��� */
		float viewPosX = ScreenSize.width / divded - x;

		//���õ�ͼ�������ƶ�
		setPositionX(viewPosX);
	}

	//���������������������
	if (cameraView == CameraView::vertical || cameraView == CameraView::both)
	{
		//�����ͼû����ô�ߣ���ֻ�����������ƶ�
		if (getTotalSize().height < ScreenSize.height * 2)
			return;
		//y�������Ž�ɫ�ƶ�
		if (isAutoFocusY)
		{
			auto y = std::max(role.getPositionY(), ScreenSize.height / divded);
			y = std::min(y, getTotalSize().height - ScreenSize.height / divded);
			auto viewPosY = ScreenSize.height / divded - y;
			setPositionY(viewPosY);

		}
		//���򣬶������ں��
		else
			setCameraY(role.getPosition());
	}
}
bool TiledMap::setCameraFrame(const bool& orientation)
{
	if (Vec2::ZERO == per10msDistance)
		per10msDistance = Vec2(0.25f, 0.25f);

	if (orientation)
	{
		float absoluteX = abs(getPositionX());

		if (absoluteX + limitedSize.x >= getTotalSize().width)
			return true;

		setPositionX(getPositionX() - per10msDistance.x);
		return false;
	}

	float absoluteY = abs(getPositionY());

	if (absoluteY + limitedSize.y >= getTotalSize().height)
		return true;

	setPositionY(getPositionY() - per10msDistance.y);
	return false;
}
void TiledMap::setCameraRepeat(Role* role, const float& mapStep, const bool& isFocusY)
{
	assert(nullptr != secondMap && "secondMap û�г�ʼ��");
	assert(getMapSize().height == getMapSize().height && "���ŵ�ͼ�߶Ȳ�һ��");

	if (isFocusY)
	{
		setCameraY(role->getPosition());
		secondMap->setCameraY(role->getPosition());
	}

	//1�������ж���Сǰ��movestep 2����ͼ���紦��ɫ�����ֵ
	auto halfStepX = mapStep / 2;

	if (abs(lastRolePositionX[role->getTag()] - role->getPositionX()) <= halfStepX)
		return;

	lastRolePositionX[role->getTag()] = role->getPositionX();

	auto absoluteRight = role->getDirection() == MovingDirection::toRight || role->getDirection() == MovingDirection::toBottomRight || role->getDirection() == MovingDirection::toTopRight;
	auto absoluteLeft = role->getDirection() == MovingDirection::toLeft || role->getDirection() == MovingDirection::toBottomLeft || role->getDirection() == MovingDirection::toTopLeft;

	log("Right:%d Left:%d Rep:%d,NotinMap:%d", absoluteRight, absoluteLeft, inSecondMap[role->getTag()], role->getParent() == nullptr);

	//float divded = ScreenSize.width / 2.f;
	if (inSecondMap.find(role->getTag()) == inSecondMap.end())
		inSecondMap[role->getTag()] = false;
	//����ڵ�һ�ŵ�ͼ

	if (!inSecondMap[role->getTag()])
	{
		auto a1 = getChildByTag(role->getTag());
		auto a2 = secondMap->getChildByTag(role->getTag());
		auto px = role->getPositionX();
		// ������
		if (absoluteRight)
		{
			//������ ����Ѿ�����ڶ��ŵ�ͼ
			if (role->getPositionX() >= getTotalSize().width)
			{
				//���������һ�ź͵ڶ��ŵ�ͼ�Ľ��紦
				inSecondMap[role->getTag()] = true;
				auto diff = role->getPositionX() - getTotalSize().width;

				secondMap->addChild(role);
				removeChild(role, false);
				role->setPositionX(-mapStep / 2);
				//�мǼ�¼�ڶ��ŵ�ͼ����һ��X����
				//lastRoleX = 0;
				//secondMap->lastRoleX = role->getPositionX();

				//auto px2 = role->getPositionX();
				//auto a1 = getChildByTag(role->getTag());
				//auto a2 = secondMap->getChildByTag(role->getTag());
				//auto px = role->getPositionX();
				return;
			}
			if (role->getTag() == ID_Player)
			{
				setPositionX(getPositionX() - mapStep);
				secondMap->setPositionX(secondMap->getPositionX() - mapStep);
			}

			isStartOff = false;

		}
		else if (absoluteLeft)
		{
			//������ ����Ѿ�����ڶ��ŵ�ͼ
			if (role->getPositionX() <= 0)
			{
				auto px = role->getPositionX();
				inSecondMap[role->getTag()] = true;
				secondMap->addChild(role);
				role->setPositionX(secondMap->getTotalSize().width + mapStep / 2);
				secondMap->lastRoleX = role->getPositionX();//�мǼ�¼�ڶ��ŵ�ͼ����һ��X����
				removeChild(role, false);
				return;
			}

			if (role->getTag() == ID_Player)
			{
				setPositionX(getPositionX() + mapStep);
				secondMap->setPositionX(secondMap->getPositionX() + mapStep);
			}

		}

	}
	else//�ڶ��ŵ�ͼ
	{
		auto px = role->getPositionX();
		auto a1 = getChildByTag(role->getTag());
		auto a2 = secondMap->getChildByTag(role->getTag());

		// ������
		if (absoluteRight)
		{
			//������ ����Ѿ������һ�ŵ�ͼ
			if (role->getPositionX() >= secondMap->getTotalSize().width)
			{

				inSecondMap[role->getTag()] = false;
				auto diff = role->getPositionX() - secondMap->getTotalSize().width;
				addChild(role);
				role->setPositionX(-mapStep / 2);
				//�мǼ�¼��һ�ŵ�ͼ����һ��X����
				//lastRoleX = role->getPositionX();
				//secondMap->lastRoleX = 0;
				secondMap->removeChild(role, false);
				return;
			}
			if (role->getTag() == ID_Player)
			{
				secondMap->setPositionX(secondMap->getPositionX() - mapStep);
				setPositionX(getPositionX() - mapStep);
			}
		}
		else if (absoluteLeft)
		{
			//������ �����һ�ŵ�ͼ
			if (role->getPositionX() <= 0)
			{
				inSecondMap[role->getTag()] = false;
				role->setPositionX(getTotalSize().width + mapStep / 2);
				addChild(role);
				//�мǼ�¼��һ�ŵ�ͼ����һ��X����
				lastRoleX = role->getPositionX();
				secondMap->removeChild(role, false);
				//return;
			}
			if (role->getTag() == ID_Player)
			{
				secondMap->setPositionX(secondMap->getPositionX() + mapStep);
				setPositionX(getPositionX() + mapStep);
			}
		}

		secondMap->lastRoleX = role->getPositionX();
	}

	/********************************
	*��ͼ���������Ļ��С������λ�û�λ
	********************************/
	if (absoluteRight)
	{
		//����һ�ŵ�ͼ�����Ҷ˺ʹ��ڵ�������غ�ʱ
		if (getPositionX() + getContentSize().width <= 0)
		{
			//����һ�ŵ�ͼ���ӵ��ڶ��ŵ�ͼ����
			//��һ�ŵ�ͼ�ĺ����� = �ڶ��ŵ�ͼ�ĺ����� + �ڶ��ŵ�ͼ�Ŀ���
			setPositionX(secondMap->getPositionX() + secondMap->getContentSize().width - mapOffsetX);
		}

		//���ڶ��ŵ�ͼ�����Ҷ˺ʹ��ڵ�������غ�ʱ
		//���ڶ��ŵ�ͼ���ӵ���һ�ŵ�ͼ����
		if (secondMap->getPositionX() + secondMap->getContentSize().width <= 0)
		{
			//���ڶ��ŵ�ͼ���ӵ���һ�ŵ�ͼ����
			//�ڶ��ŵ�ͼ�ĺ����� = ��һ�ŵ�ͼ�ĺ����� + ��һ�ŵ�ͼ�Ŀ���
			secondMap->setPositionX(getPositionX() + getContentSize().width - mapOffsetX);
		}
	}
	else if (absoluteLeft)
	{
		if (!inSecondMap[role->getTag()])
		{
			if (secondMap->getPositionX() >= ScreenSize.width)
				secondMap->setPositionX(-secondMap->getTotalSize().width + getPositionX() + mapOffsetX);
		}
		else
		{
			if (getPositionX() >= ScreenSize.width)
				setPositionX(-getTotalSize().width + secondMap->getPositionX() + mapOffsetX);
		}
	}
}

bool TiledMap::moveCameraX(const float & step, const float & distance)
{
	if (Vec2::ZERO == originPosition)
		originPosition = getPosition();

	if (abs(abs(getPositionX()) - abs(originPosition.x)) >= distance)
		return true;
	//��ͷ��
	if (abs(getPositionX()) >= (getTotalSize().width - ScreenSize.width))
		return true;
	setPositionX(getPositionX() - step);
	return false;
}

/*ƽ̨�ƶ�����*/
Vec2 TiledMap::getPositionByTileCoordinate(const Point &tileCoord)   const
{
	float factor = CC_CONTENT_SCALE_FACTOR();
	int x = (tileCoord.x * getTileSize().width) + getTileSize().width / 2;
	int y = (getMapSize().height * getTileSize().height) - (tileCoord.y * getTileSize().height) - getTileSize().height / 2;
	return Point(x / factor, y / factor);
}

Vec2 TiledMap::getPositionByTileCoordinate2(const Point & tileCoord) const
{
	float factor = CC_CONTENT_SCALE_FACTOR();
	int x = (tileCoord.x * getTileSize().width);
	int y = (getMapSize().height * getTileSize().height) - (tileCoord.y * getTileSize().height) - getTileSize().height;
	return Vec2(x / factor, y / factor);
}

void TiledMap::debugDraw()
{
	//�Ƿ������滭��ײ�߿�
	auto allowDraw = LUAH->getGlobal("AllowDraw").toValue<bool>();
	if (!allowDraw)
		return;
	drawCollision->clear();
	//�������role�����򿴲���
	drawCollision->setLocalZOrder(Z_Draw);
	for (auto role : getChildren())
	{
		auto r = dynamic_cast<Role*>(role);

		if (nullptr != r)
		{
			drawCollision->drawRact(Vec2(r->getCollisionBound(r->getInsetObject()).getMinX(), r->getCollisionBound(r->getInsetObject()).getMinY()), Vec2(r->getCollisionBound(r->getInsetObject()).getMaxX(), r->getCollisionBound(r->getInsetObject()).getMaxY()), Color4F::BLUE);
			//sensor
			r->getSensor().render(drawCollision);
		}
	}

	triggerSystem.render(drawCollision);
}

void TiledMap::loadScript()
{
	Trigger* trigger;
	TriggerSystem::triggerNext = 1;
	unsigned short type = 0;
	LUAH->processTable(Luat_Triggers, [&](LuaIntf::LuaRef ref) {

		type = ref.get(Luaf_Type, 0);

		if (type == 0)
			return;

		switch (type)
		{
		case 1:
			CCASSERT(ref.has(Luaf_Ract), "������rect");
			trigger = new TriggerRact(ref.get(Luaf_Ract, Ract(200, 200, 33, 44)));
			break;
		case 2:
			CCASSERT(ref.has(Luaf_RangeStart), "������RangeStart");
			CCASSERT(ref.has(Luaf_RangeStart), "������RangeStart");
			trigger = new TriggerLine(ref.get(Luaf_RangeStart, Vec2(100, 100)), ref.get(Luaf_RangeEnd, Vec2(100, 200)));
			break;
		case 3:
			CCASSERT(ref.has(Luaf_Ract), "������rect");
			trigger = new TriggerCircle(ref.get(Luaf_Ract, Ract(200, 200, 33, 44)));
			break;
		}

		if (ref.has(Luaf_IsActive))
			trigger->setActivation(ref.get(Luaf_IsActive, true));

		if (ref.has(Luaf_Name))
			trigger->setName(ref.get(Luaf_Name).toValue<std::string>());
		else
			trigger->setName("trigger");
		triggerSystem.addTrigger(trigger);
		//��thisָ��
		auto tag = trigger->getTag();
		ref.set(Luaf_Tag, trigger->getTag());
		ref.set(Luaf_CppRef, trigger);
		LUAH->flush();
	}, false);
}

void TiledMap::collapse(LuaRef ref)
{
	//���lua����һ�ε�collapse���ڣ�������ú����ʹ�÷�������rangeԽ��,�����������ֵ��������
	auto rangeStart = ref.get(Luaf_RangeStart, Vec2::ZERO);
	auto rangeEnd = ref.get(Luaf_RangeEnd, Vec2::ZERO);
	//auto step = ref.get(Luaf_MoveStep, Vec2::ZERO);
	//auto direction = ref.get(Luaf_Direction, MovingDirection::toBottom);
	//auto range = ref.get(Luaf_Range, Vec2(ScreenSize.width, ScreenSize.height));


	RObject* collisionTile = nullptr;

	processTileRange(rangeStart, rangeEnd, [&](const Vec2& rangeTile, const short& i) {
		auto tile = getWalls().getTileAt(rangeTile);
		if (nullptr == tile)
		{
			log("[Collapse]tile Collapse incorrect settings or no such tile coordinates");
			return;
		}

		//bool isOutOfRange = rangeTile.x >= getMapSize().width || rangeTile.y >= getMapSize().height;
		//auto mapSize = getMapSize();
		bool isInRange = rangeTile.x < getMapSize().width && rangeTile.y < getMapSize().height;

		if (!isInRange)
		{
			log("[TiledMap::collapse]out of range");
			return;
		}
		auto tileFrame = tile->getSpriteFrame();

		if (nullptr == tileFrame)
		{
			log("[TiledMap::collapse]mapTag tile frame");
			return;
		}
		collisionTile = Role::createWithFrame<RObject>(tileFrame);;
		//collisionTile->setName(StringUtils::format("collapse%d", collisionTile->getTag()));
		collisionTile->setPosition(getPositionByTileCoordinate(rangeTile));
		collisionTile->setOrigin(collisionTile->getPosition());
		//�Ƴ���ǰtiledmap�е�sprite
		getWalls().removeTileAt(rangeTile);
		//���ݵ�ͼid����
		ROLE_MANAGER->registerRole(collisionTile);
		addChild(collisionTile);

		ref.set(Luaf_CppRef, collisionTile);
		//ע�ᵽ�ű�
		LUAH->callback("collapse", "collapse", ref);
	});

}

void TiledMap::update(Role& role)
{
	if (triggerSystem.getCount() == 0)
		return;
}

//һ��Ҫ��stage������Role��Ϻ����ִ�д˷���������tag�Ḳ��ԭ�е�roleԪ���޷�ִ��
void TiledMap::registerKnocks(const LuaRef & ref)
{
	auto rangeStart = ref.get(Luaf_RangeStart, Vec2::ZERO);
	auto rangeEnd = ref.get(Luaf_RangeEnd, Vec2::ZERO);

	if (rangeStart == Vec2::ZERO)
		return;
	if (!ref.has(Luaf_Knocks))
	{
		log("[TiledMap::registerKnocks]please set value field of knocks actions");
		return;
	}
	RObject* knockTile = nullptr;
	processTileRange(rangeStart, rangeEnd, [&](const Vec2& rangeTile, const short& i) {
		auto tile = getWalls().getTileAt(rangeTile);
		if (nullptr == tile)
		{
			log("[Collapse]tile Collapse incorrect settings or no such tile coordinates");
			return;
		}

		//bool isOutOfRange = rangeTile.x >= getMapSize().width || rangeTile.y >= getMapSize().height;
		//auto mapSize = getMapSize();
		bool isInRange = rangeTile.x < getMapSize().width && rangeTile.y < getMapSize().height;

		if (!isInRange)
		{
			log("[TiledMap::registerKnocks]out of range");
			return;
		}
		auto tileFrame = tile->getSpriteFrame();

		if (nullptr == tileFrame)
		{
			log("[TiledMap::registerKnocks]mapTag tile frame null");
			return;
		}
		knockTile = Role::createWithFrame<RObject>(tileFrame);;
		knockTile->setName(StringUtils::format("knock%d", knockTile->getTag()));
		knockTile->registerKnocks(ref.get(Luaf_Knocks));
		knockTile->setPosition(getPositionByTileCoordinate(rangeTile));
		//������ʵoriginmap ��λcorruptλ��
		knockTile->setOriginMap(rangeTile);

		//�Ƴ���ǰtiledmap�е�sprite
		getWalls().removeTileAt(rangeTile);
		//���ݵ�ͼid����
		addChild(knockTile);
		ROLE_MANAGER->registerRole(knockTile);
		//ע�ᵽ�ű�
		LUAH->registerRole("map", "knock", knockTile);
	});
}

SpriteFrame * TiledMap::getFrameWithTile(const Vec2& pos)
{
	auto tilePos = Vec2(getTileSize().width * pos.x, getTileSize().height * pos.y);
	auto tileFrame = SpriteFrame::createWithTexture(getWalls().getTexture(), Ract(tilePos, getTileSize()));

	return tileFrame;
}

//tiledmap editor ������ϵ���ѡ�񣬶����Ǵ��µ���
void TiledMap::processTileRange(const Vec2& rangeStart, const Vec2& rangeEnd, std::function<void(const Vec2& range, const short& iter)> each)
{
	CCASSERT(rangeEnd.x >= rangeStart.x, "tiledmap editor ������ϵ���ѡ�񣬶����Ǵ��µ���");
	CCASSERT(rangeEnd.y >= rangeStart.y, "tiledmap editor ������ϵ���ѡ�񣬶����Ǵ��µ���");
	auto xTileCount = rangeEnd.x - rangeStart.x + 1;
	auto yTileCount = rangeEnd.y - rangeStart.y + 1;

	unsigned short i = 0;
	unsigned short x = 0;
	unsigned short y = 0;

	auto tileRangeCount = xTileCount * yTileCount;
	CCASSERT(tileRangeCount > 0, "tiledmap editor ������ϵ���ѡ�񣬶����Ǵ��µ���");
	while (i < tileRangeCount)//��Ƭ��������
	{
		if (i > 0 && i % static_cast<short>(xTileCount) == 0) //ÿ�� y++,x����
		{
			y++;
			x = 0;
		}

		each(Vec2(rangeStart.x + x++, rangeStart.y + y), i);
		i++;
	}
}

RangeType TiledMap::getRange(const Vec2 & a, const Vec2 & b)
{
	auto self = getCenterTile(a).tilePosition;
	auto opponent = getCenterTile(b).tilePosition;

	auto x = self.x - opponent.x;
	auto y = self.y - opponent.y;
	const auto range = 3;
	if (x <= range && y <= range)
		return RangeType::rangeEndangered;
	else if (x <= range * 2 && y <= range*2)
		return RangeType::rangeThreatened;
	else if (x <= range * 3 && y <= range*3)
		return RangeType::rangeSensed;
	else if (x <= range*4 && y <= range*4)
		return RangeType::rangeOutOfSight;

	return RangeType::rangeOut;
}