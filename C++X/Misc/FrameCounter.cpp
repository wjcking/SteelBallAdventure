#include "framecounter.h"
#include "cocos2d.h"

float Clock::fTick = 1;
int Clock::iTick = 0;

float&  Clock::getTickFloat()
{
	return fTick;
}
int&  Clock::getTickInteger()
{
	return  iTick;
}
//只在最外层update并且只执行一次
float&  Clock::addDeltaCount()
{
	fTick += 0.01f;// DeltaInterval;
	iTick += 1;

	if (fTick > 2147483647 - 1)
		fTick = 0;

	if (iTick > 2147483647 - 1)
		iTick = 0;

	return fTick;
}

std::unordered_map<short, DelayPhase> DelayCollection::delay = std::unordered_map<short, DelayPhase>();
void DelayCollection::reset(const short& id)
{
	delay[id].reset();
}
void DelayCollection::clear()
{
	delay.clear();
}
bool DelayCollection::isTimeUp(const short& id, const float& ds)
{
	if (delay.size() == 0)
		delay.reserve(2000);

	if (ds != delay[id].delaySecond)
		delay[id].delaySecond = ds;

	return delay[id].isTimeUp();
}
