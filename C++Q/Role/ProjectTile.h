#pragma once
#include "Role.h"

/*
*发射子弹类，游戏发布到2,3代的时候在进行扩展
*/
class ProjectTile : public Role
{
protected:
	bool bDestiny = false;
	Role* owner;
	Role* target;
	Vec2 offset = Vec2(30, 30);
	//子弹类型 0= 普通自动 1=旋梯（多长时间后消失设置timeOfDisappear）
	ProjectType projectType = ProjectType::object;
	MovingDirection towards;
	Vec2 range = Vec2(ScreenSize.width, ScreenSize.height);
	bool canThough = false;
public:
	ProjectTile();
	~ProjectTile();
	//尤其是角色朝向决定了子弹向哪里移动
	void setOwner(Role&);
	inline bool& isDestiny() { return bDestiny; }
	inline Role& getOwner() { return *owner; }
	inline Role& getTarget() { return *target; }
	inline void setTarget(Role& target) { this->target = &target; };

	//在lua端调用，如果内部调用的话发出的子弹会全部消除，跟tag有关
	virtual  bool isDead() override;
	virtual void setProject(const LuaIntf::LuaRef&);
	virtual inline ProjectType& getProjectType() { return projectType; }
	inline void setProjectType(const ProjectType& projectType) { this->projectType = projectType; }
	void update() override;
	inline bool& allowThrough() { return canThough; }
};

class SlugObject : public ProjectTile
{
private:
	//bool allowRelocate = false;
public:
	SlugObject();
	void setProject(const LuaRef&) override;
	bool isDead() override;
	void update() override;
};
class Footboard :public ProjectTile
{
private:
	DelayPhase delayDisappeared;
public:
	Footboard();
	bool isDead() override;
	void update() override;
	void setProject(const LuaRef&) override;
};
class Deadshot :public ProjectTile
{
public:
	Deadshot();

	void setProject(const LuaRef&) override;
	bool isDead() override;
	void update() override;
};

class Trace :public ProjectTile
{
private:
	Vec2 target = Vec2::ZERO;
public:
	inline Trace() { projectType = ProjectType::trace; };
	inline void setAim(const Vec2& val) { target = val; };
	inline void setProject(const LuaRef& ref)override { ProjectTile::setProject(ref); };
	inline bool isDead() override { return ProjectTile::isDead(); }
	inline void update() override { bDestiny = locate(target); }
};