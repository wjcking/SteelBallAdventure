#include "MapManager.h"
#include "TiledMap.h"
#include "../Utils/Constant.h"
#include "../Utils/GameScriptor.h"
/*��̬���ʼ��*/
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
/*����id�ͷ����õ�����*/
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
	assert(atlas.isTable() && "������atlas");
	assert(atlas.len() > 0 && "����Ҫ��һ����ͼ");
	//���
	entityMap.clear();
	//���ñ�ʶ
	TiledMap::nextTag = 1;
	Vec2 position;
	std::string file;
	for (auto iter = atlas.begin(); iter != atlas.end(); ++iter)
	{
		assert(iter.value().has(Luaf_File) && "û��file����");
		 file = iter.value()[Luaf_File].value<std::string>();
		assert(FileUtils::getInstance()->isFileExist(file) && "û�з��ֵ�ͼ�ļ�");
		if (file.empty())	continue;
		auto map = TiledMap::create(file);
		//���õ�ͼ�ӽ�
		map->setViewType(iter.value().get(Luaf_View, MapView::horizontal));
		if (iter.value().has("repeatMap"))
		{
			map->createRepeat(iter.value()["repeatMap"].value<std::string>());
		}
		//�������ص���ͼ�� imageLayer��֧��
		if (iter.value().has(Luaf_Image))
		{
			auto bgPath = iter.value()[Luaf_Image].value<std::string>();
			assert(FileUtils::getInstance()->isFileExist(bgPath) && "û�з��ֱ����ļ�");
			auto bg = Sprite::create(bgPath);
			bg->setPosition(bg->getContentSize().width / 2.f, bg->getContentSize().height / 2.f);
			//���������ڵ�ͼ���棬������
			map->addChild(bg, map->getLocalZOrder() - 1);

		}
		MAP_MANAGER->registerMap(map);
		 // position = iter.value().get(Luaf_Pos, Vec2::ZERO);
		 //
		 //map->setPtc(position,Vec2::ZERO);
		//����ű� sensorSystem
		map->loadScript();
		if (iter.value().has(Luaf_Knocks))
			map->registerKnocks(iter.value().get(Luaf_Knocks));
		//�ѵ�ͼָ������luaȫ�ֱ��� 
		iter.value().set(Luaf_CppRef, map);
		LUAH->flush();
	}
}
