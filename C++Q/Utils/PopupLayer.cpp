#include "PopupLayer.h"
#include "Constant.h"
#include "../Map/CheckPoint.h"
#include "ResourceHelper.h"
#include "../Role/RObject.h"
#include "../Role/RoleManager.h"
#include "Utils.h"
short PopupLayer::nextTag = 1;

PopupLayer::PopupLayer():isPrompted(true)
	,callbackListener(nullptr)
	, callback(nullptr)

{
}

PopupLayer::~PopupLayer()
{
}

bool PopupLayer::init() {
	if (!Layer::init())
		return false;

	//单点触摸
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(PopupLayer::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(PopupLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
 
	//注册Luaf_Popup
	LUAH->registerRef(Luaf_Popup, this);
	return true;
}


bool PopupLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *pEvent)
{
	beginDelta = background->convertTouchToNodeSpace(touch);
	touchedBeginID = touch->getID(); 
	return true;
}

void PopupLayer::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{

}
void PopupLayer::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	endDelta = background->convertTouchToNodeSpace(touch);
	touchedEndID = touch->getID();
	LUAH->callback("popuplayer", "ended");
}

void PopupLayer::callbackFunc(cocos2d::Ref *target, SEL_CallFuncN callfun)
{
	callbackListener = target;
	callback = callfun;
}

void PopupLayer::autoClose(const float& delaySec)
{
	if (this == nullptr)
		return;

	runAction(Sequence::create(CallFunc::create([&](){
		isPrompted = false;}),
		DelayTime::create(delaySec),
		CallFunc::create([&]() {
		if (nullptr != overlay)
			overlay->removeFromParent();
		removeFromParent();
		
	}), nullptr));

}

void PopupLayer::setString(const std::string &name, const std::string & text)
{
	auto node = background->getChildByName(name);

	if (nullptr != node)
	{
		auto label = dynamic_cast<Label*>(node);
	
		label->setString(text);
	}
	else
	{

		node = menuButton->getChildByName(name);
		if (nullptr != node)
		{
	auto label = dynamic_cast<Label*>(node);

		label->setString(text);
		}
	
	}

}
void PopupLayer::setButtonName(const std::string &name, const std::string & newName)
{
	auto node = menuButton->getChildByName(name);

	if (nullptr != node)
		node->setName(newName);
}
void PopupLayer::setButtonVisible(const std::string &name, const bool & flag)
{
	auto node = menuButton->getChildByName(name);

	if (nullptr != node)
		node->setVisible(flag);
}
void PopupLayer::displace(const std::string &name, const  Vec2 & pos)
{
	auto node = background->getChildByName(name);

	if (nullptr != node)
		node->setPosition(pos);
}

bool PopupLayer::loadScript(const std::string& tableName)
{
	//如果没有joypad则退出
	auto promptTable = LUAH->getGlobal(tableName.c_str());

	return loadScript(promptTable);
}
bool PopupLayer::loadScript(const LuaIntf::LuaRef& ref)
{
	//如果没有joypad则退出
	if (!ref.isTable())
	{
		CCLOG("[PopupLayer:loadScript]please set prompt table");
		return false;
	}

	setName(ref.get(Luaf_Name).toValue<std::string>());
	assert(ref.has(Luaf_Background) && "必须设置 background 字段");
	auto bgField = ref.get(Luaf_Background);

	assert(bgField.has(Luaf_File) && "请设置背景图像文件");
	auto backgroundFile = bgField.get(Luaf_File).toValue<std::string>();

	assert(FileUtils::getInstance()->isFileExist(backgroundFile) && "backgroundFile不存在");
	background = Sprite::create(backgroundFile);

	bAutoClose = ref.get(Luaf_AutoClose, -1.f);
	bCovered = ref.get(Luaf_IsCovered, false);
	// 初始化需要的 Menu
	 menuButton = Menu::create();

	menuButton->setPosition(Vec2::ZERO);
	background->addChild(menuButton);
	auto pos = bgField.get(Luaf_Pos, Vec2(ScreenSize.width / 2, ScreenSize.height / 2));
	background->setPosition(pos);
	addChild(background);
	setLocalZOrder(ref.get(Luaf_ZOrder, Z_PopupLayer));
	//播放弹出的音乐
	if (bgField.has(Luaf_Sound))
		Resh::playSound(bgField.get(Luaf_Sound).toValue<const char*>());
	//重置标识
	PopupLayer::nextTag = 1;

	if (ref.has(Luaf_items))
	{
		//按钮或文本
		auto itemsField = ref.get(Luaf_items);
		LuaRef ref;
		for (auto iter = itemsField.begin(); iter != itemsField.end(); ++iter)
		{
			ref = iter.value();
			assert(ref.has(Luaf_Name) && "没有名称");
			auto pos = ref.has(Luaf_Pos) ? ref[Luaf_Pos].value<Vec2>() : Vec2::ZERO;
			auto name = ref[Luaf_Name].value<std::string>();
			//如果是文本

			auto text = ref.has(Luaf_Text) ? ref[Luaf_Text].value<std::string>() : "";

			FontInfo font(ref);
			auto type = ref.has(Luaf_Type) ? ref[Luaf_Type].value<short>() : 0;
			if (type == static_cast<short>(ItemType::menu))
			{
				if (ref.has(Luaf_File))
				{
					auto file = ref[Luaf_File].value<std::string>();
					assert(FileUtils::getInstance()->isFileExist(file) && "没有发现控制器图片");
	 
					auto  selectedFile = getSelectedFile(file);
					//ios必须判断
					if (!FileUtils::getInstance()->isFileExist(selectedFile))
						selectedFile = file;
					// 创建图片菜单按钮
					auto menuImage = MenuItemImage::create(file, selectedFile, selectedFile, CC_CALLBACK_1(PopupLayer::menuCallback,this));
					menuImage->setName(name);
					if (ref.has(Luaf_Visible))
					 menuImage->setVisible(ref.get(Luaf_Visible,true));
					menuImage->setPosition(pos);
			
					// 添加文字说明并设置位置 
					auto lbTitle = Label::createWithSystemFont(text, Resh::getFontName(), font.size);
					lbTitle->setName("lbButton");
					lbTitle->setColor(font.color);
					lbTitle->setSystemFontSize(font.size);
					lbTitle->setPosition(Vec2(menuImage->getContentSize().width / 2, menuImage->getContentSize().height / 2));
					menuImage->addChild(lbTitle);
					menuButton->addChild(menuImage);
				}
				else
				{
					auto label = Label::createWithSystemFont(text, Resh::getFontName(), font.size);
					label->setColor(font.color);
					label->setSystemFontSize(font.size);
					label->setName(name);
					label->setTag(nextTag++);
					label->setPosition(pos);
					background->addChild(label);
				}

			}
			else if (type == static_cast<short>(ItemType::robject))
			{
				Role* role = nullptr;
				if (ref.has(Luaf_Frame))
				{
					auto frame = ref[Luaf_Frame].value<std::string>();
					role = Role::createWithFrameName<RObject>(frame);
				}
				else if (ref.has(Luaf_File))
				{
					auto  file = ref[Luaf_File].value<std::string>();
					role = Role::createWithFileName<RObject>(file);

				}
				if (role != nullptr)
				{
					role->setName(name);
					role->setPosition(pos);
					RoleManager::setProperty(ref, role);
					background->addChild(role);
				}
			}
		}
	}

	//检查wangba
	bSmack = Checkpoint::life == 0;
	return true;
}
RObject* PopupLayer::getChild(const char * name)
{
	return dynamic_cast<RObject*>(background->getChildByName(name));
}
void PopupLayer::menuCallback(cocos2d::Ref *pSender)
{
	auto menuItem = dynamic_cast<MenuItemImage*>(pSender);
	//stage 发送过来的指令
	if (callback && callbackListener)
		(callbackListener->*callback)(menuItem);
	//然后是
	if ("cancel" == menuItem->getName() || "retry" == menuItem->getName() )
	{
		//if (nullptr != overlay && )
		//	overlay->removeFromParent();
		//从父对象中移除
		//removeFromParent();
		return;
	}
}

void PopupLayer::onEnter()
{
	Layer::onEnter();

	// 弹出效果
	//auto popupAction = Sequence::create(ScaleTo::create(0.0f, 0.0f),
	//	ScaleTo::create(0.06f, 1.05f),
	//	ScaleTo::create(0.08f, 0.95f),
	//	ScaleTo::create(0.08f, 1.0f), nullptr);
	if (nullptr != enterAction)
		runAction(enterAction);
}

void PopupLayer::onExit()
{
	Layer::onExit();
}
/*遮罩层 屏蔽向下触摸*/
bool Overlay::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, rgbA)))
		return false;
	//创建监听器
	auto listener = EventListenerTouchOneByOne::create();
	//屏蔽向下触摸
	listener->onTouchBegan = [](Touch *, Event *)
	{
		return true;
	};
	//监听器状态设置为吞并触摸消息
	listener->setSwallowTouches(true);
	//将监听器与当前节点建立关系并注册到事件分配器
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}
Overlay* Overlay::createByOpaque(const GLubyte & value)
{
	auto pRet = new(std::nothrow) Overlay();
	pRet->rgbA = value;
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;

	}
	return nullptr;
}
