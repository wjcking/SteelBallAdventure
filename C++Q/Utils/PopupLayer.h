#pragma once

#include "cocos2d.h"
#include "ui/UIScale9Sprite.h"
#include "../../Common/Misc/FrameCounter.h"
#include "RectTools.h"
#include "GameScriptor.h"
using namespace cocos2d;
using namespace ui;
class Overlay : public LayerColor
{
private:
	GLubyte rgbA = 150;
public:
	virtual bool init()override;
	CREATE_FUNC(Overlay);

	static Overlay* createByOpaque(const GLubyte& = 0);
};

class RObject;
class PopupLayer : public Layer
{
private:
	Menu * menuButton;
	static short nextTag;
	Overlay* overlay = nullptr;
	Ref* callbackListener;
	SEL_CallFuncN callback;
	Vec2 beginDelta;
	Vec2 endDelta;
	int touchedBeginID;
	int touchedEndID;
	Sprite* background = nullptr;
	void menuCallback(Ref* pSender);
	enum class ItemType : short
	{
		menu = 0,
		robject,
		label
	};
	Action* enterAction = nullptr;
	float bAutoClose = 0;
	bool bCovered = true;
	DelayPhase delayPopup;
	bool bSmack = false;
	bool isPrompted = false;
public:

	PopupLayer();
	~PopupLayer();
	virtual bool init();
	CREATE_FUNC(PopupLayer);

	inline bool& allowSmack() { return bSmack; }
	inline void setSmack(const bool& value) {   bSmack = value; }
	void   setButtonName(const std::string &name, const std::string & newName);
	void  setButtonVisible(const std::string &name, const bool & flag);
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	void callbackFunc(Ref* target, SEL_CallFuncN callfun);

	void autoClose(const float& = 6);
	void setString(const std::string&,const std::string&);
	void displace(const std::string&, const Vec2&);
	bool loadScript(const std::string& popupTable);
	bool loadScript(const LuaIntf::LuaRef&);
	//没有则自己创建，而不是初始化创建
	inline Overlay* getOverlay() 
	{ 
		if (nullptr == overlay)
			overlay = Overlay::create();
		return this->overlay; 
	}
	//目前用于设置成就的位置
	inline void setBGPosition(const float& x, const float& y) { background->setPosition(x, y); }
	inline void addToBackground(Node* node) { background->addChild(node); }
	//获得背景里面的sprite
	RObject* getChild(const char* name);
	inline Vec2& getBeginDelta() { return beginDelta; };
	inline Vec2& getEndDelta() { return endDelta; }
	inline void setEnterAction(Action* enterAction) { this->enterAction = enterAction; }
	inline SlidedResult isSlided(const Vec2& range, const Rect& bound)
	{
		if (touchedBeginID != touchedEndID)
			return SlidedResult::missed;

		return RectTools::isSlided(beginDelta, endDelta, range, bound);
	}

	virtual void onEnter();
	virtual void onExit();
	inline bool& checkPrompt() { return isPrompted; }
	inline float& isAutoClose() { return bAutoClose; }
	inline bool& isCovered() { return bCovered; }
};
 
