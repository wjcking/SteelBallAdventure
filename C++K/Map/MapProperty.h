#pragma once
#include "cocos2d.h"
#include <unordered_map>
using namespace cocos2d;

const unsigned short  TilePropertyNumber = 1500;
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
//ȫ������
struct TilePropertyInfo
{
	int gid = 0;
	bool isSlope;
	//����
	bool isHarmful = false;
	unsigned short slopeDegree;
	unsigned short slopeTowards;
	float  slopeGravity;
	float  roleOffsetY;
	//�Ƿ�����ͨ��
	bool allowThrough;
	bool isOneWay;
	//22�Ƚǵĵڶ�����Ƭ��Y�ӵ�һ����Ƭ�����Y��ʼ
	bool initSlopeY; 
	Ract inset;
	TilePropertyInfo();

};

//�����洢��װ��rect�Լ�gid
class BoundRact :public Ract
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
		tileProperty.gid = gid;//ש������,����lua�ж�
	}
	inline const TilePropertyInfo& getProperty() const
	{
		//return TileProperties::getProperty(gid);
		return tileProperty;
	}
	BoundRact(int id, Vec2, Ract bound);
	BoundRact(int id, Vec2&, Size&);
	//BoundRact& operator= (const BoundRact& other);
	BoundRact();
};
