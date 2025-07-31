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

	//���㴥��
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(PopupLayer::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(PopupLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
 
	//ע��Luaf_Popup
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
	//���û��joypad���˳�
	auto promptTable = LUAH->getGlobal(tableName.c_str());

	return loadScript(promptTable);
}
bool PopupLayer::loadScript(const LuaIntf::LuaRef& ref)
{
	//���û��joypad���˳�
	if (!ref.isTable())
	{
		CCLOG("[PopupLayer:loadScript]please set prompt table");
		return false;
	}

	setName(ref.get(Luaf_Name).toValue<std::string>());
	assert(ref.has(Luaf_Background) && "�������� background �ֶ�");
	auto bgField = ref.get(Luaf_Background);

	assert(bgField.has(Luaf_File) && "�����ñ���ͼ���ļ�");
	auto backgroundFile = bgField.get(Luaf_File).toValue<std::string>();

	assert(FileUtils::getInstance()->isFileExist(backgroundFile) && "backgroundFile������");
	background = Sprite::create(backgroundFile);

	bAutoClose = ref.get(Luaf_AutoClose, -1.f);
	bCovered = ref.get(Luaf_IsCovered, false);
	// ��ʼ����Ҫ�� Menu
	 menuButton = Menu::create();

	menuButton->setPosition(Vec2::ZERO);
	background->addChild(menuButton);
	auto pos = bgField.get(Luaf_Pos, Vec2(ScreenSize.width / 2, ScreenSize.height / 2));
	background->setPosition(pos);
	addChild(background);
	setLocalZOrder(ref.get(Luaf_ZOrder, Z_PopupLayer));
	//���ŵ���������
	if (bgField.has(Luaf_Sound))
		Resh::playSound(bgField.get(Luaf_Sound).toValue<const char*>());
	//���ñ�ʶ
	PopupLayer::nextTag = 1;

	if (ref.has(Luaf_items))
	{
		//��ť���ı�
		auto itemsField = ref.get(Luaf_items);
		LuaRef ref;
		for (auto iter = itemsField.begin(); iter != itemsField.end(); ++iter)
		{
			ref = iter.value();
			assert(ref.has(Luaf_Name) && "û������");
			auto pos = ref.has(Luaf_Pos) ? ref[Luaf_Pos].value<Vec2>() : Vec2::ZERO;
			auto name = ref[Luaf_Name].value<std::string>();
			//������ı�

			auto text = ref.has(Luaf_Text) ? ref[Luaf_Text].value<std::string>() : "";

			FontInfo font(ref);
			auto type = ref.has(Luaf_Type) ? ref[Luaf_Type].value<short>() : 0;
			if (type == static_cast<short>(ItemType::menu))
			{
				if (ref.has(Luaf_File))
				{
					auto file = ref[Luaf_File].value<std::string>();
					assert(FileUtils::getInstance()->isFileExist(file) && "û�з��ֿ�����ͼƬ");
	 
					auto  selectedFile = getSelectedFile(file);
					//ios�����ж�
					if (!FileUtils::getInstance()->isFileExist(selectedFile))
						selectedFile = file;
					// ����ͼƬ�˵���ť
					auto menuImage = MenuItemImage::create(file, selectedFile, selectedFile, CC_CALLBACK_1(PopupLayer::menuCallback,this));
					menuImage->setName(name);
					if (ref.has(Luaf_Visible))
					 menuImage->setVisible(ref.get(Luaf_Visible,true));
					menuImage->setPosition(pos);
			
					// ��������˵��������λ�� 
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

	//���wangba
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
	//stage ���͹�����ָ��
	if (callback && callbackListener)
		(callbackListener->*callback)(menuItem);
	//Ȼ����
	if ("cancel" == menuItem->getName() || "retry" == menuItem->getName() )
	{
		//if (nullptr != overlay && )
		//	overlay->removeFromParent();
		//�Ӹ��������Ƴ�
		//removeFromParent();
		return;
	}
}

void PopupLayer::onEnter()
{
	Layer::onEnter();

	// ����Ч��
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
/*���ֲ� �������´���*/
bool Overlay::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, rgbA)))
		return false;
	//����������
	auto listener = EventListenerTouchOneByOne::create();
	//�������´���
	listener->onTouchBegan = [](Touch *, Event *)
	{
		return true;
	};
	//������״̬����Ϊ�̲�������Ϣ
	listener->setSwallowTouches(true);
	//���������뵱ǰ�ڵ㽨����ϵ��ע�ᵽ�¼�������
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
