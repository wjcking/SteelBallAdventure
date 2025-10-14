#include "ActionExtend.h"
#include "../Map/MapManager.h"
#include "../Map/TiledMap.h"
#include "ResourceHelper.h"
EaseInOut* ActionExtend::parabolaTo(const float& duration, const Vec2& startPoint, const Vec2& endPoint, const float& height, const float& angle)
{
	// �ѽǶ�ת��Ϊ����
	float radian = angle*3.14159 / 180.0;
	// ��һ�����Ƶ�Ϊ��������뻡���е�
	float q1x = startPoint.x + (endPoint.x - startPoint.x) / 4.0;
	Vec2 q1 = Vec2(q1x, height + startPoint.y + cos(radian)*q1x);
	// �ڶ������Ƶ�Ϊ���������ߵ��е�
	float q2x = startPoint.x + (endPoint.x - startPoint.x) / 2.0;
	Vec2 q2 = Vec2(q2x, height + startPoint.y + cos(radian)*q2x);

	//��������
	ccBezierConfig cfg;
	cfg.controlPoint_1 = q1;
	cfg.controlPoint_2 = q2;
	cfg.endPosition = endPoint;
	//ʹ��CCEaseInOut�������˶���һ����������ı仯���Եø���Ȼ
	return  EaseInOut::create(BezierTo::create(duration, cfg), 0.5);
}
void ActionExtend::corrupt(const Vec2& ptc, const unsigned short& shardIndex)
{	 
	auto position = MAP_STAGE->getPositionByTileCoordinate(ptc);
	Sprite* debris = nullptr;
	SpriteFrame* frame;
	for (int i = 0; i < 4; i++)
	{
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("debris%d.png", shardIndex));
		if (nullptr == frame)
			return;
		debris = Sprite::createWithSpriteFrame(frame);
		debris->setPosition(position);
		MAP_STAGE->addChild(debris);
		debris->setLocalZOrder(Z_ROLE + 1);
		debris->runAction(RepeatForever::create(RotateBy::create(0.8f, 360)));
		debris->runAction(Sequence::create(
			parabolaTo(1.5f, debris->getPosition(), Vec2(debris->getPositionX() + ((i % 2) == 0 ? 50 : -50), 30), random(-24, -14), random(78, 84)),
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, debris)), nullptr));
	}
	Resh::playSound("sound/broke.wav");

}

void ActionExtend::toast(const LuaIntf::LuaRef& ref)
{
	//if (!ref.isTable())
	//	return;
	//Role* role = nullptr;
	//if (ref.has(Luaf_CppRef))
	//	role = 
	//Spawn::create()
}

void ActionExtend::explode(const LuaIntf::LuaRef& ref)
{
	//auto delay = 1.5f;// ref.has(Luaf_Delay) ? ref.get(Luaf_Delay).toValue<float>() : 1.5f;
	//auto frame = "explode_white"; //ref.has(Luaf_Frame) ? ref.get(Luaf_Frame).toValue<std::string>() : "explode_white";
	//auto explode = Sprite::createWithSpriteFrameName(StringUtils::format("%s%d.png", frame.c_str(), random(1, 4)));
	//if (nullptr != explode)
	//{
	//	 
	//	explode->setPosition(getCollisionBound().getMidX(), getCollisionBound().getMidY());
	//	explode->setLocalZOrder(getLocalZOrder() + 1);
	//	MAP_STAGE->addChild(explode);

	//	explode->runAction(Sequence::create(DelayTime::create(delay),
	//		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, explode)),
	//		nullptr));
	//}
}
