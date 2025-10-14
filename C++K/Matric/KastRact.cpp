#include "KastRact.h"

RactAround::KastRact::KastRact(float _min, float _max) :min(_min), max(_max)
{

}

bool RactAround::KastRact::isOverlap(const KastRact& kast)
{
	if (min > kast.max)
		return false;
	if (max < kast.min)
		return false;

	return true;
}

Vec2* RactAround::getAxies()
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

Vec2 RactAround::getVertex(const int & index) const
{
	assert(0 <= index && index <= 3 && "MAST 0 TW 3 INDEX");
	return vertex[index];
}

void RactAround::setVertex(const int & index, const float & x, const float & y)
{
	assert(0 <= index && index <= 3 && "MAST 0 TW 3");
	vertex[index].x = x;
	vertex[index].y = y;
}

void RactAround::setVertex(const int & index, const Vec2 & v)
{
	assert(0 <= index && index <= 3 && "MAST 0 TW 3");
	vertex[index].x = v.x;
	vertex[index].y = v.y;
}

RactAround.KastRact RactAround::getKastRact(const Vec2& axie)
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

	return KastRact(min, max);
}

bool RactAround::isCollidWith(RactAround& bzked)
{
	auto axie1 = getAxies();
	auto axie2 = bzked.getAxies();

	KastRact p1;
	KastRact p2;

	for (int i = 0; i < 4; i++)
	{
		 p1 = getKastRact(axie1[i]);
		 p2 = bzked->getKastRact(axie1[i]);

		if (!p1.isOverlap(p2))
			return false;
	}

	for (int i = 0; i < 4; i++)
	{
		 p1 = getKastRact(axie2[i]);
		 p2 = bzked->getKastRact(axie2[i]);

		if (!p1.isOverlap(p2))
			return false;
	}

	delete[] axie1;
	delete[] axie2;

	return true;
}
