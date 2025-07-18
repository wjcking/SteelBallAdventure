#pragma once
#include "cocos2d.h"
class FuncMenu;
class MainScene : public cocos2d::Layer
{
private:
	FuncMenu* funcMenu;
	void funcMenuCallback(Node* sender);
	static bool isSplashed;
public:
	MainScene();
	~MainScene();
	static cocos2d::Scene* createScene();
	virtual bool init()override;
	CREATE_FUNC(MainScene);
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	// void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	void loadScript();
	void splash();
	void promptAchieve(const short& unlockedID);
	 
};

