
#pragma once 
#include "cocos2d.h"
#include "RectTools.h"
#include "Constant.h"
#include "ui/UIButton.h"
#include "GameScriptor.h"
/*定义摇杆方向的枚举*/
enum class JoystickStyle
{
	none = 0,
	rocker = 1,
	arrow = 2,
	arrowForward = 3, //只允许显示前进
	arrowBackward = 4 //只允许显示后退
};

class Joystick : public cocos2d::Layer
{
private:
	cocos2d::EventListenerTouchAllAtOnce* listener;  /*定义为成员变量，方便移除监听*/
	cocos2d::Sprite* rocker;  /*方便获取*/
	cocos2d::Sprite* rockerBackground;  /*方便获取*/
	const unsigned short NoControllerTouched = 0;
	Sprite* padBackward;
	Sprite* padForward;
	//方向
	MovingDirection direction;
	//当前的弧度
	float angle;
	JoystickStyle joyStyle = JoystickStyle::rocker;
	bool isRolled = false;
	//用来记录触摸id，按照先后顺序重置 控制器使用
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

	inline void hideRocker()
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
	inline void showRocker()
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

	/*检查当前角度并设置方向*/
	void checkDirection(float angle);
	cocos2d::Sprite* getRocker();
	cocos2d::Sprite* getRockerBg();
	float getAngle();
	MovingDirection& getDirection();
	void roll(const Vec2&);
	short pad(const Vec2&);
	//label注意加了lb前缀
	void setString(const std::string & name, const char* text);
	bool getTouchedPad(const short&);
	LuaIntf::LuaRef  getTouches();
	inline void setVisible(const std::string& name, const bool& flag)
	{
		auto pad = dynamic_cast<Sprite*>(getChildByName(name));
		if (nullptr == pad)
			return;
		auto isVisible = pad->isVisible();
		pad->setVisible(flag);
	};

};