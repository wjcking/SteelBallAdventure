#pragma once
#include "cocos2d.h"
#include "Constant.h"
#include "RectTools.h"
#include "../../Common/lua/include/lua.hpp" 
#include "../../Common/Lua-Intf/LuaIntf.h" 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#pragma comment(lib, "lua5.3.4.lib")
#endif

using namespace LuaIntf;
using namespace cocos2d;

struct Luaf
{
	Luaf() {};
	Luaf(LuaRef);
	float delay = 0.f;
	std::string name = "";
	//有的加扩展名有的不加 .png 或其他
	std::string file = "";
	Vec2 position = Vec2::ZERO;
	Vec2 offset = Vec2::ZERO;
	float duration;
	int zorder = 0;
};
//获得字体信息
struct FontInfo
{
	FontInfo() {};
	explicit FontInfo(LuaRef);
	float size = 16.f;
	bool isBold = false;
	cocos2d::Color3B color = cocos2d::Color3B::BLACK;
};

struct CurtainInfo : Luaf
{
	CurtainInfo() {};
	CurtainInfo(LuaRef);
	CurtainType type = CurtainType::none;
	//黑幕 或左边或上边
	std::string image = "background/curtain.png";
	//黑幕 或左边或上边
	std::string image1 = "background/curtainleft.png";
	//右边或下边
	std::string image2 = "background/curtainright.png";

	FontInfo font;
};
