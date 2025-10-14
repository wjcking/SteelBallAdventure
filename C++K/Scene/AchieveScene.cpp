#include "AchieveScene.h"
#include "../Utils/ResourceHelper.h"
#include "../Utils/Funcpads.h"
#include "../Utils/GameScriptor.h"
#include "../Utils/Constant.h"
#include "ui/UIButton.h"
#include "StageScene.h"
using namespace ui;

cocos2d::Scene* AchieveScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = AchieveScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool AchieveScene::init()
{
	if (!Layer::init())
		return false;
	auto achieveTable = LUAH->getGlobal(Luat_Achievements);
	CCASSERT(achieveTable.isTable(), StringUtils::format("请设置%s", Luat_Achievements).c_str());

	auto bgSky = Sprite::create("background/achieve.png");
	addChild(bgSky, Z_BACKGROUND);
	bgSky->setPosition(bgSky->getContentSize().width / 2.f, bgSky->getContentSize().height / 2.f - 2);

	//LUAH->doStage();
	LUAH->doScene();
	auto  funcMenu = FuncMenu::create();
	funcMenu->callbackFunc(this, callfuncN_selector(AchieveScene::funcMenuCallback));
	funcMenu->loadScript("AchieveMenu");
	funcMenu->setPosition(Vec2::ZERO);
	addChild(funcMenu, Z_JoyStick);


	auto pageview = PageView::create();
	pageview->setLocalZOrder(-100);
	//pageview->setTouchEnabled(true);
	pageview->setContentSize(Size(ScreenSize.width, ScreenHeight * 0.7));
	
	pageview->setBackGroundColor(Color3B(0, 255, 0));

	//创建4个page 
	float multi = 0.5f;
	const float dividedParam = 2.0f;
	float dividedY = dividedParam;
	const float offsetX = 20;
	unsigned short achieveIndex = 1;
	auto pageCount = achieveTable.len() / 6;
	auto achieveCount = achieveTable.len() < 6 ? achieveTable.len() : 6;
	pageCount = pageCount == 0 ? 1 : pageCount;
	LuaIntf::LuaRef ref;
 
	std::string achieveImageName;
	for (int i = 0; i < pageCount; i++)
	{
		Layout * layout = Layout::create();
		layout->setContentSize(pageview->getContentSize());
		for (int j = 1; j <= achieveCount; j++)
		{
			ref = achieveTable[achieveIndex].value();
			auto btnAchieve = Button::create("control/achieve/bg.png", "control/achieve/bg.png");
			achieveImageName = StringUtils::format("control/achieve/aimg%d.png", achieveIndex);
			//如果该索引图片不存在则使用默认
			if (!FileUtils::getInstance()->isFileExist(achieveImageName))
				achieveImageName = "control/achieve/aimg.png";
		 
			auto aimg = Sprite::create(achieveImageName);
			aimg->setPosition(aimg->getContentSize().width / 1.4, btnAchieve->getContentSize().height / 2);
			btnAchieve->addChild(aimg);
			auto text = Label::createWithSystemFont(ref[Luaf_Text].value<std::string>(), Resh::getFontName(), 14.f);
			text->setColor(Color3B::BLACK);
			text->setPosition(aimg->getContentSize().width * 2 + offsetX, btnAchieve->getContentSize().height / 2);


			btnAchieve->addChild(text);
			btnAchieve->setName(StringUtils::format("%s%d", User_Achieve, achieveIndex));
			btnAchieve->setPosition(Vec2(btnAchieve->getContentSize().width * multi + 40 * multi, pageview->getContentSize().height / dividedY));
			//如果没有达成则变灰
			//最后AchieveIndex ++
			if (!Resh::getAchieve(achieveIndex++))
			{
				btnAchieve->runAction(FadeTo::create(0.1f, 88));
				aimg->runAction(FadeTo::create(0.1f, 88));
			}
			layout->addChild(btnAchieve);
			if (j % 3 == 0)
			{
				dividedY = dividedParam * 2.5f;
				//重置
				multi = 0.5f;

			}
			else
				multi++;
		}
		//重置
		multi = 0.5f;
		dividedY = dividedParam;
		pageview->addPage(layout);
	}

	//pageview在
	pageview->setPosition(Vec2(80, ScreenHeight / 4));
	//pageview->addEventListener(CC_CALLBACK_2(this, &pagevieweventselector(AchieveScene::pageviewCallBack));
	this->addChild(pageview, Z_JoyStick);

	return true;
}
void AchieveScene::funcMenuCallback(Node * sender)
{
	//if (callback && callbackListener)
	//{
	//	(callbackListener->*callback)(menuItem);
	//}
}
 
void AchieveScene::pageviewCallBack(cocos2d::Ref * psender, cocos2d::ui::PageViewEventType type)
{

}

bool AchieveScene::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	return false;
}

void AchieveScene::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
}
