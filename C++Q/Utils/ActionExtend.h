#ifndef __ACTIONEXTEND_H__
#define __ACTIONEXTEND_H__

#include "cocos2d.h"
#include "GameScriptor.h"
USING_NS_CC;

static const unsigned short   DRWhite = 0;
static const unsigned short   DRGray = 1;
//static const unsigned short   DRWhite = 0;
static const unsigned short   DROrange = 3;
static const unsigned short   DRRed = 4;
class ActionExtend
{
private:
	SpriteFrame* frame;
public:
	ActionExtend() :frame(nullptr)
	{

	}
	~ActionExtend()
	{
	}

	/*创建一个抛物线动作
	参数：
	t 时间
	startPoint		开始点
	endPoint		结束点
	height			高度（影响抛物线的高度）
	angle			角度（贝塞尔曲线两个控制点与y轴的夹角，直接影响精灵的抛出角度）
	*/
	static EaseInOut* parabolaTo(const float& t, const Vec2& startPoint, const Vec2& endPoint, const float& height = 0, const float& angle = 60);
	static void corrupt(const Vec2&, const unsigned short& = DRWhite);
	static void toast(const LuaIntf::LuaRef& ref);
	static void explode(const LuaIntf::LuaRef& ref);

};
#endif 
