#include "OBB.h"

ObbRect::Projection::Projection(float _min, float _max) :min(_min), max(_max)
{

}

bool ObbRect::Projection::isOverlap(const Projection& proj)
{
	if (min > proj.max)
		return false;
	if (max < proj.min)
		return false;

	return true;
}

/****OBB***/
Vec2* ObbRect::getAxies()
{
	Vec2* axie = new Vec2[4];
	for (int i = 0; i < 4; i++)
	{
		auto s = vertex[i] - vertex[(i + 1) % 4];
		s.normalize();
		axie[i].x = -s.y;
		axie[i].y = s.x;
	}
	return axie;
}

ObbRect::Projection ObbRect::getProjection(const Vec2 & axie)
{
	float min = vertex[0].dot(axie);
	float max = min;

	for (int i = 1; i < 4; i++)
	{
		float temp = vertex[i].dot(axie);
		if (temp > max)
			max = temp;
		else if (temp < min)
			min = temp;
	}

	return Projection(min, max);
}

Vec2 ObbRect::getVertex(const int & index) const
{
	assert(0 <= index && index <= 3 && "MAST 0 TW 3 INDEX");
	return vertex[index];
}

void ObbRect::setVertex(const int & index, const float & x, const float & y)
{
	assert(0 <= index && index <= 3 && "MAST 0 TW 3");
	vertex[index].x = x;
	vertex[index].y = y;
}

void ObbRect::setVertex(const int & index, const Vec2 & v)
{
	assert(0 <= index && index <= 3 && "MAST 0 TW 3");
	vertex[index].x = v.x;
	vertex[index].y = v.y;
}

bool ObbRect::isCollidWithOBB(ObbRect obb)
{
	auto axie1 = getAxies();
	auto axie2 = obb.getAxies();

	Projection p1;
	Projection p2;
	for (int i = 0; i < 4; i++)
	{
		 p1 = getProjection(axie1[i]);
		 p2 = obb.getProjection(axie1[i]);

		if (!p1.isOverlap(p2))
			return false;
	}

	for (int i = 0; i < 4; i++)
	{
		 p1 = getProjection(axie2[i]);
		 p2 = obb.getProjection(axie2[i]);

		if (!p1.isOverlap(p2))
			return false;
	}

	delete[] axie1;
	delete[] axie2;

	return true;
}
