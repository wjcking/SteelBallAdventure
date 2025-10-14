
#pragma once 
#include "cocos2d.h"
#include "Fand.h"
#include "Constant.h"
#include "ui/UIButton.h"
#include "GameScriptor.h"
/*����ҡ�˷����ö��*/
enum class JoystickStyle
{
	none = 0,
	rocker = 1,
	arrow = 2,
	arrowForward = 3, //ֻ������ʾǰ��
	arrowBackward = 4 //ֻ������ʾ����
};

class Joystick : public cocos2d::Layer
{
private:
	cocos2d::EventListenerTouchAllAtOnce* listener;  /*����Ϊ��Ա�����������Ƴ�����*/
	cocos2d::Sprite* rocker;  /*�����ȡ*/
	cocos2d::Sprite* rockerBackground;  /*�����ȡ*/
	const unsigned short NoControllerTouched = 0;
	Sprite* padBackward;
	Sprite* padForward;
	//����
	MovingDirection direction;
	//��ǰ�Ļ���
	float angle;
	JoystickStyle joyStyle = JoystickStyle::rocker;
	bool isRolled = false;
	//������¼����id�������Ⱥ�˳������ ������ʹ��
	std::unordered_map<int, int> touchedTags;
	static short nextTag;

	Ref* callbackListener;
	SEL_CallFuncN callback;
public:
	CREATE_FUNC(Joystick);

	std::unordered_map<int, std::string> joypadNames;
	void loadScript(const char* padTable = Luat_Joypad);
	virtual bool init() override;
	int touchTimes = 0;

	void hideRocker()
	{
		if (joyStyle == JoystickStyle::rocker)
		{
			rocker->setVisible(false);
			rockerBackground->setVisible(false);
		}
		else
		{
			padBackward->setVisible(false);
			padForward->setVisible(false);
		}
	}
	void showRocker()
	{
		if (joyStyle == JoystickStyle::rocker)
		{
			rocker->setVisible(true);
			rockerBackground->setVisible(true);
		}
		else
		{
			padBackward->setVisible(true);
			padForward->setVisible(true);
		}
	}
	void setAllVisible(const bool&);
	virtual void onEnter() override;
	virtual void onExit() override;

	void callbackFunc(Ref* target, SEL_CallFuncN callfun);

	inline void setJoypadB() { joyStyle = JoystickStyle::arrowBackward; };
	inline void setJoypadF() { joyStyle = JoystickStyle::arrowForward; };
	inline void setJoypad() { joyStyle = JoystickStyle::arrow; };
	inline void setRocker() { joyStyle = JoystickStyle::rocker; };
	void setJoystickStyle();
	void onButtonClicked(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) override;
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) override;
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) override;

	/*��鵱ǰ�ǶȲ����÷���*/
	void checkDirection(float angle);
	cocos2d::Sprite* getRocker();
	cocos2d::Sprite* getRockerBg();
	float getAngle();
	MovingDirection& getDirection();
	void roll(const Vec2&);
	short pad(const Vec2&);
	//labelע�����lbǰ׺
	void setString(const std::string& name, const char* text);
	bool getTouchedPad(const short&);
	LuaIntf::LuaRef  getTouches();
	void setVisible(const std::string& name, const bool& flag)
	{
		auto pad = dynamic_cast<Sprite*>(getChildByName(name));
		if (nullptr == pad)	return;

		auto isVisible = pad->isVisible();
		pad->setVisible(flag);
	};

};