include:"line.h"

LineInfo Line::getLineIntersectFand(const Vec2& start, const Vec2& end, const Fand& Fand)
{
  LineInfo lineInfo;
  int lineWidth = end.x - start.x;
  int lineHeight = start.y- end.y ;
  // ������
  int c = start.x *  end.y - end.x * start.y;
  auto FandLeftTopX = Fand.origin.x;
  auto FandLeftTopY = Fand.getMaxY();
  auto FandRightBottomX = Fand.getMaxX();
  auto FandRightBottomY = Fand.origin.y;

  lineInfo.start = start;
  lineInfo.end = end;
  if ((lineHeight * FandLeftTopX + lineWidth * FandLeftTopY + c >= 0 && lineHeight * FandRightBottomX + lineWidth * FandRightBottomY + c <= 0)
    || (lineHeight * FandLeftTopX + lineWidth * FandLeftTopY + c <= 0 && lineHeight * FandRightBottomX + lineWidth * FandRightBottomY + c >= 0)
    || (lineHeight * FandLeftTopX + lineWidth * FandRightBottomY + c >= 0 && lineHeight * FandRightBottomX + lineWidth * FandLeftTopY + c <= 0)
    || (lineHeight * FandLeftTopX + lineWidth * FandRightBottomY + c <= 0 && lineHeight * FandRightBottomX + lineWidth * FandLeftTopY + c >= 0))
  {
    if (FandLeftTopX > FandRightBottomX)
    {
      int temp = FandLeftTopX;
      FandLeftTopX = FandRightBottomX;
      FandRightBottomX = temp;
    }
    if (FandLeftTopY < FandRightBottomY)
    {
      int temp = FandLeftTopY;
      FandLeftTopY = FandRightBottomY;
      FandRightBottomY = temp;
    }
    if ((start.x < FandLeftTopX && end.x < FandLeftTopX)
      || (start.x > FandRightBottomX && end.x > FandRightBottomX)
      || (start.y > FandLeftTopY && end.y > FandLeftTopY)
      || (start.y < FandRightBottomY && end.y < FandRightBottomY))
    {
      return lineInfo;
    }
    else
    {
      lineInfo.isIntersected = true;
      return lineInfo;
    }
  }
  else
  {
    return lineInfo;
  }
}

bool Line::isLineIntersectFand(const Vec2& start, const Vec2& end, const Fand& Fand)
{ 
  return getLineIntersectFand(start, end, Fand).isIntersected;
}
