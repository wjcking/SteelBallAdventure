#include "ProjectTile.h"
#include "RoleManager.h"

#include "../Map/MapManager.h"
#include "../Map/TiledMap.h"


ProjectTile::ProjectTile() : Role()
{
	isGravityOn = false;
	//自动预先设置不碰撞瓦片
	onCheckTile = false;
	type = RoleType::projectTile;
	//isSolid = projectType == ProjectType::footboard;
	setLocalZOrder(Z_ROLE);
	//临时 
	//需要预先设置isDispose，否则遗留下来的会默认为true
	//LUAH->setDisposal(getTag(), false);
}
ProjectTile::~ProjectTile()
{
}

void ProjectTile::setOwner(Role& owner)
{
	this->owner = &owner;
	towards = owner.getFacedDirection();
	switch (towards)
	{
	case MovingDirection::toLeft:
		originPosition = Vec2(owner.getPositionX() - offset.x, owner.getPositionY());
		break;
	case MovingDirection::toRight:
		originPosition = Vec2(owner.getPositionX() + offset.x, owner.getPositionY());
		break;
	case MovingDirection::toTop:
		setFlippedY(false);
		originPosition = Vec2(owner.getPositionX(), owner.getPositionY() + offset.y);
		break;
	case MovingDirection::toBottom:
		setFlippedY(false);
		originPosition = Vec2(owner.getPositionX(), owner.getPositionY() - offset.y);
		break;
	}
	setPosition(originPosition);

}

bool ProjectTile::isDead()
{
	Vec2 distance = getPosition() - originPosition;
	if (abs(distance.x) > ScreenSize.width || abs(distance.y) > ScreenSize.height)
		return true;

	return false;
}
void ProjectTile::update()
{
	//父类更新位置
	//踩在footboard上面，必须更新
	Role::update();
}
void ProjectTile::setProject(const LuaRef&  ref)
{
	//auto projectLua ref.get(Luaf_Project)
	if (ref.has(Luaf_Offset))
		offset = ref.get(Luaf_Offset).toValue<Vec2>();
	if (ref.has(Luaf_AllowThrough))
		canThough = ref.get(Luaf_AllowThrough, false);
	//1.运行加载的时候设置 2.发射物设置
	RoleManager::setProperty(ref, this);
}
/*
* 发射物体
*/
SlugObject::SlugObject() : ProjectTile()
{
}

void SlugObject::setProject(const LuaRef & ref)
{
	 ProjectTile::setProject(ref);
}

bool SlugObject::isDead()
{
	Vec2 distance = getPosition() - originPosition;
	if (abs(distance.x) > range.x || abs(distance.y) > range.y)
		return true;

	return false;
}
void SlugObject::update()
{ 
	switch (towards)
	{
	case MovingDirection::toLeft:
		setBackward();
		break;
	case MovingDirection::toRight:
		setForward();
		break;
	case MovingDirection::toTop:
		setUpward();
		break;
	case MovingDirection::toBottom:
		setDownward();
		break;
	}
	ProjectTile::update();
	//移动到
	//gotoDesirePosition();
}
/*
* 踏板
*/
Footboard::Footboard() : ProjectTile(), delayDisappeared(3.f)
{
	isSolid = true;
	projectType = ProjectType::footboard;
}

void Footboard::setProject(const LuaRef&  ref)
{
	ProjectTile::setProject(ref);
	if (ref.has(Luaf_Delay))
		delayDisappeared = ref.get(Luaf_Delay).toValue<float>();
}
bool Footboard::isDead()
{
	if (delayDisappeared.isTimeUp())
		return true;

	return false;
}
void Footboard::update()
{
	//必须加上，主角才能踩到上面
	ProjectTile::update();
}
/*
* 绝对命中
*/
Deadshot::Deadshot() : ProjectTile()
{
	projectType = ProjectType::deadshot;
}

bool Deadshot::isDead()
{
	if (nullptr == target)
		return true;

	return ProjectTile::isDead();
}
void Deadshot::setProject(const LuaRef&  ref)
{
	ProjectTile::setProject(ref);
	if (ref.has(Luaf_Target))
	{
		auto tag = ref.get(Luaf_Target).toValue<int>();
		target = ROLE_MANAGER->getRoleByTag(tag, false);
	}
}
void Deadshot::update()
{
	ProjectTile::update();

	if (nullptr == target)
		return;

	bDestiny = locate(target->getPosition());
}

 
