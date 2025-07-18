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
	secondMap = TiledMap::create(tmxFile); //将第二张地图的位置设置到第二张地图的后面
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
	////如果在第一张地图
	////if (!inSecondMap)
	////{
	//	//向右走 如果已经进入第二张地图
	//	if (role->getPositionX() >= getTotalSize().width)
	//	{
	//		auto px = role->getPositionX();
	//		//用来计算第一张和第二张地图的交界处
	//		inSecondMap = true;
	//		auto diff = role->getPositionX() - getTotalSize().width;
	//		secondMap->addChild(role);
	//		role->setPositionX(-role->getMoveStep().x / 2);
	//		//切记记录第二张地图的上一次X坐标
	//		lastRoleX = 0;
	//		secondMap->lastRoleX = role->getPositionX();
	//		removeChild(role, false);
	//		return true;
	//	}
	////}
	////else
	////{
	//	//向右走 如果已经进入第一张地图
	//	if (role->getPositionX() >= secondMap->getTotalSize().width)
	//	{
	//		inSecondMap = false;
	//		auto diff = role->getPositionX() - secondMap->getTotalSize().width;
	//		addChild(role);
	//		//切记记录第一张地图的上一次X坐标
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
/***
5(4)  2(1) 6(5)
3(2)       4(3)
7(6)  1(0) 8(7)
**/
BoundRect* TiledMap::getBoundTiles(Role& role, const std::string& layerName)
{
	auto position = role.getPosition();

	if (inSecondMap[role.getTag()])
		return secondMap->getBoundTiles(role, layerName);
	//  返回此处的tile坐标
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

	//下=0
	//休眠时回来，出现0xcccccccc
	auto boundRects = new BoundRect[TileBoundNumber];
	Vec2 tilePos = Vec2(tilePosition.x, tilePosition.y + 1);
	int gid = gid = getGidAt(tilePos);
	Rect tileRect = this->getRectByTileCoordinate(tilePos, gid);  //包围盒

	auto tileProperty = getProperty(gid);

	boundRects[BoundPosition::bottom] = BoundRect(gid, tilePos, tileRect);
	boundRects[BoundPosition::bottom].setTileProperty(tileProperty);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);

		if (nullptr != movingTile)
			boundRects[BoundPosition::bottom].tileName = movingTile->getName();
	}

	//上=1
	//临时判断上面Y会出现-1的情况
	tilePos = Vec2(tilePosition.x, tilePosition.y <= 0 ? 0 : (tilePosition.y - 1));
	//gid = layer->getTileGIDAt(tilePos);
	gid = getGidAt(tilePos);
	tileRect = this->getRectByTileCoordinate(tilePos, gid);

	tileProperty = getProperty(gid);
	boundRects[BoundPosition::top] = BoundRect(gid, tilePos, tileRect);
	boundRects[BoundPosition::top].setTileProperty(tileProperty);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRects[BoundPosition::top].tileName = movingTile->getName();
	}
	//左=2
	tilePos = Vec2(tilePosition.x - 1, tilePosition.y);
	gid = getGidAt(tilePos);
	tileRect = this->getRectByTileCoordinate(tilePos, gid);
	tileProperty = getProperty(gid);

	boundRects[BoundPosition::left] = BoundRect(gid, tilePos, tileRect);
	boundRects[BoundPosition::left].setTileProperty(tileProperty);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRects[BoundPosition::left].tileName = movingTile->getName();
	}

	//右=3
	tilePos = Vec2(tilePosition.x + 1, tilePosition.y);

	//gid = layer->getTileGIDAt(tilePos);
	//gid = tilePos.x == getMapSize().width ? 0 : layer->getTileGIDAt(tilePos); //到地图最右边，gid却是前一个x的tile ？？
	gid = getGidAt(tilePos);
	tileRect = getRectByTileCoordinate(tilePos, gid);
	tileProperty = getProperty(gid);

	boundRects[BoundPosition::right] = BoundRect(gid, tilePos, tileRect);
	boundRects[BoundPosition::right].setTileProperty(tileProperty);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRects[BoundPosition::right].tileName = movingTile->getName();
	}

	//左上=4
	tilePos = Vec2(tilePosition.x - 1, tilePosition.y <= 0 ? 0 : (tilePosition.y - 1));
	//gid = layer->getTileGIDAt(tilePos);
	gid = getGidAt(tilePos);
	tileRect = getRectByTileCoordinate(tilePos, gid);
	tileProperty = getProperty(gid);
	boundRects[BoundPosition::topLeft] = BoundRect(gid, tilePos, tileRect);
	boundRects[BoundPosition::topLeft].setTileProperty(tileProperty);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRects[BoundPosition::topLeft].tileName = movingTile->getName();
	}

	//右上=5
	tilePos = Vec2(tilePosition.x + 1, tilePosition.y <= 0 ? 0 : (tilePosition.y - 1));
	//	gid = layer->getTileGIDAt(tilePos);
	gid = getGidAt(tilePos);
	tileRect = getRectByTileCoordinate(tilePos, gid);
	tileProperty = getProperty(gid);
	boundRects[BoundPosition::topRight] = BoundRect(gid, tilePos, tileRect);
	boundRects[BoundPosition::topRight].setTileProperty(tileProperty);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRects[BoundPosition::topRight].tileName = movingTile->getName();
	}
	//左下=6
	tilePos = Vec2(tilePosition.x - 1, tilePosition.y + 1);
	//	gid = layer->getTileGIDAt(tilePos);
	gid = getGidAt(tilePos);
	tileRect = getRectByTileCoordinate(tilePos, gid);
	tileProperty = getProperty(gid);

	boundRects[BoundPosition::bottomLeft] = BoundRect(gid, tilePos, tileRect);
	boundRects[BoundPosition::bottomLeft].setTileProperty(tileProperty);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRects[BoundPosition::bottomLeft].tileName = movingTile->getName();
	}

	//右下=7
	tilePos = Vec2(tilePosition.x + 1, tilePosition.y + 1);
	//gid = tilePos.x == getMapSize().width ? 0 : layer->getTileGIDAt(tilePos); //到地图最右边，gid却是前一个x的tile ？？
	gid = getGidAt(tilePos);
	tileRect = getRectByTileCoordinate(tilePos, gid);
	tileProperty = getProperty(gid);
	boundRects[BoundPosition::bottomRight] = BoundRect(gid, tilePos, tileRect);
	boundRects[BoundPosition::bottomRight].setTileProperty(tileProperty);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRects[BoundPosition::bottomRight].tileName = movingTile->getName();
	}
	//中心部位自己
	tilePos = Vec2(tilePosition.x, tilePosition.y);
	//gid = layer->getTileGIDAt(tilePos);
	gid = getGidAt(tilePos);
	tileRect = getRectByTileCoordinate(tilePos, gid);
	tileProperty = getProperty(gid);

	boundRects[BoundPosition::centerSelf] = BoundRect(gid, tilePos, tileRect);
	boundRects[BoundPosition::centerSelf].setTileProperty(tileProperty);
	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRects[BoundPosition::centerSelf].tileName = movingTile->getName();
	}

	return boundRects;
}

BoundRect TiledMap::getCenterTile(const Vec2& rolePosition)
{
	auto layer = getLayer(LayerWalls);
	assert(layer != nullptr && "图层不能为空");

	if (rolePosition.y <= 0 || rolePosition.y < getTileSize().height || rolePosition.x > getTotalSize().width || rolePosition.y > getTotalSize().height)
		return BoundRect(0, Vec2::ZERO, Rect::ZERO);
	//  返回此处的tile坐标
	Vec2 tilePosition = this->getTileCoordinateByPosition(rolePosition);
	//中心部位自己

	auto gid = getGidAt(tilePosition);
	auto tileRect = getRectByTileCoordinate(tilePosition, gid);
	auto tileProperty = getProperty(gid);

	auto boundRect = BoundRect(gid, tilePosition, tileRect);
	boundRect.setTileProperty(tileProperty);

	return boundRect;

}

Vec2 TiledMap::getTileCoordinateByPosition(Vec2 position)
{
	float x = floor(position.x / getTileSize().width); //位置x值/地图一块tile的宽度即可得到x坐标
	float levelHeightInPixels = getMapSize().height * getTileSize().height; //地图的实际高度
	float y = floor((levelHeightInPixels - position.y) / getTileSize().height);  //地图的原点在左上角，与cocos2d-x是不同的(2dx原点在左下角)
	return Vec2(x, y);
}
Rect TiledMap::getRectByTileCoordinate(Vec2 tileCoords, const int& gid)
{
	float levelHeightInPixels = getMapSize().height * getTileSize().height; //地图的实际高度
	//把地图坐标tileCoords转化为实际游戏中的坐标
	auto origin = Vec2(tileCoords.x *  getTileSize().width, levelHeightInPixels - ((tileCoords.y + 1)* getTileSize().height));
	if (0 == gid)
		return Rect(origin.x, origin.y, getTileSize().width, getTileSize().height);

	auto inset = tileProperties[gid].inset;

	return Rect(origin.x - inset.origin.x, origin.y - inset.origin.y, getTileSize().width - inset.size.width, getTileSize().height - inset.size.height);
}
TMXLayer& TiledMap::getWalls(const std::string& layerName) const
{
	auto layer = getLayer(layerName);
	assert(layer != nullptr && "没有该层");
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
//边界判断否则ios端出问题
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
	//放到重力前面计算主角移动的位置 ：上下左右 
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
	float triggerTopY = absoluteY + limitedSize.y*0.8f; //屏幕高度的5分之2
	float triggerBottomY = absoluteY + limitedSize.y*0.18f; //屏幕高度的10分之2
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
		//	//Y必须为0，否则会漏出其他图片 背景 等
		//	if (getPositionY()>0.f)
		//		setPositionY(0.f);
		//}
		break;
	case MovingDirection::stayStill:
	case MovingDirection::toLeft:
	case MovingDirection::toRight:
		//如果是地图起始位置，并且在限制高度 1/4内 则自动移动到起始位置
		if (absoluteY < limitedSize.y / 3.5f && getPositionY() < 0)
			setPositionY(getPositionY() + 4.f);
		//Y必须为0，否则会漏出其他图片 背景 等
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
	//移动屏幕，return 没有移动完毕则不执行下面的操作以防主角 移动位置计算错误
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
			//setPositionX(getPositionX() - ScreenSize.width); //角色直接到位
			moveDirection = MovingDirection::toRight;
			per10msSteps.x = 0.f;
		}
	}
	else if (rolePos.x < lastRolePosition.x)
	{
		float absMapPosX = abs(getPositionX());
		if (absMapPosX > rolePos.x)
		{
			//setPositionX(getPositionX() + ScreenSize.width);//角色直接到位
			moveDirection = MovingDirection::toLeft;
			per10msSteps.x = 0.f;
		}
	}

	//如果地图比屏幕高
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
		/* 如果主角坐标小于屏幕的一半，则取屏幕中点坐标，否则取主角的坐标 */
		float x = std::max(role.getPositionX(), ScreenSize.width / divded);
		/* 如果X、Y的坐标大于右上角的极限值，则取极限值的坐标（极限值是指不让地图超出 屏幕造成出现黑边的极限坐标） */
		x = std::min(x, getTotalSize().width - ScreenSize.width / divded);
		/* 计算屏幕中点和所要移动的目的点之间的距离 */
		float viewPosX = ScreenSize.width / divded - x;

		//设置地图横坐标移动
		setPositionX(viewPosX);
	}

	//如果不允许处理纵坐标则
	if (cameraView == CameraView::vertical || cameraView == CameraView::both)
	{
		//如果地图没有那么高，则只能运行左右移动
		if (getTotalSize().height < ScreenSize.height * 2)
			return;
		//y坐标随着角色移动
		if (isAutoFocusY)
		{
			auto y = std::max(role.getPositionY(), ScreenSize.height / divded);
			y = std::min(y, getTotalSize().height - ScreenSize.height / divded);
			auto viewPosY = ScreenSize.height / divded - y;
			setPositionY(viewPosY);

		}
		//否则，多适用于横版
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
	assert(nullptr != secondMap && "secondMap 没有初始化");
	assert(getMapSize().height == getMapSize().height && "两张地图高度不一致");

	if (isFocusY)
	{
		setCameraY(role->getPosition());
		secondMap->setCameraY(role->getPosition());
	}

	//1）用来判断最小前进movestep 2）地图交界处角色距离差值
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
	//如果在第一张地图

	if (!inSecondMap[role->getTag()])
	{
		auto a1 = getChildByTag(role->getTag());
		auto a2 = secondMap->getChildByTag(role->getTag());
		auto px = role->getPositionX();
		// 向右走
		if (absoluteRight)
		{
			//向右走 如果已经进入第二张地图
			if (role->getPositionX() >= getTotalSize().width)
			{
				//用来计算第一张和第二张地图的交界处
				inSecondMap[role->getTag()] = true;
				auto diff = role->getPositionX() - getTotalSize().width;

				secondMap->addChild(role);
				removeChild(role, false);
				role->setPositionX(-mapStep / 2);
				//切记记录第二张地图的上一次X坐标
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
			//向左走 如果已经进入第二张地图
			if (role->getPositionX() <= 0)
			{
				auto px = role->getPositionX();
				inSecondMap[role->getTag()] = true;
				secondMap->addChild(role);
				role->setPositionX(secondMap->getTotalSize().width + mapStep / 2);
				secondMap->lastRoleX = role->getPositionX();//切记记录第二张地图的上一次X坐标
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
	else//第二张地图
	{
		auto px = role->getPositionX();
		auto a1 = getChildByTag(role->getTag());
		auto a2 = secondMap->getChildByTag(role->getTag());

		// 向右走
		if (absoluteRight)
		{
			//向右走 如果已经进入第一张地图
			if (role->getPositionX() >= secondMap->getTotalSize().width)
			{

				inSecondMap[role->getTag()] = false;
				auto diff = role->getPositionX() - secondMap->getTotalSize().width;
				addChild(role);
				role->setPositionX(-mapStep / 2);
				//切记记录第一张地图的上一次X坐标
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
			//向左走 进入第一张地图
			if (role->getPositionX() <= 0)
			{
				inSecondMap[role->getTag()] = false;
				role->setPositionX(getTotalSize().width + mapStep / 2);
				addChild(role);
				//切记记录第一张地图的上一次X坐标
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
	*地图交替根据屏幕大小，主角位置换位
	********************************/
	if (absoluteRight)
	{
		//当第一张地图的最右端和窗口的最左端重合时
		if (getPositionX() + getContentSize().width <= 0)
		{
			//将第一张地图添加到第二张地图后面
			//第一张地图的横坐标 = 第二张地图的横坐标 + 第二张地图的宽度
			setPositionX(secondMap->getPositionX() + secondMap->getContentSize().width - mapOffsetX);
		}

		//当第二张地图的最右端和窗口的最左端重合时
		//将第二张地图添加到第一张地图后面
		if (secondMap->getPositionX() + secondMap->getContentSize().width <= 0)
		{
			//将第二张地图添加到第一张地图后面
			//第二张地图的横坐标 = 第一张地图的横坐标 + 第一张地图的宽度
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
	//到头了
	if (abs(getPositionX()) >= (getTotalSize().width - ScreenSize.width))
		return true;
	setPositionX(getPositionX() - step);
	return false;
}


/*平台移动基类*/
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
	//是否允许绘画碰撞边框
	auto allowDraw = LUAH->getGlobal("AllowDraw").toValue<bool>();
	if (!allowDraw)
		return;
	drawCollision->clear();
	//必须高于role，否则看不见
	drawCollision->setLocalZOrder(Z_Draw);
	for (auto role : getChildren())
	{
		auto r = dynamic_cast<Role*>(role);

		if (nullptr != r)
		{
			drawCollision->drawRect(Vec2(r->getCollisionBound(r->getInsetObject()).getMinX(), r->getCollisionBound(r->getInsetObject()).getMinY()), Vec2(r->getCollisionBound(r->getInsetObject()).getMaxX(), r->getCollisionBound(r->getInsetObject()).getMaxY()), Color4F::BLUE);
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
			CCASSERT(ref.has(Luaf_Rect), "请设置rect");
			trigger = new TriggerRect(ref.get(Luaf_Rect, Rect(200, 200, 33, 44)));
			break;
		case 2:
			CCASSERT(ref.has(Luaf_RangeStart), "请设置RangeStart");
			CCASSERT(ref.has(Luaf_RangeStart), "请设置RangeStart");
			trigger = new TriggerLine(ref.get(Luaf_RangeStart, Vec2(100, 100)), ref.get(Luaf_RangeEnd, Vec2(100, 200)));
			break;
		case 3:
			CCASSERT(ref.has(Luaf_Rect), "请设置rect");
			trigger = new TriggerCircle(ref.get(Luaf_Rect, Rect(200, 200, 33, 44)));
			break;
		}

		if (ref.has(Luaf_IsActive))
			trigger->setActivation(ref.get(Luaf_IsActive, true));

		if (ref.has(Luaf_Name))
			trigger->setName(ref.get(Luaf_Name).toValue<std::string>());
		else
			trigger->setName("trigger");
		triggerSystem.addTrigger(trigger);
		//把this指针
		auto tag = trigger->getTag();
		ref.set(Luaf_Tag, trigger->getTag());
		ref.set(Luaf_CppRef, trigger);
		LUAH->flush();
	}, false);
}

void TiledMap::collapse(LuaRef ref)
{
	//如果lua中上一次的collapse存在，务必重置后才能使用否则会出现range越界,定义变量不赋值则可以清空
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
		//移除当前tiledmap中的sprite
		getWalls().removeTileAt(rangeTile);
		//根据地图id添加
		ROLE_MANAGER->registerRole(collisionTile);
		addChild(collisionTile);

		ref.set(Luaf_CppRef, collisionTile);
		//注册到脚本
		LUAH->callback("collapse", "collapse", ref);
	});

}

void TiledMap::update(Role& role)
{
	if (triggerSystem.getCount() == 0)
		return;

}
//一定要先stage加载完Role完毕后才能执行此方法，否则tag会覆盖原有的role元素无法执行
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
		//设置其实originmap 定位corrupt位置
		knockTile->setOriginMap(rangeTile);

		//移除当前tiledmap中的sprite
		getWalls().removeTileAt(rangeTile);
		//根据地图id添加
		addChild(knockTile);
		ROLE_MANAGER->registerRole(knockTile);
		//注册到脚本
		LUAH->registerRole("map", "knock", knockTile);
	});
}

SpriteFrame * TiledMap::getFrameWithTile(const Vec2 & pos)
{
	auto tilePos = Vec2(getTileSize().width * pos.x, getTileSize().height * pos.y);
	auto tileFrame = SpriteFrame::createWithTexture(getWalls().getTexture(), Rect(tilePos, getTileSize()));

	return tileFrame;
}

//tiledmap editor 坐标从上到下选择，而不是从下到上
void TiledMap::processTileRange(const Vec2& rangeStart, const Vec2& rangeEnd, std::function<void(const Vec2& range, const short& iter)> each)
{
	CCASSERT(rangeEnd.x >= rangeStart.x, "tiledmap editor 坐标从上到下选择，而不是从下到上");
	CCASSERT(rangeEnd.y >= rangeStart.y, "tiledmap editor 坐标从上到下选择，而不是从下到上");
	auto xTileCount = rangeEnd.x - rangeStart.x + 1;
	auto yTileCount = rangeEnd.y - rangeStart.y + 1;

	unsigned short i = 0;
	unsigned short x = 0;
	unsigned short y = 0;

	auto tileRangeCount = xTileCount * yTileCount;
	CCASSERT(tileRangeCount > 0, "tiledmap editor 坐标从上到下选择，而不是从下到上");
	while (i < tileRangeCount)//瓦片行列总数
	{
		if (i > 0 && i % static_cast<short>(xTileCount) == 0) //每列 y++,x重置
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