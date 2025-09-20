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
	//��lua����
	static void registerLuaRole(LuaIntf::LuaRef);
public:
	static RoleManager* getInstance();
	//static Player* getPlayer();
	static Player& getPlayerRefer();
	void process(function<void(Role&)> roleAction);
	static void updateRole(const int&, const bool&);
	void  registerRole(Role* NewEntity);
	//�ڽ�ɫ���ٺ󣬻�����Ҳ���������Ի������ڸý�ɫ�Ļ�����null�����ж���
	Role* getRoleByTag(const int&, bool allowAssert = true) const;
	template<class T>
	inline T* getRole(const int& tag)
	{
		return dynamic_cast<T*>(getRoleByTag(tag, false));
	}
	void removeRole(Role* pEntity);
	void removeRole(const unsigned int& tag) { entityMap.erase(tag); };
	void release();
	//�����ɫ �Լ���ɫ��Ҫ��֡
	void loadScript();
	//1.���м��ص�ʱ������ 2.����������
	static void setProperty(LuaIntf::LuaRef ref, Role* role);

	//����
	static void appendRole(LuaIntf::LuaRef);
};
#endif