#include "JoyStick.h"
#include "GameScriptor.h"
#include "ResourceHelper.h"
USING_NS_CC;
using namespace ui;
short Joystick::nextTag = 1;
void Joystick::loadScript(const char* padTable)
{
	auto joystick = LUAH->getGlobal( "Joystick");

	if (joystick.isTable())
	{
		joyStyle = joystick.get(Luaf_Type).toValue<JoystickStyle>();

		joystick.set(Luaf_CppRef, this);
		LUAH->flush();
	}
	else
		joyStyle = JoystickStyle::rocker;

	auto joypads = LUAH->getGlobal(padTable);
	//���û��joypad���˳�
	assert(joypads.isTable() && "������pads");
	//���ñ�ʶ
	Joystick::nextTag = 1;
	for (auto iter = joypads.begin(); iter != joypads.end(); ++iter)
	{
		assert(iter.value().has(Luaf_Pos) && "��������ʼλ��");
		assert(iter.value().has(Luaf_Name) && "û������");
		joypadNames[nextTag] = "";
		auto pos = iter.value()[Luaf_Pos].value<Vec2>();
		auto name = iter.value()[Luaf_Name].value<std::string>();
		Sprite* pad = nullptr;
		if (iter.value().has(Luaf_Frame))
			pad = Sprite::createWithSpriteFrameName(iter.value()[Luaf_Frame].value<std::string>());
		else
		{
			auto file = iter.value()[Luaf_File].value<std::string>();
			assert(iter.value().has(Luaf_File) && "û��ͼƬ�ļ�����");
			assert(FileUtils::getInstance()->isFileExist(file) && "û�з��ֿ�����ͼƬ");
			pad = Sprite::create(file);
		}
		pad->setName(name);
		pad->setPosition(pos);
		pad->setTag(nextTag++);
		if (iter.value().has(Luaf_Visible))
		{
			auto visible = iter.value()[Luaf_Visible].value<bool>();
			pad->setVisible(visible);
		}
		if (iter.value().has(Luaf_Text))
		{
			FontInfo fi(iter.value());
			/*if (iter.value().has(Luaf_Font))
				fi = LUAH->getFont(iter.value()[Luaf_Font]);*/
			auto label = Label::createWithSystemFont(iter.value()[Luaf_Text].value<std::string>(), Resh::getFontName(), fi.size);
			label->setColor(fi.color);
			label->setName("lb" + name);
			label->setPosition(pad->getContentSize().width / 2.f, pad->getContentSize().height / 2.f);
			pad->addChild(label);
		}
		addChild(pad, Z_JoyStick);
	}

	//���ÿ�����
	setJoystickStyle();
}
bool Joystick::init()
{
	if (!Layer::init())
	{
		return false;
	}

	joypadNames.reserve(20);
	touchedTags.reserve(20);
	return true;
}
void Joystick::callbackFunc(cocos2d::Ref *target, SEL_CallFuncN callfun)
{
	callbackListener = target;
	callback = callfun;
}

void Joystick::setAllVisible(const bool& flag)
{
	for (auto tag = 1; tag < nextTag; tag++)
	{
		auto pad = getChildByTag(tag);
		pad->setVisible(flag);
	}
}

/*��ǰ���󱻼��뵽����ڵ��ϻ���ø÷���*/
void Joystick::onEnter()
{
	/*���ȵ��ø����onEnter����*/
	Layer::onEnter();

	/*�������㴥���ļ���, ������auto*/
	listener = EventListenerTouchAllAtOnce::create();

	/*����ʹ��lambda����ʽ��������������������̫��, �������ǻ���ʹ�ûص�����*/
	listener->onTouchesBegan = CC_CALLBACK_2(Joystick::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(Joystick::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(Joystick::onTouchesEnded, this);

	/*ע�����*/
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

}

/*��ǰ����Ӹ������Ƴ�ʱ����ø÷���*/
void Joystick::onExit()
{
	/*�ӷַ������Ƴ�ע��ļ���*/
	Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
	/*�����ø����onExit()����*/
	Layer::onExit();
}

void Joystick::setJoystickStyle()
{
	switch (joyStyle)
	{
	case JoystickStyle::arrow:
	case JoystickStyle::arrowForward:
	case JoystickStyle::arrowBackward:
		padForward = Sprite::createWithSpriteFrameName("forward.png");
		padBackward = Sprite::createWithSpriteFrameName("backward.png");

		padBackward->setPosition(padBackward->getContentSize().width - 20, 45);
		padForward->setPosition(padForward->getContentSize().width * 2 - 10, 45);

		padForward->setName("forward");
		padBackward->setName("backward");

		padForward->setTag(-1);
		padBackward->setTag(-2);

		addChild(padForward, Z_JoyStick);
		addChild(padBackward, Z_JoyStick);

		if (JoystickStyle::arrowBackward == joyStyle)
			padForward->setVisible(false);
		else if (JoystickStyle::arrowForward == joyStyle)
			padBackward->setVisible(false);
		break;
	case JoystickStyle::rocker:
		rockerBackground = Sprite::create("control/joystickbg.png");
		rockerBackground->setPosition(Vec2(125, 100));
		rockerBackground->setTag(-1);
		addChild(rockerBackground, Z_JoyStick);
		/*2. ����ҡ��*/
		rocker = Sprite::create("control/joystick.png");
		rocker->setPosition(Vec2(125, 100));
		rocker->setTag(-2);
		addChild(rocker, Z_JoyStick);
		break;
	default:
		break;
	}
}
void Joystick::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event * event)
{
	for (auto touch : touches)
	{
		if (joyStyle == JoystickStyle::arrow || joyStyle == JoystickStyle::arrowForward || joyStyle == JoystickStyle::arrowBackward)
		{
			touchedTags[touch->getID()] = pad(touch->getLocation());
			if (touchedTags[touch->getID()] < NoControllerTouched)
				LUAH->callback(Luap_Joystick, "begin");
		}
		else if (joyStyle == JoystickStyle::rocker)
		{	/*��������ʼ��ʱ�� ����������λ�ú��������ĵ�λ�õľ��� < Բ�İ뾶 ���ǲ���Move*/
			/*��ȡԲ�ĵ�Ͱ뾶*/
			float radius = rockerBackground->getContentSize().width / 2;
			Vec2 center = rockerBackground->getPosition();

			if (touch->getLocation().distance(center) <= radius)
			{
				touchedTags[touch->getID()] = rocker->getTag();
				rocker->setPosition(Vec2(touch->getLocation()));
				roll(touch->getLocation());
				LUAH->callback(Luap_Joystick, "begin");
			}
		}
		//��������ֱ�����ɼ���pad
		for (auto tag = 1; tag < nextTag; tag++)
		{
			auto pad = dynamic_cast<Sprite*>(getChildByTag(tag));
			if (pad->isVisible() && inRange(touch->getLocation(), pad->getBoundingBox()))
			{
				//���º�ͼƬ
				auto pressedFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(pad->getName() + "2.png");
				if (pressedFrame != nullptr)
					pad->setSpriteFrame(pressedFrame);
				touchedTags[touch->getID()] = tag;
				joypadNames[tag] = pad->getName();
				LUAH->callback(Luap_Joypad, Luap_Joypad);
			}
		}
	}


}
void Joystick::onButtonClicked(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{
	//auto button = static_cast<Button*>(sender);
	//buttonName = StringUtils::format("%s", button->getName().c_str());
}
void Joystick::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event * event)
{
	for (auto touch : touches)
	{
		if (touchedTags[touch->getID()] < NoControllerTouched)
		{
			if (joyStyle >= JoystickStyle::arrow)
				pad(touch->getLocation());
			else if (joyStyle == JoystickStyle::rocker)
			{
				touchedTags[touch->getID()] = rocker->getTag();
				roll(touch->getLocation());
			}
		}
	}
	LUAH->callback(Luap_Joystick, "moved");


}
void Joystick::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event * event)
{
	auto touchedTag = 0;
	for (auto touch : touches)
	{
		touchedTag = touchedTags[touch->getID()];
		//������
		if (touchedTag < NoControllerTouched)
		{
			//����ǲ��ݸˣ��ظ�ԭ״
			if (joyStyle == JoystickStyle::rocker)
				rocker->setPosition(rockerBackground->getPosition());
			//ͼƬ�ָ�ԭ״(ǰ��ťȫ��reset
			if (joyStyle != JoystickStyle::rocker)
			{
				auto pad = dynamic_cast<Sprite*>(getChildByName("forward"));
				SpriteFrame* originFrame = nullptr;
				if (pad != nullptr)
				{
					originFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("forward.png");
					if (nullptr != originFrame)
						pad->setSpriteFrame(originFrame);
				}
				pad = dynamic_cast<Sprite*>(getChildByName("backward"));
				if (pad != nullptr)
				{
					originFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("backward.png");
					if (nullptr != originFrame)
						pad->setSpriteFrame(originFrame);
				}
			}
			direction = MovingDirection::stayStill;
			touchedTags.erase(touch->getID());
			LUAH->callback(Luap_Joystick, "ended");
		}
		//0 func��ť
		for (auto tag = 1; tag < nextTag; tag++)
		{
			touchedTag = touchedTags[touch->getID()];
			if (touchedTag == tag)
			{
				//����ͼƬ��ԭ
				auto pad = dynamic_cast<Sprite*>(getChildByTag(touchedTag));
				if (pad != nullptr)
				{
					auto originFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(pad->getName() + ".png");
					if (nullptr != originFrame)
						pad->setSpriteFrame(originFrame);
				}
				touchedTags.erase(touch->getID());
				joypadNames[tag] = "";
				LUAH->callback(Luap_Joypad, Luap_Joypad);
			}
		}
	}
}

Sprite* Joystick::getRocker()
{
	return rocker;
}
Sprite* Joystick::getRockerBg()
{
	return rockerBackground;
}

float Joystick::getAngle()
{
	return angle;
}
MovingDirection& Joystick::getDirection()
{
	return direction;
}

void Joystick::checkDirection(float angle)
{
	/*�ҷ���*/
	if (angle >= -M_PI / 8.0 && angle <= M_PI / 8.0) {
		direction = MovingDirection::toRight;
	}
	/*�ұ��� -�˷�֮�� �� �˷�֮��*/
	if (angle >= -(M_PI / 8.0) && angle <= M_PI / 8.0) {
		direction = MovingDirection::toRight;
	}
	/*���Ϸ��� �˷�֮�� �� �˷�֮����*/
	else if (angle >= M_PI / 8.0 && angle < 3 * M_PI / 8.0) {
		direction = MovingDirection::toTopRight;
	}
	/*�Ϸ��� �˷�֮���� �� �˷�֮����*/
	else if (angle >= 3 * M_PI / 8.0 && angle <= 5 * M_PI / 8.0) {
		direction = MovingDirection::toTop;
	}
	/*���Ϸ��� �˷�֮5�� �� �˷�֮����*/
	else if (angle > 5 * M_PI / 8.0 && angle < 7 * M_PI / 8.0) {
		direction = MovingDirection::toTopLeft;
	}
	/*����*/
	else if ((angle >= 7 * M_PI / 8.0 && angle <= M_PI) || (angle <= -7 * M_PI / 8.0 && angle >= -M_PI)) {
		direction = MovingDirection::toLeft;
	}
	/*���·���*/
	else if (angle > -7 * M_PI / 8.0 && angle < -5 * M_PI / 8.0) {
		direction = MovingDirection::toBottomLeft;
	}
	/*�·���*/
	else if (angle >= -5 * M_PI / 8.0 && angle <= -3 * M_PI / 8.0) {
		direction = MovingDirection::toBottom;
	}
	/*���·���*/
	else if (angle > -3 * M_PI / 8.0 && angle < -M_PI / 8.0) {
		direction = MovingDirection::toBottomRight;
	}

}
void Joystick::roll(const Vec2& touch_pos)
{
	//�����˾Ͳ�����
	if (!rocker->isVisible())
		return;
	/*��ȡԲ�ĵ�Ͱ뾶*/
	/*�������ƶ���ʱ�� ����������λ�ú��������ĵ�λ�õľ��� < Բ�İ뾶 */
	float radius = rockerBackground->getContentSize().width / 2;
	Vec2 center = rockerBackground->getPosition();
	/*��ȡ������λ��*/

	float dis = touch_pos.distance(center);
	angle = acos((touch_pos.x - center.x) / dis);
	if (touch_pos.y > center.y)
	{
		checkDirection(angle);
	}
	else {
		checkDirection(-angle);
	}

	if (dis <= radius) {
		rocker->setPosition(Vec2(touch_pos));
	}
	else
	{
		/*������ϰ�Բ*/
		if (touch_pos.y > center.y) {
			rocker->setPosition(Vec2(center.x + radius * cos(angle), center.y + radius * sin(angle)));
		}
		else {
			rocker->setPosition(Vec2(center.x + radius * cos(angle), center.y - radius * sin(angle)));
		}
	}
}
short Joystick::pad(const Vec2 & posInView)
{
	short arrowID = 0;
	if (padForward->isVisible() && inRange(posInView, padForward->getBoundingBox()))
	{
		arrowID = padForward->getTag();
		auto pressedFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("forward2.png");
		if (pressedFrame != nullptr)
			padForward->setSpriteFrame(pressedFrame);
		direction = MovingDirection::toRight;
	}

	if (padBackward->isVisible() && inRange(posInView, padBackward->getBoundingBox()))
	{
		auto pressedFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("backward2.png");
		if (pressedFrame != nullptr)
			padBackward->setSpriteFrame(pressedFrame);
		arrowID = padBackward->getTag();
		direction = MovingDirection::toLeft;
	}

	return arrowID;
}

void Joystick::setString(const std::string & name, const char * text)
{
	auto pad = getChildByName(name);
	if (nullptr == pad)
		return;
	auto label = pad->getChildByName("lb" + name);

	if (nullptr != label)
	{
		dynamic_cast<Label*>(label)->setString(text);
	}
}

bool Joystick::getTouchedPad(const short & tag)
{
	return joypadNames[tag] != "";
}

LuaIntf::LuaRef Joystick::getTouches()
{
	auto table = LUAH->createTable();
	short index = 1;
	for (auto pad : joypadNames)
		if (pad.second != "")
			table.set(index++, pad.second);

	return table;
}



