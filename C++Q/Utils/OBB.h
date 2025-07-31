#pragma once 
#include "cocos2d.h"
using namespace cocos2d;

class ObbRect
{
	private:
	struct Projection
	{
		Projection() {};
		float min;
		float max;
		bool isOverlap(const Projection& proj);
		Projection(float min, float max);
	};
	Vec2 vertex[4];
	Vec2* getAxies();
	Projection getProjection(const Vec2& axie);

	public:
	ObbRect() {};
	~ObbRect() {};
	Vec2 getVertex(const int& index) const;
	void setVertex(const int& index, const float& x, const float& y);
	void setVertex(const int& index, const Vec2& v);
	//gcc error ����ֵΪ�ֲ��������ݹ���������
	bool isCollidWithOBB(ObbRect obb);

};