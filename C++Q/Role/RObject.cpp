#include "RObject.h" 

#include "../Utils/ResourceHelper.h"
#include "../Utils/ActionExtend.h"
#include "SimpleAudioEngine.h"
#include "RoleManager.h"
#include "../Map/TiledMap.h"
#include "../Map/MapManager.h"
using namespace CocosDenshion;
RObject::RObject() :Role(), debrisColor(1), springInfo()
{
	isGravityOn = false;
	isSolid = true;

	type = RoleType::robject;
	moves.reserve(100);
	knocks.reserve(100);
	frameSwitch.reserve(5);
}
RObject::~RObject()
{

}

void RObject::updateSpring()
{
	if (!springInfo.isBounced && !springInfo.delaySpring.isTimeUp())
		return;

	//auto frame = springInfo.getFrame(springInfo.framePrefix);
	//if (nullptr == frame)
	//{
	//	log("[RObject:updateSpring]no frame found");
	//	return;
	//}
	////����ˢ֡
	//springInfo.delaySpring.reset();
	////���ɵĵ�һ֡�����һ֡ͼƬһģһ��
	//if (springInfo.isEnded())
		springInfo.isBounced = false;

	//setSpriteFrame(frame);
	////�ŵ�������++����0�ж��Ƿ�����frameindexes
	//springInfo.fetchNext();
}
//ע��ű��Ƿ���ˢ��λ�ã������λ�û�ˢ��һ��
void RObject::update()
{
	updateSpring();
	Role::update();
}
//framework
void RObject::loadScript()
{
	Role::loadScript();
}

KnockPhase RObject::gotKnocked(Role& collider)
{
	auto currentKnock = KnockPhase();
	auto jumpingTo = getActionByTag(1000);
	if (jumpingTo != nullptr && !jumpingTo->isDone())
		return currentKnock;

	auto iterKnocked = knocks.begin();
	//����û��ű��Ѿ���ͷ
	if (iterKnocked == knocks.end() || iterKnocked->isColliding)
		return currentKnock;

	Action* knockAction = nullptr;

	auto callEnd = CallFunc::create([&]() {
		//iterKnocked �����жϲ�����nullptr
		//if ( !iterKnocked->allowNext())
		//{
		auto tileFrame = MAP_WITHTAG(mapTag)->getFrameWithTile(Vec2(0, 3));
		if (nullptr != tileFrame)
			setSpriteFrame(tileFrame);
		//}
	});

	auto jumpTo = JumpTo::create(0.2f, Vec2(originBound.getMidX(), originBound.getMidY()), 15.f, 1);

	if (iterKnocked->colliderDirection == CollisionDirection::atBottom)
		knockAction = Sequence::create(jumpTo, callEnd, nullptr);
	else
		knockAction = Sequence::create(jumpTo, nullptr); //Sequence::create(Blink::create(0.2f, 1), callEnd, nullptr);
	knockAction->setTag(1000);
	switch (iterKnocked->knockAction)
	{
		//ֱ�ӿ��Դ���ȥ���corruptʹ��
	case KnockAction::hollow:
		if (iterKnocked->colliderDirection == collider.getCollidedDirection() || iterKnocked->colliderDirection == CollisionDirection::intersected)
			iterKnocked->isRemovable = true;
		break;
		//����ש��
	case KnockAction::hidden:
		if (iterKnocked->colliderDirection == collider.getCollidedDirection())
		{
			iterKnocked->isColliding = true;
			if (!isVisible())
			{
				iterKnocked->times++;

				allowCollision();
				setVisible(true);
				//updatePosition();
				SimpleAudioEngine::getInstance()->playEffect(iterKnocked->sound.c_str());
				iterKnocked->isRemovable = true;

				setAnimation(iterKnocked->toast);
				runAction(knockAction);
			}
		}
		break;
	case KnockAction::fetchChar:
		//���������û�����
		if (iterKnocked->colliderDirection == collider.getCollidedDirection() && !iterKnocked->isColliding)
		{
			iterKnocked->times++;
			iterKnocked->isColliding = true;

			SimpleAudioEngine::getInstance()->playEffect(iterKnocked->sound.c_str());

			if (nullptr == label)
				return currentKnock;
			labelPhase.getNextChar();
			label->setString(labelPhase.getString());
			runAction(Sequence::create(jumpTo, nullptr));
		}
		break;
		//���������û�����������ʲô�ڽű�
	case KnockAction::popup:
		if (iterKnocked->colliderDirection == collider.getCollidedDirection())
		{
			if (iterKnocked->allowNext())
			{
				setAnimation(iterKnocked->toast);
				iterKnocked->times++;
				SimpleAudioEngine::getInstance()->playEffect(iterKnocked->sound.c_str());
				runAction(jumpTo);
			}
			else
			{
				iterKnocked->isRemovable = true;
				auto tileFrame = MAP_WITHTAG(mapTag)->getFrameWithTile(iterKnocked->replacedTile);
				if (nullptr != tileFrame)
					setSpriteFrame(tileFrame);
			}

		}
		break;
	case KnockAction::corrupt:
		if (iterKnocked->colliderDirection == collider.getCollidedDirection() || iterKnocked->colliderDirection == CollisionDirection::intersected)
			iterKnocked->isColliding = true;
		break;
	default:
		break;
	}
	currentKnock = *iterKnocked;

	if (iterKnocked->isRemovable)
		iterKnocked = knocks.erase(iterKnocked);

	return currentKnock;
}

//�ڽű���ˢִ֡��
void RObject::checkKnocks(Role& opponent)
{
	auto offset = Vec2(getCollisionBound().size.width / 2, getCollisionBound().size.height / 2);
	//	auto isOff = isGettingOff(opponent, Vec2(getCollisionBound().size.width / 2, getCollisionBound().size.height / 2));
	bool isOff = abs(originBound.getMidX() - opponent.getPositionX()) > getCollisionBound().size.width + offset.x
		|| abs(originBound.getMidX() - opponent.getCollisionBound().getMinY()) > getCollisionBound().size.height + offset.y;

	//������ײ״̬
	for (auto& knock : knocks)
	{
		//������������������
		if (knock.knockAction == KnockAction::corrupt)
			break;

		if (isOff)
			knock.isColliding = false;
	}

	auto iterKnock = knocks.begin();

	if (iterKnock != knocks.end())
	{
		switch (iterKnock->knockAction)
		{
		case KnockAction::corrupt:
			//�ӳ�����
			if (iterKnock->isColliding && iterKnock->isTimeUp())
			{
				ActionExtend::corrupt(originMapPosition, debrisColor);
				SimpleAudioEngine::getInstance()->playEffect(iterKnock->sound.c_str());
				setDisposal(true);
				iterKnock->isRemovable = true;
			}
			break;
		default:
			break;
		}
		//��ש��һ����������Ժ���������Ƴ���Ӷ������Ƴ�
	/*	if (iterKnock->isRemovable)
			iterKnock = knocks.erase(iterKnock);*/
	}
}

SpringInfo& RObject::bounce(Role& opponent)
{
	if (!checkObjectCollision(opponent))
		return springInfo;

	opponent.checkObjectSide();
	opponent.checkFollowing();
	//û�в��ڵ�������
	if (opponent.getCollidedDirection() != springInfo.direction)
		return springInfo;

	if (springInfo.direction == CollisionDirection::atTop)
	{
		opponent.setJumpForce(springInfo.force);
		opponent.setJumpCut(springInfo.force);
		opponent.setOnJump(true);
	}
	else if (springInfo.direction == CollisionDirection::atBottom)
		opponent.setGravity(springInfo.force);

	springInfo.isBounced = true;
	springInfo.times++;
	//����ˢ֡
	springInfo.delaySpring.reset();
	//����Ѿ���ʼ��frame ��ʲô������
	//if (springInfo.index > 0)
	//	return;

	//��frame�������������
	//springInfo = frameIndexes[static_cast<short>(FrameStyle::idle)];
	return springInfo;
}

void RObject::registerSpring(const LuaRef& ref)
{
	if (ref.has(Luaf_Name))
		springInfo.framePrefix = ref.get(Luaf_Name).toValue<const char*>();
	if (ref.has(Luaf_JumpForce))
		springInfo.force = ref.get(Luaf_JumpForce).toValue<Vec2>();
	if (ref.has(Luaf_Direction))
		springInfo.direction = ref.get(Luaf_Direction).toValue<CollisionDirection>();
}

void RObject::setAnimation(const LuaRef& ref)
{
	if (!ref.isValid() || !ref.has(Luaf_Type))
		return;
	auto type = ref.get(Luaf_Type).toValue<Anistyle>();

	if (Anistyle::rotate == type)
	{
		auto limitedTimes = ref.get(Luaf_limitedTimes, -1);
		auto angle = ref.get(Luaf_Angle, 365);
		//�����ص�
		isGravityOn = false;
		if (ref.has(Luaf_Anchor))
		{
			auto anchor = ref.get(Luaf_Anchor).toValue<Vec2>();
			setAnchorPoint(anchor);
		}
		auto duration = ref.get(Luaf_Duration, 1.0f / 60.f);
		if (limitedTimes < 0)
			runAction(RepeatForever::create(RotateBy::create(duration, angle)));
		else
			runAction(Repeat::create(RotateBy::create(duration, angle), limitedTimes));
	}

	Role::setAnimation(ref);
}

std::string RObject::pollChar()
{
	if (nullptr == label)	return "";
	char result[2];
	sprintf(result, "%c", labelPhase.poll());
	label->setString(result);
	return result;
}
bool RObject::checkObb(Role& opponent, const bool& allowFollow)
{
	if (getObb().isCollidWithOBB(opponent.getObb(false)))
	{
		return true;
		//�����ʵ��Ļ��������վ������
		//auto collision = MAKE_VECTOR2(pt1.x - pt2.x, pt1.y - pt2.y);
		//Vec2Normalize(collision, collision);
		//Vec2Mul(collision, collision, 2.8);
		//m_vx1 = collision.x;
		//m_vy1 = collision.y;
		//m_vx2 = -collision.x;
		//m_vy2 = -collision.y;
	}
	return false;
}


BlinkStatus RObject::blinkObject()
{
	if (blink.isShown)
	{
		//���˶����������
		if (Clock::getTickFloat() > blink.strampHidden)
		{
			setVisible(false);
			setIsSolid(false);
			if (!blink.isDone)
			{
				blink.isDone = true;
				return BlinkStatus::blinkDoneHidden;
			}
		}
		return BlinkStatus::blinkWaiting;
	}
	//ʱ����ж϶�����������һ�׶�
	if (blink.isTimeUp())
	{
		spawn(blink.spawnPosition);
		setVisible(true);
		setIsSolid(true);
		//ʱ��� ��ʼ�� �����������
		if (blink.strampHidden == 0.f)
			blink.strampHidden = Clock::getTickFloat() + blink.visibleDuration;

		blink.isShown = true;
		return BlinkStatus::blinkShown;
	}

	return BlinkStatus::blinkInit;
}

void RObject::blinkSelf()
{
	auto bs = blinkObject();
	switch (bs)
	{
	case blinkDoneHidden:
		blink.reset();
		break;
	case blinkShown:
		break;
	case blinkWaiting:
		break;
	case blinkInit:
		break;
	}
}

void RObject::blinkGroup()
{
	//���û������ִ��
	auto blinkInfo = BlinkSystem::getBlinkInfo(blink.groupID);
	if (!blinkInfo.isTriggered)
		return;

	bool isGroupDone = BlinkSystem::isGroupDone(blink.groupID);

	if (isGroupDone)
		BlinkSystem::resetGroup(blink.groupID);
	//����ʱ��������
	BlinkStatus bs = blinkObject();
	switch (bs)
	{
	case blinkDoneHidden:
		BlinkSystem::addFinished(blink.groupID);
		break;
	case blinkShown:
		//	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/blink.wav");
		break;
	case blinkWaiting:
		break;
	case blinkInit:
		break;
	default:
		break;
	}
}

void RObject::registerKnocks(const LuaRef& table)
{
	 
	knocks.clear();
	//���غ���
	auto local = [&](const LuaRef& ref) -> void 
	{
		auto knock = KnockPhase(ref);

		switch (knock.knockAction)
		{
		case KnockAction::hollow:
			setIsSolid(false);
			disallowCollision();
			break;
		case KnockAction::hidden:
			setVisible(false);
			allowCollision(knock.colliderDirection);
			break;
		case KnockAction::corrupt:
			break;
		case KnockAction::popup:

			break;
		}
		knocks.push_back(knock);
	};  

	if (table.len() == 0)
	{
		if (table.has(Luaf_Type)) 
			local(table); 
		else
		log("[RObject::registerKnocks] please set a array of knock options");
		return;
	}
	for (auto ref : table) 
		local(ref.value()); 
}
void RObject::registerSwitch(const LuaRef & ref)
{
	CCASSERT(ref.has(Luaf_Name), "��������FrameSwitchTemplate:framePreix");

	frameSwitchTemplate.framePrefix = ref[Luaf_Name].value<const char*>();
	frameSwitchTemplate.start = ref.get(Luaf_RangeStart, 1);
	frameSwitchTemplate.index = frameSwitchTemplate.start;
	frameSwitchTemplate.end = ref.get(Luaf_RangeEnd, 1);
	frameSwitchTemplate.direction = ref.get(Luaf_Direction, CollisionDirection::intersected);

	if (ref.has(Luaf_Sound))
		frameSwitchTemplate.sound = ref.get(Luaf_Sound).toValue<std::string>();
	if (ref.has(Luaf_limitedTimes))
		frameSwitchTemplate.limitedTimes = ref[Luaf_limitedTimes].value<short>();
}

void RObject::registerChar(const LuaRef & ref)
{
	//����Ѿ���ʼ����ʲô������
	if (nullptr != label)
		return;
	labelPhase = LabelPhase(ref);
	FontInfo font = FontInfo(ref);
	label = Label::createWithSystemFont("", Resh::getFontName(), font.size);
	addChild(label, getLocalZOrder() + 1);
	label->setPosition(font.size, font.size);
	label->setColor(font.color);
	label->setString(labelPhase.getString());
}

void RObject::registerText(const LuaRef & ref)
{
	if (!ref.has(Luaf_Text))
		return;

	auto font = FontInfo(ref);
	if (nullptr == label)
	{
		label = Label::createWithSystemFont("", Resh::getFontName(), font.size);
		addChild(label, getLocalZOrder() + 1);
	}

	label->setString(ref.get(Luaf_Text).toValue<std::string>());
	label->setColor(font.color);
	label->setSystemFontSize(font.size);
	auto offset = ref.get(Luaf_Offset, Vec2::ZERO);
	label->setPosition(Vec2(getContentSize().width / 2 , getContentSize().height / 2) + offset);

}

bool RObject::isGettingOff(Role& opponent, const Vec2& offset)
{
	bool isOff = abs(getPositionX() - opponent.getPositionX()) > getCollisionBound().size.width + offset.x
		|| abs(getCollisionBound().getMinY() - opponent.getCollisionBound().getMinY()) > getCollisionBound().size.height + offset.y;

	return isOff;
}

FrameSwitch RObject::switchFrame(Role & opponent)
{
	int tag = opponent.getTag();
	if (frameSwitch.find(tag) == frameSwitch.end())
		frameSwitch[tag] = frameSwitchTemplate;
	//û��key
	//�����ײ�����໥һ�� �����ȺͶ�����ײ���������ɶ���
	auto isCollided = checkObjectCollision(opponent);

	//û����ײ
	if (!isCollided)
	{
		//�뿪����Ч �ʵ�

		bool isOff = (!isGravityOn) ? true : isGettingOff(opponent);
		//����Ѿ��ߵ����һ֡,ˢ֡�޴�+1
		if (isOff && frameSwitch[tag].allowNext() && frameSwitch[tag].index == frameSwitch[tag].end)
		{
			frameSwitch[tag].touchedCount = 0;
			frameSwitch[tag].isTouched = false;
			frameSwitch[tag].index = frameSwitch[tag].start;

			//�������������ˢ֡һ������Դ�ͷ��ʼ������Ϊ��һ��frameindex
			if (frameSwitch[tag].allowNext())
			{
				auto frame = frameSwitch[tag].getFrame(frameSwitch[tag].framePrefix);
				if (nullptr != frame)
					setSpriteFrame(frame);
			}
		}

		return frameSwitch[tag];
	}

	//��[�м�]�����ֶ�����̧�����������,lua��player��enemy��Ҫ��˷�����ײ
	//��� isSolid = true ������excepted ��
	if (isSolid)
	{
		opponent.checkObjectSide();
		opponent.checkFollowing();
	}
	//���ƴ���
	if (!frameSwitch[tag].allowNext())
		return frameSwitch[tag];

	//��ײ���򲻷���Ҫ��
	if (isSolid)
		if (frameSwitch[tag].direction != opponent.getCollidedDirection())
			return frameSwitch[tag];
	//��һ֡
	if (frameSwitch[tag].touchedCount < (frameSwitch[tag].end - frameSwitch[tag].start))
	{
		Resh::playSound(frameSwitch[tag].sound.c_str());
		frameSwitch[tag].fetchNext();
		frameSwitch[tag].touchedCount++;
	}

	auto frame = frameSwitch[tag].getFrame(frameSwitch[tag].framePrefix);

	if (nullptr == frame)
		return frameSwitch[tag];

	setSpriteFrame(frame);

	if (!frameSwitch[tag].isTouched)
	{
		frameSwitch[tag].isTouched = true;
		frameSwitch[tag].touchedTimes++;
	}

	//���ص�ǰ֡index
	return frameSwitch[tag];
}

