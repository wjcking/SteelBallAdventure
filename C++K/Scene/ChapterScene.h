#pragma once
#include "cocos2d.h"
#include "ui/UIPageView.h"
 
using namespace cocos2d;

class ChapterScene : public Layer
{
private:
    ui::ScrollView* scrollView = nullptr;
	void funcMenuCallback(Node* sender);
    static Vec2 lastContainerPosition;
	static bool isInitialized;
public:
	ChapterScene()
	{
	}
	static cocos2d::Scene* createScene();

	virtual bool init() override; 
	CREATE_FUNC(ChapterScene);
 
	void onButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
};
