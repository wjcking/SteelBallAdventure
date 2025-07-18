#ifndef BLINKSYSTEM_H
#define  BLINKSYSTEM_H
#include "cocos2d.h"  
#include "../Utils/GameScriptor.h"
using namespace cocos2d;
#define BlinkInstance BlinkSystem::getInstance()
enum BlinkStatus
{
	blinkDoneHidden,
	blinkShown,
	blinkWaiting,
	blinkInit
};

struct BlinkInfo
{
	//记录每一个要闪烁的Robject tag，当每组结束后统一重置
	std::vector<int>  tags;
	short groupID;
	bool isTriggered;
	bool isPhaseDone;
	unsigned short finishedCount;
	BlinkInfo() : finishedCount(0), isPhaseDone(false), isTriggered(true), groupID(1)
	{

	}

	inline void reset()
	{
		finishedCount = 0;
	}
};
class BlinkSystem
{
private:
	static std::unordered_map<unsigned short, BlinkInfo> blinkGroup;
public:
	static void registerBlinks(LuaIntf::LuaRef);
	//robject blinkGroup调用
	static void addFinished(const unsigned short& gid)
	{
		blinkGroup[gid].finishedCount++;
	}
	//robject blinkGroup调用
	static bool isGroupDone(const unsigned short& gid)
	{
		return blinkGroup[gid].tags.size() == blinkGroup[gid].finishedCount;
	}
	static void resetGroup(const unsigned short& gid);
	static BlinkInfo& getBlinkInfo(const unsigned short& gid)
	{
		return blinkGroup[gid];
	}
};
#endif