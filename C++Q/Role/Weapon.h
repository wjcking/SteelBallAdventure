#ifndef WEAPON_BASE_H
#define WEAPON_BASE_H

#include "cocos2d.h"

#include "../Utils/Constant.h"
#include "../../Common/Misc/FrameCounter.h"
#include "../Utils/GameScriptor.h"
using namespace cocos2d;

class  Role;
class Weapon
{
protected:
	Role* owner;
	//枪类型
	unsigned short	type = 0;
	//发射物类型
	ProjectType projectType = ProjectType::object;
	//发射物弹夹里面有多少 
	short  slugsLeft = 10;
	short  slugsCount = 10;
	//能带多少弹夹
	//unsigned short  maxRounds = 5.f;
	//发射率
	float rate = 1.f;
	//什么时候可以下个发射物
	float nextAvailable;

	inline bool isReadyForNextShot() { return Clock::getTickFloat() > nextAvailable ? true : false; }
	inline void updateNextAvailable() { nextAvailable = Clock::getTickFloat() +  rate; }
	//发射物，发射时的声音
	std::string sound;
	//子弹frame帧
	std::string projectFrame;

	//传给ProjectTile
	LuaIntf::LuaRef refProject;
	LuaRef refWeapon;

	bool isFire = false;
public:

	Weapon(Role& o) : owner(&o)
	{
		nextAvailable = Clock::getTickFloat();
	}
	//获得lua回掉，判断枪械类型
	inline LuaRef& getRef() { return refWeapon; }
	virtual ~Weapon() {}
	virtual void  shootAt(const Vec2& pos = Vec2::ZERO) { isFire = true; };
	virtual void attack() {};
	virtual void  render() {};
	//目前只有一个弹夹，子弹最大数
	inline int getRoundsRemaining()const { return slugsLeft; }
	inline void decrementNumRounds()
	{
		if (slugsLeft > 0)
			--slugsLeft;
	}
	//inline void incrementRounds(int num) { slugsLeft += num; }
	inline void reload(int num)
	{
		if (slugsLeft == 0)
			slugsLeft = num <= slugsCount ? num : slugsCount;
	}
	inline short& getSlugLeft() { return this->slugsLeft; }
	inline unsigned int getType()const { return type; }
	//设置武器参数,一定要在dofile之后允许，不要放在preload事件里面
	void registerWeapon(const LuaIntf::LuaRef&);

	inline bool& isFiring() { return isFire; }
	inline void setFireOn() { isFire = true; }
	inline void setFireOff() { isFire = false; }
	//卸载子弹
	inline void unload() 
	{
		refWeapon = nullptr;
		projectFrame = "";
	}
};

class Gun : public Weapon
{
private:
	const unsigned int  TypeOfGun = 0;
	const unsigned int  DefaultNumRounds = 1;
	const unsigned int  MaxRoundsCarried = 5;

	const float  RateOfFire = 115.f;
	const float  bulletSpeed = 3.f;
public:
	Gun(Role& o) : Weapon(o)
	{
		//数值越大速度越快,versa vice
		rate = RateOfFire;
		nextAvailable = Clock::getTickFloat();
	}

	void  render() override {}
	void  shootAt(const Vec2& pos = Vec2::ZERO) override;

};

#endif