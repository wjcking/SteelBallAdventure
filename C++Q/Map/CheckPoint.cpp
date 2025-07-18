#include "CheckPoint.h"
#include "../Utils/Constant.h"
#include "../Utils/GameScriptor.h"
#include "TiledMap.h"
#include "MapManager.h"
#include "../Utils/ResourceHelper.h"
int Checkpoint::life = 10;
int Checkpoint::trialTimes = 0;

short Checkpoint::id = 0;
short Checkpoint::mapID = 1;
int Checkpoint::gid = -1;
Vec2 Checkpoint::position = Vec2::ZERO;
Vec2 Checkpoint::rangeStart = Vec2::ZERO;
Vec2 Checkpoint::rangeEnd = Vec2::ZERO;
std::unordered_map<short, short> Checkpoint::objectMap = std::unordered_map<short, short>();

Checkpoint::Checkpoint()
{
	objectMap.reserve(50);
}

Checkpoint::~Checkpoint()
{
}

void Checkpoint::setPosition(const Vec2 & pos)
{
	//mapID = mapid;
	position = pos;
	id++;
}
void Checkpoint::setPtc(const Vec2 & pos)
{
	position = pos;
	id++;
}
void Checkpoint::setObject(const short & objectIndex, const short & count)
{
	objectMap[objectIndex] = count;
}
void Checkpoint::addObject(const short & objectIndex)
{
	objectMap[objectIndex]++;
}

void Checkpoint::updateTrial()
{
	if (life > 0)
		--life;
	trialTimes++;
	//记录一共死了多少次
	Resh::addNumber(User_Trial, 1);
};
short Checkpoint::getObject(const short & objectIndex)
{
	if (objectMap.find(objectIndex) != objectMap.end())
		return objectMap[objectIndex];

	return 0;
}

void Checkpoint::setTile()
{
	//如果gid = -1 什么都不做
	if (gid < 0)
		return;
	if (gid == 0)
		MAP_WITHTAG(mapID)->removeTileRange(rangeStart, rangeEnd);
	else if (gid > 0)
		MAP_WITHTAG(mapID)->setGidRange(gid, rangeStart, rangeEnd);
}

void Checkpoint::reset(const bool& isClearTrial  )
{
	id = 0;
	position = Vec2::ZERO;
	life = 10;

	gid = -1;
	if (isClearTrial)
		trialTimes = 0;
	objectMap.clear();
}

