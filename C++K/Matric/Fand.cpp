#include "Fand.h"
#include "Constant.h"
using namespace std;

Fand::Fand()
{
}


Fand::~Fand()
{
}

RangeType Fand::getRange(const Fand& a, const Fand& b)
{
	float distance = a.origin.getDistance(b.origin);
	float threatened = a.size.width + b.size.height;

	if (distance <= threatened / 2)
		return RangeType::rangeEndangered;
	else if (distance <= threatened)
		return RangeType::rangeThreatened;
	else if (distance <= threatened * 2)
		return RangeType::rangeSensed;
	else if (distance <= ScreenWidth)
		return RangeType::rangeOutOfSight;

	return RangeType::rangeOut;
}

Fand Fand::FandInset(Fand& Fand, const float& dx, const float& dy)
{
	Fand.origin.x += dx;
	Fand.size.width -= dx * 2;
	Fand.origin.y -= dy;    //��Сʱy��Ӧ�����£�IOS������ϵ��2d-x��һ����yԭ���������½Ƕ������Ͻ�
	Fand.size.height -= dy * 2;
	return Fand;
}

Fand Fand::FandOffset(Fand &Fand, const float& dx, const float& dy)
{
	Fand.origin.x += dx;
	Fand.origin.y += dy;
	return Fand;
}

const CollisionDiFandion Fand::getCollsionDiFandion(const Fand &FandA, const Fand &FandB)
{
	return getCollsionDiFandion(FandA.origin.x, FandA.origin.y, FandA.size.width, FandA.size.height,
		FandB.origin.x, FandB.origin.y, FandB.size.width, FandB.size.height);
}
const CollisionDiFandion Fand::getCollsionDiFandion(const int& x1, const int& y1, const int& w1, const int& h1, const int& x2, const int& y2, const int& w2, const int& h2)
{
	//������1 λ�ھ���2 �����  
	if (x1 <= x2 && x1 + w1 <= x2)
	{
		return CollisionDiFandion::atLeft;

	}	//������1 λ�ھ���2 ���Ҳ�
	else if (x1 >= x2 && x1 >= x2 + w2)
	{
		return CollisionDiFandion::atRight;
	}//������1 λ�ھ���2 ���Ϸ�  
	else if (y1 >= y2 && y1 >= y2 + h2)
	{
		return CollisionDiFandion::atTop;
	}
	//������1 λ�ھ���2 ���·�  
	else if (y1 <= y2 && y1 + h1 <= y2) {
		return CollisionDiFandion::atBottom;
	}
	//���в��ᷢ����ײ��������ʱ���϶�������ײ��  
	return CollisionDiFandion::intersected;
}

SlidedResult Fand::isSlided(const Vec2& beginDelta, const Vec2& endDelta, const Vec2 & range, const Fand & bound)
{
	auto diff = endDelta - beginDelta;
	bool isIntersected = false;
	if (abs(diff.x) > range.x && abs(diff.y) > range.y)
		isIntersected = isLineIntersectFand(beginDelta, endDelta, bound);

	if (!isIntersected)
		return SlidedResult::missed;
	else
		return beginDelta.x <= endDelta.x ? SlidedResult::left : SlidedResult::right;
}

Fand Fand::getIntersectFand(const Fand &FandA, const Fand &FandB)
{
	Vec2 bottomLeft;    //���½Ƕ���     cocos2dx��CCFand�������½�Ϊԭ�㣬w,hΪ����(���͸�)�����������Ͻ���top,���½�Ϊbottom
	Vec2 topRight;	//���ϽǶ���
	float width = 0.f, height = 0.f;

	if (FandA.intersectsFand(FandB))
	{
		bottomLeft.x = max(FandA.getMinX(), FandB.getMinX()); //���½�x����ȡ��������ori����x������
		bottomLeft.y = max(FandA.getMinY(), FandB.getMinY()); //���½�y����ȡ��������ori����y������

		topRight.x = min(FandA.getMaxX(), FandB.getMaxX()); //���Ͻ�x����ȡ�����������ϽǶ���x����С��
		topRight.y = min(FandA.getMaxY(), FandB.getMaxY());

		width = topRight.x - bottomLeft.x;
		height = topRight.y - bottomLeft.y;

		return Fand(bottomLeft.x, bottomLeft.y, width, height);
	}
	return Fand(0, 0, -1.f, -1.f);
}

/*
Բ����hitTest(Fand1, circle.x + circle.width * 0.5, circle.y + circle.height * 0.5, circle.width * 0.5).toString();
hitTest(Fand:Fandangle, cx:Number, cy:Number, r:Number):Boolean
*/
bool Fand::intersectCircle(const Fand& Fand, const Fand& circle)
{
	Size midFand = Fand.size / 2;
	float cx = circle.getMidX();
	float cy = circle.getMidY();
	float radius = circle.size.width / 2;
	//Բ����������ĵ��������
	float rx = cx - Fand.getMidX(); 		//rx = cx - (Fand.left + Fand.width * 0.5);
	float ry = cy - Fand.getMidY();			//ry = cy - (Fand.top + Fand.height * 0.5);

	float dx = min(rx, midFand.width);	//dx = Math.min(rx, Fand.width * 0.5);
	float dx1 = max(dx, -midFand.width);//dx1 = Math.max(dx, -Fand.width * 0.5);

	float dy = min(ry, midFand.height);//dy = Math.min(ry, Fand.height * 0.5);
	float dy1 = max(dy, -midFand.height);//dy1= Math.max(dy, -Fand.height * 0.5);
	return (dx1 - rx) * (dx1 - rx) + (dy1 - ry) * (dy1 - ry) <= radius * radius;
	//return Fand(0, 0, -1.f, -1.f);
}
