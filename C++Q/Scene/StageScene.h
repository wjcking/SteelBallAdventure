#ifndef __MAINSCENE_H__
#define __MAINSCENE_H__

#include "cocos2d.h"
#include "../Utils/JoyStick.h" 
#include "../Role/Dialogue.h"


class Joystick;
class Overlay;
class PopupLayer;
class StageScene : public Layer
{
private:
	//显示坐标
	Label * lbOutput;
	bool isStageCorrect = false;
	//1.关卡一开始幕帘，从关卡选择界面进入才可以执行，重新关卡不需要
	//2.电影上下黑屏使用
	bool isCurtainDrawn = false;
	//打字机银幕完成后不可刷帧，  左右银幕则打字机屏幕则完成操作，不可刷帧
	bool isTypeCurtainDone = false;

	bool loadScript();
	//控制器
	Joystick* joystick;
	//退出，提示
	Menu* menuButton;
	MenuItemImage* miPrompt;
	//
	Typewriter typeCurtain;
	Dialogue dialogCurtain;
	CurtainInfo curtainInfo;

	//提示框回调
	void dialogCallback(Node*);
	void menuCallback(Ref*);
	void joystickCallback(Node*);
	void updateCurtainText();
	std::string doLuaFunction(const std::string&, const int& = 0);
	const std::string nameCurtainBlack = "curtainBlack";
	const std::string nameOfCurtianTop = "curtianTop";
	const std::string nameOfCurtianBottom = "curtianBottom";
	const std::string nameCurtainLabel = "lbCurtainText";
	const std::string nameOverlayCurtain = "overlayCurtain";

	Vec2 beginDelta;
	Vec2 endDelta;
	int touchedBeginID;
	int touchedEndID;
public:
	StageScene()
	{
	}
	CREATE_FUNC(StageScene);
	static cocos2d::Scene* createScene();
	virtual bool init()override;
//	void popupLayer();
	void gotoNext();
	void promptAchieve(const short&, const LuaRef&);
	//自定义表
	void prompt(const LuaRef& table);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) override;
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;

	//void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
  //回收点击的 
	void update(float) override;
	//ios 必须调用父类Layer::onExit();
	void onExit() override;
	void release(const bool& = false);
	//脚本doFile后才能使用，否则不能获取值
	void loadCurtain(const LuaRef& curtainTable);
	//像电影一样拉下黑色窗帘
	void dropCurtain(const LuaRef&);
	//像电影一样关闭黑色窗帘
	void shutCurtain();
	bool processCurtain();
	inline void setToolbar(const bool& flag = true)
	{
		menuButton->setVisible(flag);
		miPrompt->setVisible(flag);
	}
	inline Vec2& getBeginDelta() { return beginDelta; };
	inline Vec2& getEndDelta() { return endDelta; }
	inline void output(const char* value) { lbOutput->setString(value); }
	inline SlidedResult isSlided(const Vec2& range, const Rect& bound)
	{
		if (touchedBeginID != touchedEndID)
			return SlidedResult::missed;

		return RectTools::isSlided(beginDelta, endDelta, range, bound);
	}
	inline bool& isTypeDone() { return isTypeCurtainDone; }
	

};

#endif

