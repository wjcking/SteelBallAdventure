#include "Funcpads.h"
#include "../Scene/ChapterScene.h"
#include "../Scene/AchieveScene.h"
#include "../Scene/StageScene.h"
#include "../Scene/MainScene.h"
#include "GameScriptor.h"
#include "ResourceHelper.h"
#include "Utils.h"
USING_NS_CC;

short FuncMenu::nextTag = 1;

void FuncMenu::loadScript(const char* padTable)
{
	table = padTable;
	//重置标识
	FuncMenu::nextTag = 1;
	//int testPos = 50;
	LUAH->processTable(padTable, [&](LuaIntf::LuaRef ref) {
		assert(ref.has(Luaf_Pos) && "请设置起始位置");
		assert(ref.has(Luaf_Name) && "没有名称");
		auto pos = ref[Luaf_Pos].value<Vec2>();
		auto name = ref[Luaf_Name].value<std::string>();

		//如果是文本
		if (ref.has(Luaf_Text))
		{
			auto text = ref[Luaf_Text].value<std::string>();
			auto label = MenuItemLabel::create(Label::createWithSystemFont(text, Resh::getFontName(), 20.f));
			label->setName(name);
			label->setPosition(pos);
			label->setTag(nextTag++);

			addChild(label);
		}
		else
		{
			auto file = ref[Luaf_File].value<std::string>();

			assert(FileUtils::getInstance()->isFileExist(file) && "没有发现图片");
			auto  selectedFile = getSelectedFile(file);
			//ios必须判断
			if (!FileUtils::getInstance()->isFileExist(selectedFile))
				selectedFile = file;
			// 创建图片菜单按钮
			auto menuImage = MenuItemImage::create(file, selectedFile, CC_CALLBACK_1(FuncMenu::menuCallback, this));
			if (name == "bgmusic")
			{
				auto isEnabled = UserDefault::getInstance()->getBoolForKey(User_AllowMusic, true);
				if (isEnabled)
				{
					menuImage->setNormalImage(Sprite::createWithTexture(TextureCache::getInstance()->addImage("control/bgmusic.png")));
					menuImage->setDisabledImage(Sprite::createWithTexture(TextureCache::getInstance()->addImage("control/bgmusic.png")));
				}
				else
				{
					menuImage->setNormalImage(Sprite::createWithTexture(TextureCache::getInstance()->addImage("control/bgmusic_d.png")));
					menuImage->setDisabledImage(Sprite::createWithTexture(TextureCache::getInstance()->addImage("control/bgmusic_d.png")));
				}
			}

			menuImage->setName(name);
			menuImage->setTag(nextTag++);
			menuImage->setPosition(pos);
			addChild(menuImage);

		}
	});
}

void FuncMenu::menuCallback(Ref * sender)
{
	auto menuItem = static_cast<MenuItemImage*>(sender);
	Resh::playSound("sound/menuitem.wav");
	if (menuItem->getName() == "startgame")
	{
		//继续
		Luah::CurrentStage = UserDefault::getInstance()->getIntegerForKey(User_LastStage, 1);

		Director::getInstance()->replaceScene(StageScene::createScene());
	}
	else if (menuItem->getName() == "achieve")
		Director::getInstance()->replaceScene(AchieveScene::createScene());
	else if (menuItem->getName() == "selectlevels")
		Director::getInstance()->replaceScene(ChapterScene::createScene());
	else if (menuItem->getName() == "goback")
		Director::getInstance()->replaceScene(MainScene::createScene());
	else if (menuItem->getName() == "bgmusic")
	{
		auto isEnabled = UserDefault::getInstance()->getBoolForKey(User_AllowMusic, true);
		auto flag = isEnabled ? false : true;
		UserDefault::getInstance()->setBoolForKey(User_AllowMusic, flag);
		if (flag)
		{

			menuItem->setNormalImage(Sprite::createWithTexture(TextureCache::getInstance()->addImage("control/bgmusic.png")));
			menuItem->setDisabledImage(Sprite::createWithTexture(TextureCache::getInstance()->addImage("control/bgmusic.png")));
		}
		else
		{
			menuItem->setNormalImage(Sprite::createWithTexture(TextureCache::getInstance()->addImage("control/bgmusic_d.png")));
			menuItem->setDisabledImage(Sprite::createWithTexture(TextureCache::getInstance()->addImage("control/bgmusic_d.png")));
		}
	}

	else
	{
		if (callback && callbackListener)
			(callbackListener->*callback)(menuItem);

		auto params = LUAH->createTable();
		params.set(Luaf_Tag, menuItem->getTag());
		params.set(Luaf_Name, menuItem->getName());
		params.set(Luaf_Table, table);
		// if (nullptr != menuItem->getUserData())
		//params.set(Luaf_Value, static_cast<const char*>(menuItem->getUserData())); 
		LUAH->callmenu("funcpads", "funcpads", params);
	}

}

bool FuncMenu::init()
{
	if (Menu::init())
		return true;

	return false;
}

void FuncMenu::callbackFunc(cocos2d::Ref *target, SEL_CallFuncN callfun)
{
	callbackListener = target;
	callback = callfun;
}
