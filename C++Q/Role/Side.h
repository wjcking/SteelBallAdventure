#pragma once
#include "cocos2d.h"
#include <string>
#include "../Utils/RectTools.h"
 
using namespace cocos2d;

enum OppositePopup
{
	popNothing,
	popMoney,
	popDrug,
};
struct Side
{
	//如果只允许一次，则判断是否推过
	short KnockMax = 12;
	//计算推了多少次
	short knockedTimes =0;
	//移动多少坐标量
	Vec2 pushSteps = Vec2(1.5f, 1.5f);
	//是否造成伤害
	bool isHarmful= false;
	//碰撞后是否显示刺
	bool isShowThorn = true;
	//是否允许推箱子
	bool allowPush= false;
	//允许碰撞，如果不允许角色直接过去
	bool allowCollision= true;
	//是否已经碰撞
	bool isCollided= false;
	//敲完后对面出现什么东西
	OppositePopup oppositePopup = OppositePopup::popNothing; 
	Side() {};
	//是否允许一次性碰撞,主要是上下
	inline bool getAllowKnock()
	{
		//1 如果允许碰撞则
		if (!allowCollision)
			return false;
		//2 碰撞次数
		if ( knockedTimes >= KnockMax)
			return false;
		 
		return true;
	}
};
