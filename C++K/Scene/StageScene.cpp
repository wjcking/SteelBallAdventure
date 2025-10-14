#include "StageScene.h"
#include "ChapterScene.h"
#include "../Utils/PopupLayer.h"
#include "../Utils/Constant.h"
#include "../Utils/ResourceHelper.h"
#include "../Map/TiledMap.h"
#include "../Map/MapManager.h"
#include "../Map/CheckPoint.h"
#include "../Role/RObject.h"
#include "../Role/Player.h"
#include "../Role/Npc.h"
#include "../Role/ProjectTile.h"
#include "../Role/RoleManager.h"
#include "../Role/RoleFrame.h"
#include "../Utils/ResourceHelper.h" 
#include "../Utils/GameScriptor.h"
#include "../Utils/JoyStick.h"
#include "../Utils/Funcpads.h"
#include "../Triggers/Trigger.h"
#include "../Triggers/TriggerSystem.h"
#include "../Map/Collapse.h"
#include "../Utils/Utils.h"
#include "../Role/DialogueSystem.h"
#include "../Ads/Ads.h"
std::string StageScene::doLuaFunction(const std::string & funcName, const int& tag)
{
	std::string output = "";
	try
	{
		auto luaFunc = LUAH->getGlobal(funcName.c_str());
		if (luaFunc.isFunction())

		{
			if (tag > 0)
				luaFunc(tag);
			else
				luaFunc();
		}
	}
	catch (LuaIntf::LuaException  e)
	{
		log("%s", e.what());
		output = e.what();
	}

	return output;
}
Scene* StageScene::createScene()
{
	auto scene = Scene::create();
	auto layer = StageScene::create();
	scene->addChild(layer);
	return scene;
}

bool StageScene::loadScript()
{
	/**************�̶��˵� *********/
	menuButton = Menu::create();
	// ����ͼƬ�˵���ť goback
	auto miGoback = MenuItemImage::create("control/goback2.png", "control/goback2_s.png", this, menu_selector(StageScene::menuCallback));
	miGoback->setName("goback");
	//menuImage->setTag(tag);
	miGoback->setPosition(Vec2(56 / 2 - 2, ScreenHeight - 56 / 2));
	menuButton->addChild(miGoback);

	// ����ͼƬ�˵���ť prompt
	miPrompt = MenuItemImage::create("control/prompt.png", "control/prompt_s.png", this, menu_selector(StageScene::menuCallback));
	miPrompt->setName("prompt");
	//menuImage->setTag(tag);
	miPrompt->setPosition(Vec2(ScreenWidth - 28, ScreenHeight - 56 / 2));
	menuButton->addChild(miPrompt);
	menuButton->setPosition(Vec2::ZERO);
	addChild(menuButton, Z_JoyStick);

	/************************************/
	//[��Ҫ��һ��]����lua
	auto stageFile = LUAH->doStage();

	// "û�з��ָ�stage";
	if (stageFile == "")
		return false;

	auto result = LUAH->doFile(stageFile.c_str());
	LUAH->preloadResources();
	//����
	LUAH->setGlobal<int>(User_Language, UserDefault::getInstance()->getIntegerForKey(User_Language, 1));
	//����
	auto bgTable = LUAH->getGlobal("Background");
	assert(bgTable.isTable() && "������ Background Table");
	Sprite* background = nullptr;
	//�������
	for (auto iter = bgTable.begin(); iter != bgTable.end(); ++iter)
	{
		//�б���ͼƬ�ļ�
		if (!iter.value().has(Luaf_File))
			break;
		auto file = iter.value()[Luaf_File].value<std::string>();
		assert(FileUtils::getInstance()->isFileExist(file) && "û�з��ֱ���ͼƬ");

		background = Sprite::create(file);

		//û�����꣬�Զ�����
		if (!iter.value().has(Luaf_Pos))
			background->setPosition(background->getContentSize().width / 2.f, background->getContentSize().height / 2.f);
		else {
			auto pos = iter.value()[Luaf_Pos].value<Vec2>();
			background->setPosition(pos);
		}

		addChild(background, Z_BACKGROUND);
	}
	//������
	joystick = Joystick::create();
	joystick->setPosition(Vec2::ZERO);
	joystick->loadScript();
	joystick->callbackFunc(this, callfuncN_selector(StageScene::joystickCallback));
	addChild(joystick, Z_JoyStick);
	//������ʾlabel
	lbOutput = Label::createWithSystemFont("", Resh::getFontName(), 16.0f);
	lbOutput->setColor(Color3B::BLACK);
	lbOutput->setPosition(220, 220);
	addChild(lbOutput, Z_Achieve);

	//������Դ��������ͼƬ�ȣ�
	lbOutput->setString(doLuaFunction(Luac_Preload));

	//�������ͼ
	MAP_MANAGER->loadScript();
	addChild(MAP_STAGE, Z_MAP);
	//�����ѭ����ͼ������
	//if (nullptr != MAP_STAGE->getRepeatedMap())
	//	addChild(MAP_STAGE->getRepeatedMap(), Z_MAP);
	ROLE_MANAGER->loadScript();
	DIALOGUE->loadScript();
	//��ʼ���Ի�
	TYPE_DIALOG->setContent(DIALOGUE->getCurrentDialog().text);

	//����
	if (Checkpoint::isChecked())
	{
		ROLE_PLAYER.setPosition(Checkpoint::getPosition());
		ROLE_PLAYER.setDesire(Checkpoint::getPosition());
		Checkpoint::setTile();
	}

	//ע��stage
	LUAH->registerRef(Luaf_StageScene, this);
	//��ʼ���¼�
	lbOutput->setString(doLuaFunction("initiate"));

	//����
	return true;
}

void StageScene::joystickCallback(Node* pSender)
{
	//auto object = static_cast<Node*>(pSender);

	//log("name=%s end:=%d", object->getName().c_str(), object->getTag());
}
void StageScene::promptAchieve(const short& unlockedID, const LuaRef& ref)
{
	//����⿪�˻���û�ҵ����ֶ�������
	if (Resh::getAchieve(unlockedID))
		return;
	//�����ɾ�
	Resh::setAchieve(unlockedID, true);

	prompt(ref);
}

void StageScene::gotoNext()
{
	Luah::CurrentStage++;
	//�ϴ���������
	if (Luah::CurrentStage >= UserDefault::getInstance()->getIntegerForKey(User_LastStage, 1))
		Resh::setNumber(User_LastStage, Luah::CurrentStage);
	release(true);

	loadScript();
}


void StageScene::prompt(const LuaRef & ref)
{
	auto promptCustomer = PopupLayer::create();
	promptCustomer->callbackFunc(this, callfuncN_selector(StageScene::dialogCallback));
	promptCustomer->loadScript(ref);
	//promptCustomer->setName(Name_Prompt);
	auto zorder = ref.get(Luaf_ZOrder, Z_PopupLayer);
	addChild(promptCustomer, zorder);
	if (promptCustomer->isCovered())
		addChild(promptCustomer->getOverlay(), zorder-1);

	if (promptCustomer->isAutoClose() >= 0)
		promptCustomer->autoClose(promptCustomer->isAutoClose());
	
}


void StageScene::dialogCallback(cocos2d::Node *node)
{
	auto menuItem = dynamic_cast<MenuItemImage*>(node);
	if ("retry" == menuItem->getName())
	{
		if (Checkpoint::life <= 0)
			Checkpoint::reset(false);
		release();
		loadScript();
	}
	else if ("continue" == menuItem->getName())
		gotoNext();
	else if ("stop" == menuItem->getName())
		return;
	else
	{
		auto table = LUAH->createTable();
		table.set(Luaf_Name, menuItem->getName());
		LUAH->callback("stage", "dialog", table);

		auto promptCustomer = dynamic_cast<PopupLayer*>(node->getParent()->getParent()->getParent());
		promptCustomer->autoClose(0.0f);

	}
}
bool StageScene::init()
{
	if (!Layer::init())
		return false;

	//�ű�
	isStageCorrect = loadScript();

	//TYPE_DIALOG->setContent(DIALOGUE->update().text);
	if (!isStageCorrect)
		return true;
	//�¼�
	auto listenerKeyboard = EventListenerKeyboard::create();
	listenerKeyboard->onKeyPressed = CC_CALLBACK_2(StageScene::onKeyPressed, this);
	listenerKeyboard->onKeyReleased = CC_CALLBACK_2(StageScene::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerKeyboard, this);
	//���㴥��
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(StageScene::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(StageScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//ˢ֡
	Director::getInstance()->getScheduler()->schedule(SEL_SCHEDULE(&StageScene::update), this, DeltaInterval, false);
	return true;
}
void StageScene::menuCallback(Ref * pSender)
{
	//���ֻ�û����ɣ���ʲô������
	//if (!isTypeCurtainDone)
	//	return;
	auto menuItem = dynamic_cast<MenuItemImage*>(pSender);
	if ("goback" == menuItem->getName())
	{
		unschedule(schedule_selector(StageScene::update));
		//�ͷ�lua,�����msgdispatcher�ӳٵ����Ѿ��ͷŵ�cpp����
		doLuaFunction("unload");

		Director::getInstance()->replaceScene(ChapterScene::createScene());
	}
	else if ("prompt" == menuItem->getName())
	{
		auto table = LUAH->createTable();
		table.set(Luaf_Tag, menuItem->getTag());
		table.set(Luaf_Name, menuItem->getName());
		LUAH->callmenu("stage", "menu", table);
	}
}
void StageScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	ROLE_PLAYER.controlByKey(keyCode, true);
}
void StageScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	ROLE_PLAYER.controlByKey(keyCode, false);
}

bool StageScene::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	beginDelta = MAP_STAGE->convertTouchToNodeSpace(touch);
	touchedBeginID = touch->getID();
	return true;
}

void StageScene::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{	//[ע��ǰ�����˳��]������Ļ
	if (!processCurtain())
		return;
	endDelta = MAP_STAGE->convertTouchToNodeSpace(touch);
	touchedEndID = touch->getID();
	//auto dialogTable = LUAH->createTable();
	//dialogTable.set("isTypeCurtainDone", isTypeCurtainDone);
	LUAH->callback("stage", "ended");
}

void StageScene::update(float dt)
{
	updateCurtainText();
	std::string result = "";

	result = doLuaFunction("update");
	//���Ի�ͼ
	MAP_MANAGER->getCurrentMap()->debugDraw();

	//����������ֹͣˢ֡
	if (result != "")
		unschedule(schedule_selector(StageScene::update));

	//�ۼ���
	Clock::addDeltaCount();
}
void StageScene::release(const bool& isClearAll)
{

	DialogueSystem::isDialogueOn = false;
	//�����ʱ��
	DelayCollection::clear();
	//�ͷŽ�ɫ
	ROLE_MANAGER->release();
	//�Ƴ� 
	MAP_STAGE->removeAllChildrenWithCleanup(true);

	//�ͷ�frame
	//ѭ����ͼ
	if (nullptr != MAP_STAGE->getRepeatedMap())
	{
		MAP_STAGE->getRepeatedMap()->removeAllChildrenWithCleanup(true);
		removeChild(MAP_STAGE->getRepeatedMap());
	}
	removeChild(MAP_STAGE);
	//�ͷ�lua
	doLuaFunction("unload");
	removeAllChildrenWithCleanup(true);
	if (isClearAll)
	{
		//��ע�⡿�洢���ͷţ����˳��ܿ���
		Checkpoint::reset();
		//�����ʱ��
	//	unschedule(schedule_selector(StageScene::update));
		//����ֻ�б任�ؿ�ʱ���ͷ�
		isCurtainDrawn = false;
		Resh::release();
        Ads::hideBanner();
	}

}
void StageScene::onExit()
{
	if (!isStageCorrect)
	{
		Layer::onExit();
		return;
	}

	release(true);
	//�Ի�

	//�ͷ���Ƶ
	Resh::release();
	Layer::onExit();
}


void StageScene::loadCurtain(const LuaRef& curtainTable)
{
	if (isCurtainDrawn || !curtainTable.isTable())
		return;

	curtainInfo = CurtainInfo(curtainTable);

	if (curtainInfo.type == CurtainType::none)
		return;

	//���ֲ㣬�̲��¼�
	if (CurtainType::black == curtainInfo.type)
	{
		//������ֻ��ı�
		dialogCurtain.loadCurtainText();
		//��ʼ����һ���Ի�
		typeCurtain.setContent(dialogCurtain.getCurrentDialog().text);
		//��Ļ�����ֻ�
		auto curtainBlack = Sprite::create(curtainInfo.image);
		if (curtainBlack == nullptr)
			return;

		curtainBlack->setPosition(curtainBlack->getContentSize().width / 2.f, curtainBlack->getContentSize().height / 2.f - 2.f);
		auto label = Label::createWithSystemFont("", Resh::getFontName(), 24.f);
		label->setName(nameCurtainLabel);
		label->setColor(Color3B::WHITE);
		label->setPosition(curtainBlack->getContentSize().width / 2.f, curtainBlack->getContentSize().height / 2.f);

		curtainBlack->setName(nameCurtainBlack);
		curtainBlack->addChild(label);
		addChild(curtainBlack, Z_Curtain);
		//�����Ƿ�������
		isTypeCurtainDone = false;
		isCurtainDrawn = true;
	}

	else if (CurtainType::bilateral == curtainInfo.type)
	{
		auto overlay = Overlay::createByOpaque();
		overlay->setName(nameOverlayCurtain);
		overlay->setPosition(overlay->getContentSize().width / 2.f, overlay->getContentSize().height / 2.f);
		addChild(overlay, Z_Curtain - 1);
		//��������
		auto curtainLeft = Sprite::create(curtainInfo.image1);
		if (curtainLeft == nullptr)
			return;
		auto curtainY = curtainLeft->getContentSize().height / 2.f - 2.f;
		curtainLeft->setPosition(curtainLeft->getContentSize().width / 2.f, curtainY);

		auto curtainRight = Sprite::create(curtainInfo.image2);
		if (curtainRight == nullptr)
			return;
		auto curtainRightX = curtainLeft->getContentSize().width + curtainRight->getContentSize().width / 2.f - 50;
		curtainRight->setPosition(curtainRightX, curtainY);

		addChild(curtainLeft, Z_Curtain);
		addChild(curtainRight, Z_Curtain);

		auto moveLeft = MoveTo::create(curtainInfo.delay, Vec2(-curtainLeft->getContentSize().width, curtainY));
		auto moveRight = MoveTo::create(curtainInfo.delay, Vec2(curtainRightX * 2, curtainY));
		auto seqLeft = Sequence::create(DelayTime::create(2),
			moveLeft,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, curtainLeft)),
			nullptr);
		auto seqRight = Sequence::create(DelayTime::create(2),
			moveRight,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, curtainRight)),
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, overlay)),
			nullptr);

		curtainLeft->runAction(seqLeft);
		curtainRight->runAction(seqRight);

		isCurtainDrawn = true;
	}

	else if (CurtainType::updown == curtainInfo.type)
	{
		auto overlay = Overlay::createByOpaque();
		overlay->setName(nameOverlayCurtain);
		overlay->setPosition(overlay->getContentSize().width / 2.f, overlay->getContentSize().height / 2.f);
		addChild(overlay, Z_Curtain - 1);
		//��������
		auto curtainTop = Sprite::create(curtainInfo.image);
		if (curtainTop == nullptr)
			return;

		auto curtainX = curtainTop->getContentSize().width / 2.f;
		curtainTop->setPosition(curtainX, ScreenSize.height - curtainTop->getContentSize().height / 2);
		addChild(curtainTop, Z_Curtain);

		auto moveTop = MoveTo::create(curtainInfo.delay, Vec2(curtainX, ScreenSize.height + curtainTop->getContentSize().height));


		auto seqTop = Sequence::create(DelayTime::create(2),
			moveTop,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, curtainTop)),
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, overlay)),
			nullptr);

		curtainTop->runAction(seqTop);

		isCurtainDrawn = true;
	}
}

void StageScene::dropCurtain(const LuaRef& curtainTable)
{
	if (!curtainTable.isTable())
		return;

	curtainInfo = CurtainInfo(curtainTable);

	//��������
	auto curtainTop = Sprite::create(curtainInfo.image1);
	if (curtainTop == nullptr)
		return;

	auto curtainX = curtainTop->getContentSize().width / 2.f;
	curtainTop->setPosition(curtainX, ScreenSize.height + curtainTop->getContentSize().height);
	curtainTop->setName(nameOfCurtianTop);
	auto curtainBottom = Sprite::create(curtainInfo.image2);
	if (curtainBottom == nullptr)
		return;

	curtainBottom->setPosition(curtainX, -curtainTop->getContentSize().height);
	curtainBottom->setName(nameOfCurtianBottom);
	addChild(curtainTop, curtainInfo.zorder);
	addChild(curtainBottom, curtainInfo.zorder);
	curtainTop->setLocalZOrder(curtainInfo.zorder);
	curtainBottom->setLocalZOrder(curtainInfo.zorder);
	auto dropTop = MoveTo::create(curtainInfo.delay, Vec2(curtainX, ScreenSize.height - curtainTop->getContentSize().height / 2));
	auto dropBottom = MoveTo::create(curtainInfo.delay, Vec2(curtainX, curtainTop->getContentSize().height / 2 - 1));//offset������ȫ����ס

	curtainTop->runAction(dropTop);
	curtainBottom->runAction(dropBottom);
}
void StageScene::shutCurtain()
{
	//����Ѿ�лĻ��ʲô������
	if (isCurtainDrawn)
		return;

	auto curtainTop = getChildByName(nameOfCurtianTop);

	if (nullptr == curtainTop)
		return;
	isCurtainDrawn = true;
	auto curtainX = curtainTop->getPositionX();
	auto curtainBottom = getChildByName(nameOfCurtianBottom);
	auto moveTop = MoveTo::create(curtainInfo.delay, Vec2(curtainX, ScreenSize.height + curtainTop->getContentSize().height));
	auto moveBottom = MoveTo::create(curtainInfo.delay, Vec2(curtainX, -curtainTop->getContentSize().height));
	auto seqTop = Sequence::create(moveTop,
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, curtainTop)),
		nullptr);
	auto seqBottom = Sequence::create(moveBottom,
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, curtainBottom)),

		nullptr);

	curtainTop->runAction(seqTop);
	curtainBottom->runAction(seqBottom);
}

void StageScene::updateCurtainText()
{
	//��ʶλ������������лĻ
	if (isTypeCurtainDone || curtainInfo.type != CurtainType::black)
		return;

	auto curtainBlack = getChildByName(nameCurtainBlack);
	if (nullptr == curtainBlack)
		return;
	auto lbCurtainText = dynamic_cast<Label*>(curtainBlack->getChildByName(nameCurtainLabel));
	if (nullptr == lbCurtainText)
		return;
	auto thread = dialogCurtain.getCurrentDialog();
	//lbCurtainText->setColor(thread.font.color);
	//lbCurtainText->setSystemFontSize(thread.font.size);
	lbCurtainText->setString(typeCurtain.getTyped());

	//����ı���ͷ�Ҵ��ֻ�����������˳�
	if (dialogCurtain.isDone() && typeCurtain.isDone(false))
	{
		isTypeCurtainDone = true;
		auto sequence = Sequence::create(
			DelayTime::create(2.5f),
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, lbCurtainText)),

			FadeOut::create(1.f),
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, curtainBlack)),
			nullptr);
		curtainBlack->runAction(sequence);
	}
}

bool StageScene::processCurtain()
{
	if (isTypeCurtainDone || curtainInfo.type != CurtainType::black)
		return true;

	Resh::playSound("sound/proceed.wav");
	//���ֻ�
	if (typeCurtain.isDone())
	{
		dialogCurtain.fetchNext();
		typeCurtain.reset();
		typeCurtain.setContent(dialogCurtain.getCurrentDialog().text);
	}

	return false;
}
