#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include <functional>
#include <unordered_map> 
#include "../Utils/GameScriptor.h"
using namespace std;
class Role;
class Player;
class Npc;
class RObject;
class ProjectTile; 
#define ROLE_MANAGER RoleManager::getInstance()
#define ROLE_PLAYER RoleManager::getPlayerRefer()  

class RoleManager
{
private:
	unordered_map<unsigned int, Role*> entityMap;

	RoleManager();
	RoleManager(const RoleManager&);
	RoleManager& operator=(const RoleManager&);
	static Player* player;
	//从lua添加
	static void registerLuaRole(LuaIntf::LuaRef);
public:
	static RoleManager* getInstance();
	//static Player* getPlayer();
	static Player& getPlayerRefer();
	void process(function<void(Role&)> roleAction);
	static void updateRole(const int&, const bool&);
	void  registerRole(Role* NewEntity);
	//在角色销毁后，会出现找不到；如果对话框属于该角色的话返回null不进行断言
	Role* getRoleByTag(const int&, bool allowAssert = true) const;
	template<class T>
	inline T* getRole(const int& tag)
	{
		return dynamic_cast<T*>(getRoleByTag(tag, false));
	}
	void removeRole(Role* pEntity);
	void removeRole(const unsigned int& tag) { entityMap.erase(tag); };
	void release();
	//载入角色 以及角色需要的帧
	void loadScript();
	//1.运行加载的时候设置 2.发射物设置
	static void setProperty(LuaIntf::LuaRef ref, Role* role);

	//调用
	static void appendRole(LuaIntf::LuaRef);
};
#endif