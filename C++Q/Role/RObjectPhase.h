#pragma once
#include "../Utils/RectTools.h"
#include "../Utils/Constant.h"
#include "../../Common/Misc/FrameCounter.h"
#include "BlinkSystem.h"

enum class KnockAction
{
	normal,
	//���Դ�͸��
	hollow,
	//���ص�
	hidden,
	corrupt,
	//���˶��ٴκ��������
	popup,
	//����ַ�
	fetchChar,
};

struct KnockPhase : DelayPhase
{
	KnockAction knockAction = KnockAction::normal;
	CollisionDirection colliderDirection = CollisionDirection::intersected;
	CollisionDirection knockerDirection = CollisionDirection::intersected;
	//�Ƿ��vector���Ƴ��û�����
	bool isRemovable = false;
	bool isColliding = false;
	Vec2 replacedTile = Vec2(1, 3);
	std::string sound = "";
	//�û��˶��ٴ�
	unsigned short times = 0;
	short limitedTimes = -1;	//-1=������
	LuaRef toast = LUAH->createTable();
	KnockPhase(const LuaRef& ref)
	{
		limitedTimes = ref.get(Luaf_limitedTimes,-1);
		replacedTile = ref.get(Luaf_Tile, Vec2(1, 3));
		knockAction = ref.get(Luaf_Type, KnockAction::normal);
		colliderDirection = ref.get(Luaf_Direction, CollisionDirection::intersected);
		//�ӳٶ����������
		delaySecond = ref.get(Luaf_Delay, 0);
		if (ref.has(Luaf_Sound))
			sound = ref.get(Luaf_Sound).toValue<std::string>();
		if (ref.has(Luaf_Toast))
			toast = ref.get(Luaf_Toast);
	}

	//һ��ˢ֡���Ƿ�������һ��
	inline bool allowNext() { return limitedTimes == -1 || times < limitedTimes; };

	KnockPhase() {}
};

//�ƶ�ѡ��
struct MovePhase : DelayPhase
{
	MovingDirection direction;
	bool isDone = false;

	Vec2 moveSteps;
	Vec2 distance;
	MovePhase(const LuaRef& ref)
	{
		//blinkZones.reserve(25);
		delaySecond = ref.get(Luaf_Delay, 1);
		moveSteps = ref.get(Luaf_MoveStep, Vec2(1.5f, 1.5f));
		direction = ref.get(Luaf_Direction, MovingDirection::stayStill);
		distance = ref.get(Luaf_Distance, Vec2(100, 100));
	}

	inline void reset()
	{
		isDone = false;
		DelayPhase::reset();
	}
};

struct BlinkPhase :DelayPhase
{
	//����ʱ���
	float strampHidden = 0.f;
	Vec2 spawnPosition;
	//�೤ʱ�������
	float visibleDuration = 2;
	unsigned short groupID;
	bool isShown = false;
	bool isDone = false;
	int tag = 0;

	BlinkPhase() {}
	BlinkPhase(LuaIntf::LuaRef ref)
	{
		if (ref.has(Luaf_Pos))
			spawnPosition = ref.get(Luaf_Pos).toValue<Vec2>();
		if (ref.has(Luaf_Duration))
			visibleDuration = ref.get(Luaf_Duration).toValue<float>();
		if (ref.has(Luaf_Delay))
			delaySecond = ref.get(Luaf_Delay).toValue<float>();
	}
	inline void reset()
	{
		isDone = false;
		isShown = false;
		strampHidden = 0.f;
		DelayPhase::reset();
	}
};

struct LabelPhase : DelayPhase
{
	char start;
	char end;
	char current;
	LabelPhase() {}
	LabelPhase(const LuaRef& ref)
	{
		start = ref.get(Luaf_RangeStart, 'A');
		end = ref.get(Luaf_RangeEnd, 'Z');
		current = ref.get(Luaf_Text, 'A');
	}
	char getNextChar()
	{
		if (current >= end)
		{
			current = start;
			return current;
		}

		return current++;
	}
	char poll()
	{
		if (current >= end)
		{
			current = start;
			return current;
		}

		if (isTimeUp())
		{
			reset();
			return current++;
		}
		return current;
	}
	inline std::string getString()
	{
		std::string str;
		std::stringstream stream;
		stream << current;
		str = stream.str();
		return str;
	}
};
//����
struct FrameSwitch : FrameIndexes
{
	//int tag = 0;
	const char* framePrefix="";
	//armeabi gcc �����ø���FrameIndexesע��lua����������
	inline unsigned short& getIndex() { return index; }
	//unsigned short frameIndex;
	CollisionDirection	direction = CollisionDirection::atTop;
	std::string	sound = "";
	//��ײ��δ��ײ��Ĵ���
	short touchedCount = 0;
	bool isTouched = false;
	short touchedTimes = 0;
	//-1=������
	short limitedTimes = -1;
	//һ��ˢ֡���Ƿ�������һ��
	inline bool isUnlimited() { return limitedTimes == -1; };
	inline bool allowNext() { return limitedTimes == -1 || touchedTimes < limitedTimes; };
};

//����
struct SpringInfo : FrameSwitch
{
	SpringInfo() :delaySpring(0.05f) {};
	bool isBounced = false;
	int times = 0;
	Vec2 force = Vec2(0, 8);
	DelayPhase delaySpring;
	inline void operator=(const FrameIndexes& fi)
	{
		index = fi.index;
		start = fi.start;
		end = fi.end;
	}
};
//���� 
struct ReboundInfo : FrameSwitch
{
	bool isRunning = false;
	bool isActive = true;
	bool allowBack = true;
	float duration = 1;
	float durationBack = 1;
	float delay = 0;
	float delayBack = 0;
	Vec2 offset = Vec2(0, 50.f);
};