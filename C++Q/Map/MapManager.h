#ifndef MAPMANAGER_H
#define MAPMANAGER_H
#pragma warning (disable:4786)

#include <unordered_map>
#include <cassert>
#include <string> 

class TiledMap;
#define MAP_MANAGER MapManager::getInstance()
#define MAP_STAGE MapManager::getCurrentMap()
#define MAP_WITHTAG(tag) MapManager::getInstance()->getMapByTagID(tag)

class MapManager
{
	private:
	std::unordered_map<int, TiledMap*> entityMap; 
	MapManager(){
		entityMap.reserve(10);
	}
	MapManager(const MapManager&);
	MapManager& operator=(const MapManager&);

	public:
	static unsigned  short MapTag;
	static MapManager* getInstance();
	static TiledMap* getCurrentMap();
	void  registerMap(TiledMap* NewEntity);
	TiledMap* getMapByTagID(int id)const;
	void removeMap(const int& tagID); 
	void loadScript();
 
	void clear() {
		entityMap.clear();
	};
};

#endif