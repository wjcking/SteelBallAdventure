#pragma once 
#include "Role.h"


using namespace std;

class RObject : public Role
{
private:
	//�����ƶ���ɸ������ moves ʹ��
	unsigned short mobileFinished = 0;
	//��Ƭ��ɫ
	unsigned short debrisColor;
	vector<MovePhase> moves;
	//����������
	vector<KnockPhase> knocks;
	//��������,���Ϊ��������������Լ�ʱ��,�����İ���ʱ���ʹ���rect
	BlinkPhase blink;
	//���أ���ť�任
	FrameSwitch frameSwitchTemplate;
	std::unordered_map<int, FrameSwitch> frameSwitch;
	//bool isLabeled
	LabelPhase labelPhase;
	BlinkStatus  blinkObject();
	Size originSize;
	//��������
	SpringInfo springInfo;

	//ˢ�µ���
	void updateSpring();
	//void proceedKnocks(function<void(KnockPhase&)> knoPhase);
	//�Ƿ��뿪���ɻ򿪹أ������ж� �պ��뿪������Ǹ��ڹ���
	bool isGettingOff(Role&, const Vec2& = Vec2(10.f, 10.f));

public:
	RObject();
	~RObject();
	CREATE_FUNC(RObject);
	/*
	*��������
	*/
	void update() override;
	void loadScript() override;
	/*
	*ִ�ж���
	*/
	KnockPhase gotKnocked(Role&);
	void checkKnocks(Role&);
	void blinkSelf();
	void blinkGroup();
	//ע��step Vec2 ��ô���0.7������ײ��׼ȷ
	SpringInfo& bounce(Role&);
	std::string pollChar();
	FrameSwitch switchFrame(Role&);
	/*
	*ע��ű�
	*/
	void registerSpring(const LuaRef&);

	void registerChar(const LuaRef&);
	void registerText(const LuaRef&);
	void registerKnocks(const LuaRef&);
	void registerSwitch(const LuaRef&);

	void setAnimation(const LuaRef&) override;
	bool checkRactAround(Role& opponent, const bool& allowFollow);
	inline FrameSwitch& getSwitch(const int& tag) { return frameSwitch[tag]; }
	inline bool hasSwitches() { return frameSwitchTemplate.framePrefix != ""; }
	//�û�, ���û������򷢳�, ���һ�Ҫ�ж��û���λ��
	bool hasKnocks(const CollisionDirection& direction)
	{
		if (knocks.size() <= 0 )
			return false;
		if (knocks.begin()->colliderDirection == CollisionDirection::intersected)
			return true;
		if ( knocks.begin()->colliderDirection != direction)
			return false;

		return true;
	}
	inline bool isFloat() { return moves.size() > 0; }
	inline char& getChar() { return labelPhase.current; };
	//unsigned short resetFramework(const std::string&);
	inline void setBlink(const BlinkPhase& val) { blink = val; };
	inline void setDebrisColor(const unsigned short& val) { debrisColor = val; };
	//�ڷ�����ʱ�����
	inline BlinkPhase& getBlink() { return blink; };
	inline const unsigned short& getDebrisColor() const { return debrisColor; };

};
