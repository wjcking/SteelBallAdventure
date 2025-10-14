#include "Property.h"

Property::Property()
{

}


bool Property::inAir(const bool isClear)
{
	bool isInAir = !onSlopeBottom && !onGround && !onObject;
		//如果落在地上
	//if (!isInAir)
	//	isJumping = false;
	if (isClear)
	{
		onSlopeBottom = false;
		onGround = false;
		onObject = false;
	}

	return isInAir;
}
