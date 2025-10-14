#ifndef  WEAPONSYSTEM
#define WEAPONSYSTEM

#include "cocos2d.h"
#include "Weapon.h"
using namespace cocos2d;
class Role;
class WeaponSystem
{
private:

	Role* owner;

	std::unordered_map< unsigned short, Weapon*>        weaponMap;

	//a pointer to the weapon the bot is currently holding
	unsigned short    currentWeaponType;

	//this is the minimum amount of time a bot needs to see an opponent before
	//it can react to it. This variable is used to prevent a bot shooting at
	//an opponent the instant it becomes visible.
	//float            reactionTime;

	//each time the current weapon is fired a certain amount of random noise is
	//added to the the angle of the shot. This prevents the bots from hitting
	//their opponents 100% of the time. The lower this value the more accurate
	//a bot's aim will be. Recommended values are between 0 and 0.2 (the value
	//represents the max deviation in radians that can be added to each shot).
	//float            aimAccuracy;

	//the amount of time a bot will continue aiming at the position of the target
	//even if the target disappears from view.
	//float            aimPersistance;

public:

	WeaponSystem(Role& owner);
	~WeaponSystem();

	//sets up the weapon map with just one weapon: the blaster
	void Initialize();
	inline	void  changeWeapon(const unsigned short& wid = 0) { this->currentWeaponType = wid; }

	//this will add a weapon of the specified type to the bot's inventory. 
	//If the bot already has a weapon of this type only the ammo is added. 
	//(called by the weapon giver-sensorSystem to give a bot a weapon)
	void addWeapon(const unsigned short&  weapon_type);
	//shoots the current weapon at the given position
	void shootAt(const Vec2& pos = Vec2::ZERO);

	//returns a pointer to the current weapon
	Weapon* getCurrentWeapon();

	//returns a pointer to the specified weapon type (if in inventory, null if 
	//not)
	Weapon& getWeaponFromInventory(const unsigned short&   weapon_type);

	//returns the amount of ammo remaining for the specified weapon
	int getAmmoRemainingForWeapon(const unsigned short& weapon_type);

	//float ReactionTime()const{ return reactionTime; }

	void RenderCurrentWeapon();

	inline void clear() {
		for (auto w : weaponMap)
			delete w.second;
	}
};

#endif