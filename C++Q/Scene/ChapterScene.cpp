#include "ChapterScene.h"
#include "../Utils/Funcpads.h"
#include "../Utils/GameScriptor.h"
#include "../Utils/Constant.h"
#include "../Utils/ResourceHelper.h"
#include "ui/UIButton.h"
#include "StageScene.h"
using namespace ui;
Vec2 ChapterScene::lastContainerPosition = Vec2::ZERO;
bool ChapterScene::isInitialized = false;
cocos2d::Scene* ChapterScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ChapterScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool ChapterScene::init()
{
	if (!Layer::init())
		return false;
	auto chapterLength = LUAH->getGlobal(Luat_Chapters);
	CCASSERT(chapterLength.isTable(), StringUtils::format("请设置%s", Luat_Chapters).c_str());

	auto bgSky = Sprite::create("background/levels.png");
	addChild(bgSky, Z_BACKGROUND);
	bgSky->setPosition(bgSky->getContentSize().width / 2.f, bgSky->getContentSize().height / 2.f - 2);

	LUAH->doScene();

	auto  funcMenu = FuncMenu::create();
	funcMenu->callbackFunc(this, callfuncN_selector(ChapterScene::funcMenuCallback));
	funcMenu->loadScript("LevelMenu");
	funcMenu->setPosition(Vec2::ZERO);
	addChild(funcMenu, Z_JoyStick);
	scrollView = ui::ScrollView::create();
	scrollView->setScrollBarEnabled(false);
	scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	scrollView->setContentSize(Size(ScreenSize.width - 150, ScreenHeight / 1.6f));
	scrollView->setPosition(Vec2(160, ScreenHeight / 5));
	scrollView->scrollToBottom(0.1f, false);

	float multi = 0.5f;
	const float GeneralOffsetY = 10.f;
	const float liOffsetY = 70.f + GeneralOffsetY;
	const float separatorOffsetY = 50.f + GeneralOffsetY;
	int layoutHeight = 60 + liOffsetY;
	unsigned short stageNumber = 1;
	unsigned short partCount = 4;

	int totalHeight = layoutHeight * chapterLength.len();
	Button* btnStage = Button::create("control/chapter/locked.png", "control/chapter/locked.png");

	std::string title = "";
	//章节scene
	unsigned short chapterIndex = 1;
	const std::string DefaultStageImage = "control/chapter/locked.png";
	std::string btnImage = "";
	auto labelOffset = Resh::getLanguage() == Lang_Chinwan ? 60 : 60 * 1.3;
	auto lastStage = UserDefault::getInstance()->getIntegerForKey(User_LastStage, 1);
	LUAH->processTable(Luat_Chapters, [&](LuaIntf::LuaRef ref) {

		if (stageNumber > lastStage)
			return;

		auto layout = Layout::create();
		multi = 0.5f;
		if (ref.has(Luaf_Count))
			partCount = ref[Luaf_Count].value<short>();
		//章节图片
		for (int j = 1; j <= partCount; j++)
		{
			btnImage = StringUtils::format("control/chapter/stage%d.png", stageNumber);
			if (FileUtils::getInstance()->isFileExist(btnImage))
				btnStage = Button::create(btnImage, btnImage);
			else
				btnStage = Button::create(DefaultStageImage, DefaultStageImage);

			btnStage = dynamic_cast<Button*>(btnStage);
			btnStage->setName(StringUtils::format("stage%d.png", stageNumber++));
			btnStage->addTouchEventListener(CC_CALLBACK_2(ChapterScene::onButtonClicked, this));
			btnStage->setPosition(Vec2(btnStage->getContentSize().width * multi + 10 * multi, btnStage->getContentSize().height));
			layout->addChild(btnStage);
			multi++;

			if (stageNumber > lastStage)
				break;
		}
		//分隔符等
		auto li = Sprite::create("control/chapter/li.png");
		auto separator = Sprite::create("control/chapter/separator.png");
		auto label = Label::createWithSystemFont("", Resh::getFontName(), 18.f);
		li->setPosition(Vec2(li->getContentSize().width * 0.8f, btnStage->getContentSize().height + liOffsetY));
		label->setPosition(Vec2(li->getContentSize().width * 0.8f + labelOffset, btnStage->getContentSize().height + liOffsetY));
		separator->setPosition(Vec2(separator->getContentSize().width * 0.6f, btnStage->getContentSize().height + separatorOffsetY));
		if (ref.has(Luaf_Text))
			title = ref[Luaf_Text].value<std::string>();

		layout->addChild(li);
		layout->addChild(label);
		layout->addChild(separator);
		layout->setPosition(Vec2(0, totalHeight - layoutHeight * chapterIndex++ - 10));
		scrollView->addChild(layout);
		//设置标题
		label->setString(title);


	});
	scrollView->setInnerContainerSize(Size(ScreenSize.width - 170, totalHeight + 30));
	addChild(scrollView, Z_JoyStick);
	if (!isInitialized)
	{
		scrollView->jumpToTop(); 
		isInitialized = true;
		lastContainerPosition = Vec2(0, -totalHeight);
	}
	else
	{
		auto percent = 100 - abs(lastContainerPosition.y / -scrollView->getInnerContainerSize().height * 100);
		if (percent < 10)
			scrollView->jumpToTop();
		else if(percent > 90)
			scrollView->jumpToBottom();
		else
			scrollView->jumpToPercentVertical(percent);

	}

	return true;
}
void ChapterScene::funcMenuCallback(Node * sender)
{
	//if (callback && callbackListener)
	//{
	//	(callbackListener->*callback)(menuItem);
	//}
}


void ChapterScene::onButtonClicked(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{
	//触摸完毕后没有在原来的按钮上则退出
	if (Widget::TouchEventType::ENDED != type)
		return;

	auto button = static_cast<Button*>(sender);
	lastContainerPosition = Vec2(0,-button->getParent()->getPositionY()-button->getContentSize().height);
 
	sscanf(button->getName().substr(5, 2).c_str(), "%hu", &Luah::CurrentStage);
	auto table = LUAH->createTable();
	table.set(Luaf_Tag, Luah::CurrentStage);
	LUAH->callmenu("chapter", "stage", table);
	auto scene = StageScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

bool ChapterScene::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	return false;
}

void ChapterScene::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
}
