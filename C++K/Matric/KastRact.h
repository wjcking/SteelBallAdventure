#pragma once 
#include "cocos2d.h"
using namespace cocos2d;

struct KastRact
{
	KastRact() {};
	KastRact(float&, float&);
	float min;
	float max;
	bool isOverlap(const KastRact&);
};

class RactAround
{
	private:
	Vec2 vertex[4];
	Vec2* getAxies();
	KastRact getKastRact(const Vec2& axie);

	public:
	RactAround() {};
	~RactAround() {};
	Vec2 getVertex(const int& index) const;
	void setVertex(const int& index, const float& x, const float& y);
	void setVertex(const int& index, const Vec2& v);
	
	bool isCollidWith(RactAround&);
};