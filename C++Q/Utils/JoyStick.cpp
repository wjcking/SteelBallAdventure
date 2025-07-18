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
	//如果没有joypad则退出
	assert(joypads.isTable() && "请设置pads");
	//重置标识
	Joystick::nextTag = 1;
	for (auto iter = joypads.begin(); iter != joypads.end(); ++iter)
	{
		assert(iter.value().has(Luaf_Pos) && "请设置起始位置");
		assert(iter.value().has(Luaf_Name) && "没有名称");
		joypadNames[nextTag] = "";
		auto pos = iter.value()[Luaf_Pos].value<Vec2>();
		auto name = iter.value()[Luaf_Name].value<std::string>();
		Sprite* pad = nullptr;
		if (iter.value().has(Luaf_Frame))
			pad = Sprite::createWithSpriteFrameName(iter.value()[Luaf_Frame].value<std::string>());
		else
		{
			auto file = iter.value()[Luaf_File].value<std::string>();
			assert(iter.value().has(Luaf_File) && "没有图片文件属性");
			assert(FileUtils::getInstance()->isFileExist(file) && "没有发现控制器图片");
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

	//设置控制器
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

/*当前对象被加入到父类节点上会调用该方法*/
void Joystick::onEnter()
{
	/*首先调用父类的onEnter方法*/
	Layer::onEnter();

	/*开启单点触摸的监听, 可以用auto*/
	listener = EventListenerTouchAllAtOnce::create();

	/*可以使用lambda表达式来创建，但那样看起来太乱, 这里我们还是使用回调函数*/
	listener->onTouchesBegan = CC_CALLBACK_2(Joystick::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(Joystick::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(Joystick::onTouchesEnded, this);

	/*注册监听*/
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

}

/*当前对象从父类上移除时会调用该方法*/
void Joystick::onExit()
{
	/*从分发中心移除注册的监听*/
	Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
	/*最后调用父类的onExit()方法*/
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
		/*2. 创建摇杆*/
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
		{	/*当触摸开始的时候， 如果触摸点的位置和我们中心点位置的距离 < 圆的半径 我们才能Move*/
			/*获取圆心点和半径*/
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
		//如果不是手柄，则可见的pad
		for (auto tag = 1; tag < nextTag; tag++)
		{
			auto pad = dynamic_cast<Sprite*>(getChildByTag(tag));
			if (pad->isVisible() && inRange(touch->getLocation(), pad->getBoundingBox()))
			{
				//按下后图片
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
		//控制器
		if (touchedTag < NoControllerTouched)
		{
			//如果是操纵杆，回复原状
			if (joyStyle == JoystickStyle::rocker)
				rocker->setPosition(rockerBackground->getPosition());
			//图片恢复原状(前后按钮全部reset
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
		//0 func按钮
		for (auto tag = 1; tag < nextTag; tag++)
		{
			touchedTag = touchedTags[touch->getID()];
			if (touchedTag == tag)
			{
				//背景图片还原
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
	/*右方向*/
	if (angle >= -M_PI / 8.0 && angle <= M_PI / 8.0) {
		direction = MovingDirection::toRight;
	}
	/*右边走 -八分之派 到 八分之派*/
	if (angle >= -(M_PI / 8.0) && angle <= M_PI / 8.0) {
		direction = MovingDirection::toRight;
	}
	/*右上方向 八分之派 到 八分之三派*/
	else if (angle >= M_PI / 8.0 && angle < 3 * M_PI / 8.0) {
		direction = MovingDirection::toTopRight;
	}
	/*上方向 八分之三派 到 八分之五派*/
	else if (angle >= 3 * M_PI / 8.0 && angle <= 5 * M_PI / 8.0) {
		direction = MovingDirection::toTop;
	}
	/*左上方向 八分之5派 到 八分之七派*/
	else if (angle > 5 * M_PI / 8.0 && angle < 7 * M_PI / 8.0) {
		direction = MovingDirection::toTopLeft;
	}
	/*左方向*/
	else if ((angle >= 7 * M_PI / 8.0 && angle <= M_PI) || (angle <= -7 * M_PI / 8.0 && angle >= -M_PI)) {
		direction = MovingDirection::toLeft;
	}
	/*左下方向*/
	else if (angle > -7 * M_PI / 8.0 && angle < -5 * M_PI / 8.0) {
		direction = MovingDirection::toBottomLeft;
	}
	/*下方向*/
	else if (angle >= -5 * M_PI / 8.0 && angle <= -3 * M_PI / 8.0) {
		direction = MovingDirection::toBottom;
	}
	/*右下方向*/
	else if (angle > -3 * M_PI / 8.0 && angle < -M_PI / 8.0) {
		direction = MovingDirection::toBottomRight;
	}

}
void Joystick::roll(const Vec2& touch_pos)
{
	//隐藏了就不能用
	if (!rocker->isVisible())
		return;
	/*获取圆心点和半径*/
	/*当触摸移动的时候， 如果触摸点的位置和我们中心点位置的距离 < 圆的半径 */
	float radius = rockerBackground->getContentSize().width / 2;
	Vec2 center = rockerBackground->getPosition();
	/*获取触摸点位置*/

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
		/*如果在上半圆*/
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



