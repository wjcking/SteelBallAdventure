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
	//ǹ����
	unsigned short	type = 0;
	//����������
	ProjectType projectType = ProjectType::object;
	//�����ﵯ�������ж��� 
	short  slugsLeft = 10;
	short  slugsCount = 10;
	//�ܴ����ٵ���
	//unsigned short  maxRounds = 5.f;
	//������
	float rate = 1.f;
	//ʲôʱ������¸�������
	float nextAvailable;

	inline bool isReadyForNextShot() { return Clock::getTickFloat() > nextAvailable ? true : false; }
	inline void updateNextAvailable() { nextAvailable = Clock::getTickFloat() +  rate; }
	//���������ʱ������
	std::string sound;
	//�ӵ�frame֡
	std::string projectFrame;

	//����ProjectTile
	LuaIntf::LuaRef refProject;
	LuaRef refWeapon;

	bool isFire = false;
public:

	Weapon(Role& o) : owner(&o)
	{
		nextAvailable = Clock::getTickFloat();
	}
	//���lua�ص����ж�ǹе����
	inline LuaRef& getRef() { return refWeapon; }
	virtual ~Weapon() {}
	virtual void  shootAt(const Vec2& pos = Vec2::ZERO) { isFire = true; };
	virtual void attack() {};
	virtual void  render() {};
	//Ŀǰֻ��һ�����У��ӵ������
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
	//������������,һ��Ҫ��dofile֮����������Ҫ����preload�¼�����
	void registerWeapon(const LuaIntf::LuaRef&);

	inline bool& isFiring() { return isFire; }
	inline void setFireOn() { isFire = true; }
	inline void setFireOff() { isFire = false; }
	//ж���ӵ�
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
		//��ֵԽ���ٶ�Խ��,versa vice
		rate = RateOfFire;
		nextAvailable = Clock::getTickFloat();
	}

	void  render() override {}
	void  shootAt(const Vec2& pos = Vec2::ZERO) override;

};

#endif