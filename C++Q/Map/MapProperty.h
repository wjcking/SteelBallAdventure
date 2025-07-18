#pragma once
#include "cocos2d.h"
#include <unordered_map>
using namespace cocos2d;

const unsigned short   TilePropertyNumber = 1500;
static const  char PSlopeDegree[25] = "SlopeDegree";
static const  char PSlopeTowards[25] = "SlopeTowards";
static const  char PSlopeGravity[25] = "SlopeGravity";
static const  char PRoleOffsetY[25] = "RoleOffsetY";
static const  char PAllowThrough[25] = "AllowThrough";
static const  char PIsOneWay[25] = "IsOneWay";
static const  char PInitSlopeY[25] = "InitSlopeY";
static const  char PIsSlope[10] = "IsSlope";
static const  char PIsHarmful[10] = "IsHarmful";

static const  char PInsetX[10] = "InsetX";
static const  char PInsetY[10] = "InsetY";
static const  char PInsetWidth[12] = "InsetWidth";
static const  char PInsetHeight[12] = "InsetHeight";

#define TileProp TileProperties::getInstance()

enum SlopeTowards : short
{
	faceNothing = -1,
	faceLeft = 0,
	faceRight = 1
};
enum SlopeDegree :unsigned short
{
	degree45 = 45,
	degree22_5 = 22
};
//全部公开
struct TilePropertyInfo
{
	int gid = 0;
	bool isSlope;
	//倒刺
	bool isHarmful = false;
	unsigned short slopeDegree;
	unsigned short slopeTowards;
	float  slopeGravity;
	float  roleOffsetY;
	//是否允许通过
	bool allowThrough;
	bool isOneWay;
	//22度角的第二个瓦片，Y从第一个瓦片的最高Y开始
	bool initSlopeY; 
	Rect inset;
	TilePropertyInfo();

};

//用来存储包装盒rect以及gid
class BoundRect :public Rect
{
private:
	TilePropertyInfo tileProperty;
public:
	int gid;
	Vec2 tilePosition;
	std::string tileName;
	inline void setTileProperty(const TilePropertyInfo& tp)
	{
		tileProperty = tp;
		tileProperty.gid = gid;//砖坯属性,用于lua判断
	}
	inline const TilePropertyInfo& getProperty() const
	{
		//return TileProperties::getProperty(gid);
		return tileProperty;
	}
	BoundRect(int id, Vec2, Rect bound);
	BoundRect(int id, Vec2&, Size&);
	//BoundRect& operator= (const BoundRect& other);
	BoundRect();
};
