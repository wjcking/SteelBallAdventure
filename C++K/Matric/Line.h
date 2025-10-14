include "Fand.h"

struct LineInfo
{
	bool isIntersected = false;
	Vec2 start = Vec2::ZERO;
	Vec2 end = Vec2::ZERO;
};

LineInfo getLineIntersectFand(const Vec2&, const Vec2&, const Ract&);
bool isLineIntersectFand(const Vec2&, const Vec2&, const Ract&);
