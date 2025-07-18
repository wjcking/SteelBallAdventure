#include "MainScene.h"
#include "StageScene.h"
#include "ChapterScene.h"
#include "../Utils/Funcpads.h"
#include "../Utils/GameScriptor.h"
#include "../Utils/PopupLayer.h"

#include "../Ads/Ads.h"

bool MainScene::isSplashed = false;
MainScene::MainScene()
{

}


MainScene::~MainScene()
{

}

Scene* MainScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool MainScene::init()
{
	if (!Layer::init())
		return false;


	if (!UserDefault::getInstance()->getBoolForKey(User_IsInitialized, false))
	{
		switch (Application::getInstance()->getCurrentLanguage())
		{
		case LanguageType::CHINESE:
			UserDefault::getInstance()->setIntegerForKey(User_Language, Lang_Chinwan);
			break;
		case LanguageType::ENGLISH:
			UserDefault::getInstance()->setIntegerForKey(User_Language, Lang_English);
			break;
		default:
			UserDefault::getInstance()->setIntegerForKey(User_Language, Lang_English);
			break;
		}

		UserDefault::getInstance()->setBoolForKey(User_IsInitialized, true);
		//�浵
		UserDefault::getInstance()->flush();
	}
	loadScript();
	splash();

	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}
void MainScene::funcMenuCallback(Node* sender)
{
    auto menuItem = static_cast<Node*>(sender);
	auto lang = UserDefault::getInstance()->getIntegerForKey(User_Language, Lang_Chinwan);

	if (menuItem->getName() == "chinwan" && lang != Lang_Chinwan)
	{
		UserDefault::getInstance()->setIntegerForKey(User_Language, Lang_Chinwan);
		UserDefault::getInstance()->flush();
		loadScript();
	}

	else if (menuItem->getName() == "america" && lang != Lang_English)
	{
		UserDefault::getInstance()->setIntegerForKey(User_Language, Lang_English);
		UserDefault::getInstance()->flush();
		loadScript();
	}

}
bool MainScene::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	return true;
}

void MainScene::onTouchEnded(Touch* touches, cocos2d::Event* event)
{

}

void MainScene::loadScript()
{
	removeAllChildrenWithCleanup(true);
	auto bgSky = Sprite::create("background/main.png");
	addChild(bgSky);
	bgSky->setPosition(bgSky->getContentSize().width / 2.f, bgSky->getContentSize().height / 2.f - 2);

	LUAH->doScene();
	LUAH->preloadResources("PreloadFiles");

	funcMenu = FuncMenu::create();
	funcMenu->callbackFunc(this, callfuncN_selector(MainScene::funcMenuCallback));
	funcMenu->loadScript("MainMenu");
	funcMenu->setPosition(Vec2::ZERO);
	addChild(funcMenu, Z_JoyStick);

	LUAH->registerRef(Luaf_MainScene, this);
}

void MainScene::splash()
{
	if (isSplashed)
		return;
	auto overlay = Overlay::createByOpaque();
	overlay->setPosition(overlay->getContentSize().width / 2.f, overlay->getContentSize().width / 2.f);
	addChild(overlay, Z_Curtain - 1);
	auto splash = Sprite::create(StringUtils::format("background/splash%d.png", random(1, 2)));
	splash->setPosition(splash->getContentSize().width / 2.f, splash->getContentSize().height / 2.f - 2);
	addChild(splash, Z_Curtain);
	auto sequence = Sequence::create(
		DelayTime::create(2.5f),
		FadeOut::create(1.2f),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, splash)),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, overlay)),
		nullptr);
	splash->runAction(sequence);
	isSplashed = true;
}

void MainScene::promptAchieve(const short & unlockedID)
{
}