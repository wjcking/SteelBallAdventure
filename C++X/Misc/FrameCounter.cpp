#include "framecounter.h"
#include "cocos2d.h"

float Clock::tik = 1;
int Clock::iTick = 0;

float&  Clock::getTickFloat()
{
	return tik;
}
int&  Clock::getTickInteger()
{
	return  iTick;
}
//ֻ�������update����ִֻ��һ��
float&  Clock::addDeltaCount()
{
	tik += 0.01f;// DtaIntvao;
	iTick += 1;

	if (tik > 2147483647 - 1)
		tik = 0;

	if (iTick > 2147483647 - 1)
		iTick = 0;

	return tik;
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
