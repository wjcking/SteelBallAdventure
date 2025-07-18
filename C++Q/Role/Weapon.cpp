#include "Weapon.h"
#include "ProjectTile.h"
#include "RoleManager.h"
#include "../Map/MapManager.h"
#include "../Map/TiledMap.h"
#include "SimpleAudioEngine.h"

void Gun::shootAt(const Vec2& pos)
{
	if (owner->getBodyStatus() == BodyStatus::hurting)
	{
		CCLOG("[Gun:shootAt]hurting me, unable to shoot");
		return;
	}
	if (projectFrame == "")
	{
		CCLOG("[Gun:shootAt]no frame name, please set weapon");
		return;
	}
	auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(projectFrame);
	if (nullptr == frame)
	{
		CCLOG("[Gun:shootAt]nullptr frame, please set weapon");
		return;
	}
	//如果slugLeft <0 则是无限
	bool canShoot = false;

	if (slugsLeft < 0)
		canShoot = isReadyForNextShot();
	else
		canShoot = isReadyForNextShot() && (getRoundsRemaining() > 0);
	//设置开火状态
	//用来刷帧使用
	isFire = true;

	if (canShoot)
	{
		ProjectTile* projectTile = nullptr;

		switch (projectType)
		{
		case ProjectType::deadshot:
			projectTile = Role::createWithFrame<Deadshot>(frame);
			break;
		case ProjectType::footboard:
			projectTile = Role::createWithFrame<Footboard>(frame);
			break;
		case ProjectType::trace:
			projectTile = Role::createWithFrame<Trace>(frame);
			//dynamic_cast<Trace*>(projectTile)->setAim(pos);
			break;
		default:
			projectTile = Role::createWithFrame<SlugObject>(frame);
			break;
		}
		//1.添加到管理器里面才有tag,否则会lua找不到 tag=-1
		ROLE_MANAGER->registerRole(projectTile);
		projectTile->getMap()->addChild(projectTile);
		projectTile->setProjectType(projectType);
		projectTile->setProject(refProject);
		//2.设置谁发射的
		projectTile->setOwner(*owner);
		//3.注册this指针到Role表
		// 注意有的子弹没有target，直接获取可能无法转换 downcast error lua
		LUAH->registerRole("gun", "shoot", projectTile);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(sound.c_str(), false);
		decrementNumRounds();		 
	}
	//没子弹的声音
	if (getRoundsRemaining() == 0)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/outofammo.wav");
	if (isReadyForNextShot())
		updateNextAvailable();
}


void Weapon::registerWeapon(const LuaRef& ref)
{
	if (!ref.isTable())
		return;
	refWeapon = ref;

	if (ref.has(Luaf_Frame))
		projectFrame = ref.get(Luaf_Frame).toValue<std::string>();
	if (ref.has(Luaf_Count))
	{
		slugsLeft = ref.get(Luaf_Count).toValue<short>();
		slugsCount = slugsLeft;
	}


	//if (ref.has(Luaf_MaxRounds))
	//	maxRounds = ref.get(Luaf_MaxRounds).toValue<unsigned short>();
	if (ref.has(Luaf_Sound))
		sound = ref.get(Luaf_Sound).toValue<std::string>();
	if (ref.has(Luaf_Rate))
		rate = ref.get(Luaf_Rate).toValue<float>();

	if (ref.has(Luaf_Project))
	{
		refProject = ref.get(Luaf_Project);

		if (refProject.has(Luaf_Type))
			projectType = refProject.get(Luaf_Type).toValue<ProjectType>();
	}
}