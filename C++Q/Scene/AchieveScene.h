#pragma once
#include "cocos2d.h"
#include "ui/UIPageView.h"
 
using namespace cocos2d;

class AchieveScene : public Layer
{
private:
	void funcMenuCallback(Node* sender);
public:
	AchieveScene()
	{
	}
	static cocos2d::Scene* createScene();

	virtual bool init() override; 
	CREATE_FUNC(AchieveScene);
	void pageviewCallBack(cocos2d::Ref * psender, cocos2d::ui::PageViewEventType type);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;

};