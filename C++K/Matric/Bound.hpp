//��ɫ����Ƭ����
enum MovingDirection : unsigned short
{
  stayStill = 0,
  toTop = 1,
  toBottom = 2,
  toLeft = 3,
  toRight = 4,
  toTopLeft = 5,
  toTopRight = 6,
  toBottomLeft = 7,
  toBottomRight = 8
};
//��ײǰ��λ�� ��2��n�η�����ֵ
enum CollisionDirection :short
{
  intersected = 0,
  atLeft,
  atRight,
  atTop,
  atBottom,
};

enum BoundPosition : unsigned short
{
  top = 1,
  bottom = 0,
  left = 2,
  right = 3,
  topLeft = 4,
  topRight = 5,
  bottomLeft = 6,
  bottomRight = 7,
  centerSelf = 8
};
static const unsigned short BandRact = static_cast<unsigned short>(BoundPosition::centerSelf) + 1;

enum RangeType : short
{
  rangeOut = 1,
  rangeOutOfSight = 2,
  rangeSensed = 3,
  rangeThreatened = 4,
  rangeEndangered = 5,
};

enum class SlidedResult
{
  missed,
  left,
  right,
};