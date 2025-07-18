#include "MapManager.h"
#include "TiledMap.h"
#include "../Utils/Constant.h"
#include "../Utils/GameScriptor.h"
/*静态类初始化*/
//   this class is a singleton
unsigned  short MapManager::MapTag = ID_Map;

MapManager* MapManager::getInstance()
{
	static MapManager instance;
	return &instance;
}

TiledMap* MapManager::getCurrentMap()
{
	return MAP_MANAGER->getMapByTagID(MapTag);
}
/*根据id和分类获得单或多个*/
TiledMap* MapManager::getMapByTagID(int id)const
{
	auto ent = entityMap.find(id);
	assert((ent != entityMap.end()) && "<MapManager::getMapByTagID>: invalid ID");
	return ent->second;
}

void MapManager::removeMap(const int& tagID)
{
	entityMap.erase(entityMap.find(tagID));
}

void MapManager::loadScript()
{

	auto atlas = LUAH->getGlobal( Luat_Atlas);
	assert(atlas.isTable() && "请设置atlas");
	assert(atlas.len() > 0 && "至少要有一个地图");
	//清空
	entityMap.clear();
	//重置标识
	TiledMap::nextTag = 1;
	Vec2 position;
	std::string file;
	for (auto iter = atlas.begin(); iter != atlas.end(); ++iter)
	{
		assert(iter.value().has(Luaf_File) && "没有file属性");
		 file = iter.value()[Luaf_File].value<std::string>();
		assert(FileUtils::getInstance()->isFileExist(file) && "没有发现地图文件");
		if (file.empty())
			continue;
		auto map = TiledMap::create(file);
		//设置地图视角
		map->setViewType(iter.value().get(Luaf_View, MapView::horizontal));
		if (iter.value().has("repeatMap"))
		{
			map->createRepeat(iter.value()["repeatMap"].value<std::string>());
		}
		//背景加载到地图中 imageLayer不支持
		if (iter.value().has(Luaf_Image))
		{
			auto bgPath = iter.value()[Luaf_Image].value<std::string>();
			assert(FileUtils::getInstance()->isFileExist(bgPath) && "没有发现背景文件");
			auto bg = Sprite::create(bgPath);
			bg->setPosition(bg->getContentSize().width / 2.f, bg->getContentSize().height / 2.f);
			//背景必须在地图下面，否则被遮
			map->addChild(bg, map->getLocalZOrder() - 1);

		}
		MAP_MANAGER->registerMap(map);
		 // position = iter.value().get(Luaf_Pos, Vec2::ZERO);
		 //
		 //map->setPtc(position,Vec2::ZERO);
		//载入脚本 sensorSystem
		map->loadScript();
		if (iter.value().has(Luaf_Knocks))
			map->registerKnocks(iter.value().get(Luaf_Knocks));
		//把地图指针送入lua全局变量 
		iter.value().set(Luaf_CppRef, map);
		LUAH->flush();
	}
}

 

void MapManager::registerMap(TiledMap* map)
{
	entityMap[map->getTag()] = map;
}
