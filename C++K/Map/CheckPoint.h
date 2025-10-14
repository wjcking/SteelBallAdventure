#pragma once
#include "cocos2d.h"
using namespace cocos2d;

//静态类
class Checkpoint
{
private:
	static std::unordered_map<short, short> objectMap;
public:
	Checkpoint();
	~Checkpoint();
	static int life;
	static int trialTimes;
	static short mapID;
	//在那个阶段 stage 1 2 3
	static short id;
	//如果=0，则是要去除的 否则替换 =-1则什么都不做
	static  int gid;
	static Vec2 rangeStart;
	static Vec2 rangeEnd;
	//【注意】目前是地图tile的位置
	static Vec2 position;
	//设置角色位置并且id++
	static void setPosition(const Vec2& pos);
	static void setPtc(const Vec2& pos);
	static Vec2& getPosition() { return position; }
	//对象个数
	static void setObject(const short&, const short&);
	static void addObject(const short&);
	static void updateTrial();
	
	inline static void addLife() 
	{
		if (life < 9)
			++life; 
	};
	static short getObject(const short&);
	//tile =-1则什么都不做 如果=0，则是要去除的 否则替换
	static void setTile();
	//重置，id=0，就从新开始
	static void reset(const bool& isClearTrial = true);
	static bool isChecked() { return id > 0; }
};