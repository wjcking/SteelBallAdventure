#include "WeaponSystem.h"
#include "Weapon.h"

WeaponSystem::WeaponSystem(Role& o) :owner(&o)
{
	Initialize();
}

WeaponSystem::~WeaponSystem()
{

}

void WeaponSystem::Initialize()
{
	currentWeaponType = 0;
	weaponMap.clear();
	weaponMap[currentWeaponType] = new Pistol(*owner);
}


void  WeaponSystem::addWeapon(const unsigned short& weapon_type)
{

}

Weapon& WeaponSystem::getWeaponFromInventory(const unsigned short&  weapon_type)
{
	return  *weaponMap[weapon_type];
}

int WeaponSystem::getAmmoRemainingForWeapon(const unsigned short&  weapon_type)
{
	if (weaponMap[weapon_type])
	{
		return weaponMap[weapon_type]->getRoundsRemaining();
	}

	return 0;
}
Weapon* WeaponSystem::getCurrentWeapon()
{
	return weaponMap.find(currentWeaponType) != weaponMap.end() ? weaponMap[currentWeaponType] : nullptr;
}
void WeaponSystem::shootAt(const Vec2& pos)
{
	getCurrentWeapon()->shootAt(pos);
}

void WeaponSystem::RenderCurrentWeapon()
{
	getCurrentWeapon()->render();
}
