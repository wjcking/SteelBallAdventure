#include "Role.h"  
#include "RObject.h"
#include "Player.h"
#include "Npc.h"
#include "RoleManager.h"
#include "RoleFrame.h"
#include "../Map/MapManager.h"
#include "../Map/TiledMap.h"

#include "SimpleAudioEngine.h"
#include "../../../Common/StateMachine/StateMachine.h"
#include "../Triggers/Trigger.h"
#include "../Utils/GameScriptor.h"
#include "../Utils/ResourceHelper.h"

using namespace CocosDenshion;

int Role::nextTag = 1;
int Role::touchedRole = 0;
Role::Role() :Property(), weaponSystem(*this), delayRecovery(0.30f), label(nullptr), delayFrame(0.15f), scaleDialog(nullptr)
{
	allowFollow = false;
	allowDisposal = false;

	isSolid = false;
	onFire = false;
	collidedOpponent = nullptr;
	facedDirection = MovingDirection::toRight;

	resizeContent();
	lastCollideCollection.reserve(Reserve_Role);
	lastCollideLine.reserve(Reserve_65534);

}
Role::~Role()
{
	sensorSystem.clear();

}

//sensor,动态添加的role没有
void Role::loadScript()
{
	auto roleTable = LUAH->getRole(getTag());
	//如果没有sensor
	if (!roleTable.has(Luaf_Sensor))
		return;
	Trigger* sensor;

	auto sensorTable = roleTable[Luaf_Sensor].value();
	for (auto iter = sensorTable.begin(); iter != sensorTable.end(); ++iter)
	{
		auto ref = iter.value();

		CCASSERT(ref.has(Luaf_Type), "请设置type");
		CCASSERT(ref.has(Luaf_Range), "请设置range-size");
		switch (ref[Luaf_Type].value<unsigned short>())
		{
		case 1:
			sensor = new TriggerRect(ref[Luaf_Range].value<Size>(), ref[Luaf_Direction].value<MovingDirection>());
			break;
		case 2:
			sensor = new TriggerLine(ref[Luaf_Range].value<Size>(), ref[Luaf_Direction].value<MovingDirection>());
			break;
		case 3:
			sensor = new TriggerCircle(ref[Luaf_Range].value<Size>(), ref[Luaf_Direction].value<MovingDirection>());
			break;
		}
		sensorSystem.addSensor(sensor);
	}
}
MovingDirection Role::scanSensor(Role* opponent)
{
	using namespace LuaIntf;

	auto sensedDirection = MovingDirection::stayStill;
	if (sensorSystem.getCount() == 0)
		return sensedDirection;
	//扫描要碰撞的对手 opponent 不是自己 
	sensorSystem.scanSensor(*opponent, [&](Trigger& sensor) {
		//更新的是自己的位置，来感知碰撞位置
		sensor.updateRegion(*this);
		//停止
		if (sensor.isTouching())
		{
			sensedDirection = sensor.getSensorDirection();
			return;
		}
	});

	return sensedDirection;
}

void Role::spawn(const Vec2& pos, const Vec2& offset)
{
	auto realPos = getMap()->getPositionByTileCoordinate(pos);
	realPos = realPos + offset;
	displace(realPos);
}

void Role::control(const MovingDirection & direction, const float & angle)
{
	moveAngle = angle;
	moveDirection = direction;
}
void Role::registerMoves(const LuaRef& table)
{
	moves.clear();
	allowFlip = false;//不允许反转

	for (auto iter = table.begin(); iter != table.end(); ++iter)
		moves.push_back(MovePhase(iter.value()));
}

/*
返回已经进行的阶段下标
*/
unsigned short& Role::moveObject(const bool& isLoop)
{
	for (auto& iter : moves)
	{
		moveStep = iter.moveSteps;
		//如果第一阶段已经完成，进入下一阶段
		if (iter.isDone)
			continue;
		//时间戳判断多少秒后进入下一阶段
		if (getDistance().x >= iter.distance.x || getDistance().y >= iter.distance.y)
		{
			mobileFinished++;
			iter.isDone = true;
			refreshOrigin();
			continue;
		}
		if (iter.direction == MovingDirection::stayStill && iter.isTimeUp())
		{
			iter.isDone = true;
			mobileFinished++;
			continue;
		}
		control(iter.direction);
		break;
	}
	//标识所有的阶段已经完成
	if (mobileFinished == moves.size())
	{
		stop();
		if (isLoop)
		{
			mobileFinished = 0.f;
			for (auto& iter : moves)
				iter.reset();
		}
	}
	return mobileFinished;
}

//重力，tile 提取关掉，否则影响起始位置
void Role::registerRebound(const LuaRef& ref)
{
	//重力，tile 提取关掉，否则影响起始位置
	isGravityOn = false;
	onCheckTile = false;

	if (ref.has(Luaf_Delay))
		reboundInfo.delay = ref.get(Luaf_Delay).toValue<float>();
	if (ref.has(Luaf_DelayBack))
		reboundInfo.delayBack = ref.get(Luaf_DelayBack).toValue<float>();
	if (ref.has(Luaf_Duration))
		reboundInfo.duration = ref.get(Luaf_Duration).toValue<float>();
	if (ref.has(Luaf_DurationBack))
		reboundInfo.durationBack = ref.get(Luaf_DurationBack).toValue<float>();
	if (ref.has(Luaf_Offset))
		reboundInfo.offset = ref.get(Luaf_Offset).toValue<Vec2>();
	if (ref.has(Luaf_limitedTimes))
		reboundInfo.limitedTimes = ref.get(Luaf_limitedTimes).toValue<short>();
	if (ref.has(Luaf_IsActive))
		reboundInfo.isActive = ref.get(Luaf_IsActive, true);

	if (ref.has(Luaf_AllowBack))
		reboundInfo.allowBack = ref.get(Luaf_AllowBack).toValue<bool>();
}

void Role::setAnimation(const LuaRef& ref)
{
	if (!ref.has(Luaf_Type))
		return;
	auto type = ref.get(Luaf_Type).toValue<Anistyle>();
	//toast 文字向上并慢慢消失 目前不受isAnimationFinished 限制
	if (Anistyle::toast == type)
	{
		auto moveRise = MoveTo::create(ref.get(Luaf_Delay, 0.4f),
			Vec2(getBoundingBox().getMidX(), getBoundingBox().getMaxY() + ref.get(Luaf_Height, 40.f)));
		auto fadeOut = FadeOut::create(0.5f);
		auto callEnd = CallFunc::create([&]() {
			//	isAnimateFinished = true;
		});

		if (ref.has(Luaf_File))
		{
			auto file = ref[Luaf_File].value<std::string>();
			assert(FileUtils::getInstance()->isFileExist(file) && "文件不存在");
			auto sprite = Sprite::create(file);
			getMap()->addChild(sprite, Z_RObject);
			sprite->setPosition(Vec2(getBoundingBox().getMidX(), getBoundingBox().getMaxY() + 20.f));
			auto toastAct = Sequence::create(moveRise, fadeOut, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sprite)),
				callEnd,
				nullptr
			);
			sprite->runAction(toastAct);
		}

		else if (ref.has(Luaf_Frame))
		{
			auto frame = ref[Luaf_Frame].value<std::string>();
			auto sprite = Sprite::createWithSpriteFrameName(frame);

			getMap()->addChild(sprite, Z_RObject);

			sprite->setPosition(Vec2(getBoundingBox().getMidX(), getBoundingBox().getMaxY() + 20.f));
			auto toastAct = Sequence::create(moveRise, fadeOut, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sprite)),
				callEnd,
				nullptr
			);
			sprite->runAction(toastAct);
		}
		else if (ref.has(Luaf_Text))
		{
			FontInfo font(ref);
			auto toast = ref.get(Luaf_Text).toValue<std::string>();
			auto lbHint = Label::createWithSystemFont(toast, Resh::getFontName(), font.size);

			lbHint->setColor(font.color);
			lbHint->setPosition(Vec2(getBoundingBox().getMidX(), getBoundingBox().getMaxY() + 20.f));

			getMap()->addChild(lbHint, Z_RObject);
			auto toastAct = Sequence::create(moveRise, fadeOut, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, lbHint)),
				callEnd,
				nullptr
			);

			lbHint->runAction(toastAct);
		}

	}
	else if (Anistyle::rebound == type)
	{
		//设置伸缩
		registerRebound(ref);
		//如果自动执行
		if (reboundInfo.isActive)
			rebound();
	}
	//jump
	if (Anistyle::jump == type)
	{
		stopAllActions();
		isAnimateFinished = false;
		auto duration = ref.has(Luaf_Duration) ? ref.get(Luaf_Duration).toValue<float>() : 0.2f;
		auto limitedTimes = ref.get(Luaf_limitedTimes, -1);
		auto height = ref.get(Luaf_Height, 10.0f);
		if (limitedTimes < 0)
			runAction(RepeatForever::create(JumpTo::create(duration, getPosition(), height, 1)));
		else
			runAction(JumpTo::create(duration, getPosition(), height, limitedTimes));

	}
	//if (!isAnimateFinished)
	//	return;
	//角色并慢慢消失，最后回收 lua isDisposed = true
	if (Anistyle::fade == type)
	{
		isAnimateFinished = false;
		auto delay = ref.has(Luaf_Delay) ? ref.get(Luaf_Delay, 1.f) : 1.0f;
		auto delayTime = ref.has(Luaf_DelayTime) ? ref.get(Luaf_DelayTime, 1.f) : 0;
		auto fadeSequence = Sequence::create(DelayTime::create(delayTime),
			FadeOut::create(delay),
			CallFunc::create([&]() {
			isAnimateFinished = true;
			setDisposal(getTag());

		}), nullptr);
		runAction(fadeSequence);
	}
	//角色滚动出屏幕, 最后回收 lua isDisposed = true
	else if (Anistyle::rollOut == type)
	{
		isAnimateFinished = false;
		//重力关掉
		isGravityOn = false;
		Vec2 destiny;
		if (isFlippedX())
			destiny = Vec2(getPositionX() - ScreenSize.width / 2.5f, getPositionY() + ScreenSize.height / 2.5f);
		else
			destiny = Vec2(getPositionX() + ScreenSize.width / 2.5f, getPositionY() + ScreenSize.height / 2.5f);
		auto spawnRollOut = Sequence::create(
			MoveTo::create(0.5f, destiny),
			CallFunc::create([&]() {
			isAnimateFinished = true;
			//这里直接释放player出错
		//	if (getTag() != ID_Player)
			//	else
			setIsSolid(false);
			setHarmful(false);
			setVisible(false);
		//	 	setDisposal(getTag());
		}), nullptr);

		runAction(RepeatForever::create(RotateBy::create(0.35f, 360)));
		runAction(spawnRollOut);
	}
	//爆炸
	else if (Anistyle::explosion == type)
	{
		auto delay = ref.has(Luaf_Delay) ? ref.get(Luaf_Delay).toValue<float>() : 1.5f;
		auto frame = ref.has(Luaf_Frame) ? ref.get(Luaf_Frame).toValue<std::string>() : "explode_white";
		auto explode = Sprite::createWithSpriteFrameName(StringUtils::format("%s%d.png", frame.c_str(), random(1, 4)));
		Resh::playSound(ref.get(Luaf_Sound).toValue<const char*>());
		if (nullptr != explode)
		{
			isAnimateFinished = false;
			explode->setPosition(getCollisionBound().getMidX(), getCollisionBound().getMidY());
			explode->setLocalZOrder(getLocalZOrder() + 1);
			getMap()->addChild(explode);

			explode->runAction(Sequence::create(DelayTime::create(delay),
				CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, explode)),
				CallFunc::create([&]() { isAnimateFinished = true; }),
				nullptr));
		}
	}
}

void Role::rebound()
{
	//如果允许则退出
	if (reboundInfo.isRunning)
		return;

	//如果没有初始化起始位置
	if (originPosition == Vec2::ZERO)
		refreshOrigin();
	//一定是起始位置，并且要预先设置tile不要碰撞,否则位置会被迫改变
	auto destiny = originPosition + reboundInfo.offset;
	auto delay = DelayTime::create(reboundInfo.delay);
	auto moveStart = MoveTo::create(reboundInfo.duration, destiny);

	Sequence* hovering = nullptr;
	//直接设置为活动状态
	//如果只不返回则不用callFunc end
	reboundInfo.isRunning = true;

	if (reboundInfo.isUnlimited())
	{
		auto moveBack = MoveTo::create(reboundInfo.durationBack, originPosition);
		auto  delayBack = DelayTime::create(reboundInfo.delayBack);
		auto funcEnd = CallFunc::create([&]() {
			//	reboundInfo.isRunning = false;
		});
		hovering = Sequence::create(delay, moveStart, delayBack, moveBack, funcEnd, nullptr);
		runAction(RepeatForever::create(hovering));
	}
	else
	{
		if (reboundInfo.allowBack)
		{
			auto moveBack = MoveTo::create(reboundInfo.durationBack, originPosition);
			auto  delayBack = DelayTime::create(reboundInfo.delayBack);

			auto funcEnd = CallFunc::create([&]() {
				reboundInfo.isRunning = false;
			});
			hovering = Sequence::create(delay, moveStart, delayBack, moveBack, funcEnd, nullptr);
		}
		else
		{
			hovering = Sequence::create(delay, moveStart, nullptr);
		}
		runAction(Repeat::create(hovering, reboundInfo.limitedTimes));
	}
}

/*缩放大小适应tile*/
void Role::resizeContent()
{
	Size tileSize = getMap()->getTileSize();
	Size originSize = getContentSize();
	float fitWidth = tileSize.width * 2 - 0.1f;
	float fitHeight = tileSize.height * 2 + 0.1f;
	setContentSize(Size(originSize.width > fitWidth ? fitWidth : originSize.width, originSize.height > fitHeight ? fitHeight : originSize.height));
}

/*AABB包围盒*/
void Role::setDesirePosition(const BoundPosition& boundPos, const Rect& intersection)
{
	switch (boundPos)
	{
	case BoundPosition::bottom:
		desiredPosition = Vec2(desiredPosition.x, desiredPosition.y + intersection.size.height);
		velocity = Vec2(velocity.x, 0.0f); //重要	
		//log("bottom:Width:%lf,Height::%lf", intersection.size.width, intersection.size.height);
		//如果只踩到一点则不算踩上去
		if (intersection.size.height > 0.012f)
			onGround = true;
		break;
	case BoundPosition::top:
		//在上面的tile，要让主角向上移移
		desiredPosition = Vec2(desiredPosition.x, desiredPosition.y - intersection.size.height);
		velocity = Vec2(velocity.x, 0.f); //设速度为0 
		break;
	case BoundPosition::left:
		desiredPosition = Vec2(desiredPosition.x + intersection.size.width, desiredPosition.y);
		break;
	case BoundPosition::right:
		desiredPosition = Vec2(desiredPosition.x - intersection.size.width, desiredPosition.y);
		break;
	default:
		//7 如果碰撞的水平面大于竖直面，说明角色是上下碰撞
		if (intersection.size.width > intersection.size.height)
		{
			velocity = Vec2(velocity.x, 0.f); //重要
			//tile is diagonal, but resolving collision vertically
			float intersectionHeight;
			if (boundPos > BoundPosition::topRight) //说明是踩到斜下的砖块,角色应该向上去
				//	if (boundPos == BoundPosition::bottomLeft ||  boundPos == BoundPosition::bottomRight)
			{
				////如果只踩到一点则不算踩上去
				//if (intersection.size.width < 0.66 && intersection.size.height < 0.12)
				//{
				//	addDesired(boundPos == BoundPosition::bottomLeft ? intersection.size.width : -intersection.size.width, - intersection.size.height);
				//	break;
				//}
				//log("updown:Width:%lf,Height::%lf", intersection.size.width, intersection.size.height);
				intersectionHeight = intersection.size.height;
				//如果只踩到一点则不算踩上去
				if (intersection.size.height > 0.012f)
					onGround = true;
			}
			else  //说明是顶到斜上的砖块，角色应该向下托
			{
				intersectionHeight = -intersection.size.height;
			}
			desiredPosition = Vec2(desiredPosition.x, desiredPosition.y + intersectionHeight);
		}

		else //如果碰撞的水平面小于竖直面，说明角色是左右撞到
		{
			float resolutionWidth;
			if (boundPos == BoundPosition::bottomLeft || boundPos == BoundPosition::topLeft) //角色碰到斜左边的tile 角色应该向右去
			{
				resolutionWidth = intersection.size.width;
			}
			else //角色碰到斜右边的tile, 角色应该向左去
			{
				resolutionWidth = -intersection.size.width;
			}
			desiredPosition = Vec2(desiredPosition.x + resolutionWidth, desiredPosition.y);
		}
		break;
	}

	//设置不在跳跃,角色刷帧
	if (onGround)
		isJumping = false;
}
/*
放在AABB碰撞前面,如果是不是solid并且只能从下方碰撞，要判断
*/
bool Role::checkObjectCollision(Role& oppo, const bool& isOriginBound)
{
	//敲砖块用原始bound
	auto bound = oppo.getCollisionBound(oppo.insetObject, isOriginBound);
	//碰撞前的位置,注意值得设置和tile平衡，不能卡住
	CollisionDirection cd = getCollisionDirection(bound);

	//如果没有碰撞则继续
	if (cd != CollisionDirection::intersected)
	{
		//记录上次的位置 
		lastCollideCollection[oppo.getTag()] = cd;
		return false;
	}

	//碰撞前的位置,注意值得设置和tile平衡，不能卡住
	objectIntersection = RectTools::getIntersectRect(getCollisionBound(insetObject), bound);//2.5f
	//没有发生碰撞
	if (objectIntersection.origin == Vec2::ZERO)
		return false;

	lastCollideDirection = lastCollideCollection[oppo.getTag()];
	collidedOpponent = &oppo;
	return true;
}
void Role::checkObjectSide()
{
	if (nullptr == collidedOpponent)
		return;
	const float JitterOffsetLeft = 1.55f;
	const float JitterOffsetRight = 2.0f;
	switch (lastCollideDirection)
	{
	case CollisionDirection::atLeft:
		if (!collidedOpponent->sideLeft.allowCollision)
			break;

		//如果交叉小于范围，则返回当前位置，以免和瓦片碰撞
		if (objectIntersection.size.width <= JitterOffsetLeft && objectIntersection.size.height <= JitterOffsetLeft)
			addDesired(objectIntersection.size.width / 2, objectIntersection.size.height * 2 + gravity.y);
		else
		{
			onWall = true;
			addDesiredX(-objectIntersection.size.width);
			//addDesiredY(objectIntersection.size.height + velocity.y + gravity.y);
		}
		break;
	case CollisionDirection::atRight:
		if (!collidedOpponent->sideRight.allowCollision)
			break;
		//如果在移动瓦片中则直接踩上去 	
		//同时在object向右走的时候高度应抬高
		if (objectIntersection.size.width <= JitterOffsetRight && objectIntersection.size.height <= JitterOffsetRight)
			addDesired(-objectIntersection.size.width, objectIntersection.size.height * 2 + gravity.y); //??? * 3.2f
		else
		{
			onWall = true;
			addDesiredX(objectIntersection.size.width);
			//addDesiredY(objectIntersection.size.height + velocity.y + gravity.y);
		}
		break;
	case CollisionDirection::atTop:
		if (!collidedOpponent->sideTop.allowCollision)
			break;
		resetVelocity();
		addDesiredY(objectIntersection.size.height - collidedOpponent->moveStep.y);
		//在物体上
		onObject = true;
		//设置不在跳跃 
		//if (isJumped)
		//	isJumped = false;
		isJumping = false;
		break;
	case CollisionDirection::atBottom:
		if (!collidedOpponent->sideBottom.allowCollision)
			break;
		addDesiredY(-objectIntersection.size.height);
		resetVelocity();
		break;
	}
	/*************注意这里*******/
	//log("pos:%d, width:%lf, height:%lf", lastCollideDirection, objectIntersection.size.width, objectIntersection.size.height);
	//collidedOpponent->gotoDesirePosition();
}

void Role::checkLine(const short& tag, const Vec2 & start, const Vec2 & end)
{
	//如果没开重力
	if (!isGravityOn)
		return;

	auto bound = getCollisionBound(insetObject);
	float distance = getPosition().distance(Vec2(start.x, end.x));

	distance = RectTools::getRange(Rect(start.x, end.y, end.x - start.x, 0.05f), bound);

	if (distance < RangeType::rangeThreatened)
		return;

	auto collided = RectTools::getLineIntersectRect(start, end, bound);

	if (collided.isIntersected && lastCollideLine[tag].start.y >= collided.start.y)
	{
		addDesiredY(collided.start.y - bound.getMinY());
		resetVelocity();
		//在物体上
		onObject = true;
		//设置不在跳跃
		isJumping = false;
		//这里要加一个,否则会下陷
		gotoDesirePosition();
	}
	else
		this->lastCollideLine[tag].start = bound.origin;
}

void Role::checkFollowing()
{
	if (nullptr == collidedOpponent)
		return;
	//如果什么都不动则不检测
	if (collidedOpponent->moveDirection == MovingDirection::stayStill)
	{
		gotoDesirePosition();
		return;
	}
	//【robject，npc可以】考虑移动物体如果没有踩在平台上 退出 
	//[??]固定物体如果添加此项,角色会渐渐深入物体
	//有待解决
	if (collidedOpponent->getType() == RoleType::robject && lastCollideDirection != CollisionDirection::atTop)
		return;

	//跳到平台上以后
	switch (collidedOpponent->getDirection())
	{
	case MovingDirection::toTop:
		break;
	case MovingDirection::toBottom:
		//设置速率，向下时跳跃不管用，updatePosition根据velocity里面限制了onJump
		//不需要设置desire
		velocity.y += -collidedOpponent->moveStep.y + gravity.y;
		break;
	case MovingDirection::toLeft:
		if (!collidedOpponent->getAllowFollow())
			break;
		//control(MovingDirection::toLeft);
		velocity.x -= collidedOpponent->moveStep.x;
		//这里减去重力主角才能呆在旋梯上否则会慢慢下陷
		//减去重力就相当于加上反重力，向上托
		desiredPosition = getPosition() + velocity - gravity;
		break;
	case MovingDirection::toRight:
		if (!collidedOpponent->getAllowFollow())
			break;
		velocity.x += collidedOpponent->moveStep.x;
		//这里减去重力主角才能呆在旋梯上否则会慢慢下陷
		//减去重力就相当于加上反重力，向上托
		desiredPosition = getPosition() + velocity - gravity;
		break;
	}
	//log("dx:%lf,dy:%lf, vx:%lf,%lf", desiredPosition.x, desiredPosition.y);
	gotoDesirePosition();
}

void Role::checkBorder(const Vec2 & min, const Vec2 & max)
{
	if (getPositionX() < min.x)
		addDesiredX(min.x - getPositionX());
	else if (getPositionX() > max.x)
		addDesiredX(-(getPositionX() - max.x));

	if (getPositionY() < min.y)
		addDesiredY(min.y - getPositionY());
	else if (getPositionY() > max.y)
		addDesiredY(-(getPositionY() - max.y));

	gotoDesirePosition();
}

void Role::checkHints(const bool& flag)
{
	if (!flag)
	{
		closeDialog();
		return;
	}
	auto hint = DIALOGUE->fetchHint(getTag());

	if (0 == hint.tag)
		return;

	openDialogBox(hint);

}
void Role::showHints(const LuaRef& ref)
{
	if (!ref.has(Luaf_Text))
		return;

	HintText hint;
	hint.text = ref.get(Luaf_Text).toValue<std::string>();
	hint.font = FontInfo(ref);
	hint.zorder = ref.get(Luaf_ZOrder, Z_Dialog);
	openDialogBox(hint);
	scaleDialog->setLocalZOrder(hint.zorder);

}
bool Role::locate(const Vec2& target, const bool& allowStop)
{
	//距离在0 和 1 之间 就到达目的地
	// log("[pos]%lf %f [target]%lf %lf", getPositionX(),getPositionY(), target.x,target.y);
	auto distance = getPosition().getDistance(target);
	// log("[distance]%lf  %d", distance,);

	if (distance > 0 && distance < 5)
	{
		isLocating = false;

		if (allowStop)
			return true;
	}
	isLocating = true;
	/*
	* 【注意】这里用的是当前位置，表示跟踪，而不是originPosition，
	*/
	auto deltaX = target.x - getPositionX(); // 注意，、是以主角位置为起点 在上图中表示就是x1-x0
	auto deltaY = target.y - getPositionY(); // y1-y0

	// 子弹需要下移
	// 子弹需要上移
	if (deltaX == 0)
		deltaX = (target.y >= desiredPosition.y) ? 0.0000001 : -0.0000001;
	//同理，对deltaY作判断

	// 子弹需要右移
	// 子弹需要左移
	if (deltaY == 0)
		deltaY = (target.x >= desiredPosition.x) ? 0.0000001 : -0.0000001;

	float angle = 0;
	if (deltaX > 0 && deltaY > 0)
		angle = atan(fabs(deltaY / deltaX)); // 第一项限
	else if (deltaX < 0 && deltaY > 0)
		angle = 3.1415926f - atan(fabs(deltaY / deltaX)); // 第二项限
	else if (deltaX < 0 && deltaY < 0)
		angle = 3.1415926f + atan(fabs(deltaY / deltaX)); // 第三项限
	else
		angle = 2 * 3.1415926f - atan(fabs(deltaY / deltaX)); // 第四项限
															  //其中π取3.1415926…………(呵呵，别忘记近似哦)
															  //好了，现在已经得到正确的方向了，可以计算子弹坐标了！
	desiredPosition.x += moveStep.x * cos(angle);
	desiredPosition.y += moveStep.y * sin(angle);

	//移动到
	gotoDesirePosition();
	return false;
}

MovingDirection Role::getTowards() const
{
	switch (facedDirection)
	{
	case MovingDirection::toTop:
	case MovingDirection::toTopLeft:
	case MovingDirection::toTopRight:
		return MovingDirection::toTop;
		break;
	case MovingDirection::toBottom:
	case MovingDirection::toBottomLeft:
	case MovingDirection::toBottomRight:
		return MovingDirection::toBottom;
		break;
	case MovingDirection::toLeft:
		return MovingDirection::toLeft;
	case MovingDirection::toRight:
		return MovingDirection::toRight;
		break;
	}
	return MovingDirection::stayStill;
}

void Role::openDialogBox(const HintText& dlg)
{
	if (nullptr == label)
		label = Label::createWithSystemFont("", Resh::getFontName(), dlg.font.size);
	//每次对话更新文字颜色大小
	label->setColor(dlg.font.color);
	//label->setLocalZOrder(Z_Dialog);
	label->setSystemFontSize(dlg.font.size);
	label->setString(dlg.text);
	if (nullptr == scaleDialog)
	{
		scaleDialog = ui::Scale9Sprite::createWithSpriteFrameName("dialog.png");
		anchor = Sprite::createWithSpriteFrameName("anchor.png");

		scaleDialog->addChild(anchor);
		scaleDialog->addChild(label);
		scaleDialog->setLocalZOrder(Z_Dialog);
		label->setContentSize(Size(dlg.scaleWidth, 50));
		scaleDialog->setContentSize(Size(dlg.scaleWidth, 50));
		getMap()->addChild(scaleDialog);

	}
	
	//主要是label来控制
	scaleDialog->setVisible(true);
	const Size padding = Size(18, 18);
	label->setPosition(scaleDialog->getContentSize().width / 2, scaleDialog->getContentSize().height / 2);

	auto labelSize = label->getContentSize() + padding;
	auto dialogSize = labelSize.width <= dlg.scaleWidth ? Size(dlg.scaleWidth, labelSize.height) : labelSize;
	scaleDialog->setContentSize(dialogSize);


	//设置相对于role的位置，对话框放在那里
	if (!isFlippedX())
	{
		anchor->setFlippedX(false);
		anchor->setPosition(15.f, -1.8f);
		scaleDialog->setPosition(Vec2(getCollisionBound().getMaxX() + scaleDialog->getContentSize().width / 2, getCollisionBound().getMaxY() + scaleDialog->getContentSize().height / 2));
	}
	else
	{
		anchor->setFlippedX(true);
		anchor->setPosition(scaleDialog->getContentSize().width - 15.f, -1.8f);
		scaleDialog->setPosition(Vec2(getCollisionBound().getMinX() - scaleDialog->getContentSize().width / 2, getCollisionBound().getMaxY() + scaleDialog->getContentSize().height / 2));
	}


}

void Role::closeDialog()
{//label->removeFromParent();
		//anchor->removeFromParent();
		//scaleDialog->removeFromParent();

		//label = nullptr;
		//anchor = nullptr;
		//scaleDialog = nullptr;
	if (nullptr != scaleDialog)
		scaleDialog->setVisible(false);

	DIALOGUE->resetHint(getTag());
}

/*
AABB 瓦片碰撞
*/
void Role::checkTileCollision()
{
	boundRects = getMap()->getBoundTiles(*this);
	if (nullptr == boundRects)
	{
		gotoDesirePosition();
		return;
	}

	//重新设置是否在地面上
	//isJumped = false;
	onGround = false;
	//如果到达斜坡底部,重置重力
	resetGravity();

	//最后一个tile自己，斜坡一定要用
	for (int index = 0; index < TileBoundNumber; index++)
	{
		//注意lua下标 1 
		//如果是0或者指定的tile 则可以穿行
		if (boundRects[index].gid <= 0)
			continue;

		if (boundRects[index].getProperty().allowThrough)
		{
			continue;
		}
		//如果有名称则为移动瓦片，原始位置瓦片不能在碰撞
		if (!boundRects[index].tileName.empty())
			continue;

		auto bound = getCollisionBound(insetTile);

		Rect intersection = RectTools::getIntersectRect(bound, boundRects[index]);
		if (intersection.origin == Vec2::ZERO)
			continue;

		//斜坡   
		if (checkTileSlopes(index, boundRects))
		{
			gotoDesirePosition();
			return;
		}
		if (boundRects[index].getProperty().isSlope)
		{
			gotoDesirePosition();
			continue;
		}
		//放在setdesire前面,放在数组里，不能一个一个输出
		//one-way 平台
		if (index == BoundPosition::bottom || index == BoundPosition::bottomLeft || index == BoundPosition::bottomRight)
		{
			//tile在hero正下方 落到了tile上
			if (!boundRects[index].getProperty().isOneWay)
			{
				setDesirePosition((BoundPosition&)index, intersection);
			}
			else if (boundRects[index].getProperty().isOneWay)
			{
				float by = boundRects[index].getMaxY() + 12;//疑问
				if (onFalling &&  getPositionY() > by)
					setDesirePosition((BoundPosition&)index, intersection);
			}
		}
		else if (index == BoundPosition::top) //考拉头顶到tile
		{
			if (boundRects[index].gid > 0 && bound.getMaxY() >= boundRects[index].origin.y)
			{

			}
			if (!boundRects[index].getProperty().isOneWay)
				setDesirePosition((BoundPosition&)index, intersection);
		}
		else if (index == BoundPosition::left)
		{
			//onWall = true;
			//velocity = Vec2(velocity.x * 0.09f, velocity.y*0.75f); //2
			if (!boundRects[index].getProperty().isOneWay)
				setDesirePosition((BoundPosition&)index, intersection);

		}
		else if (index == BoundPosition::right)
		{
			//onWall = true;
			//velocity = Vec2(velocity.x * 0.09f, velocity.y*0.75f); //2
			if (!boundRects[index].getProperty().isOneWay)
				setDesirePosition((BoundPosition&)index, intersection);
		}
		else
		{
			if (!boundRects[index].getProperty().isOneWay)
				setDesirePosition((BoundPosition&)index, intersection);
		}

	}
	//把主角位置设定到它期望去的地方
	gotoDesirePosition();
	delete[] boundRects;
}

void Role::registerTouch()
{
	if (!allowTouch)
		return;

	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [&](Touch* touch, Event* event) {
		auto locationInNode = convertToNodeSpace(touch->getLocation());
		auto isTouched = Rect(Vec2::ZERO, getContentSize()).containsPoint(locationInNode);

		if (isTouched)
			touchedRole = getTag();
		return isTouched;
	};


	listener->onTouchMoved = [&](Touch* touch, Event* event) {

		if (touchedRole != getTag())
			return;
		//auto diffX = abs(getPositionX() - originPosition.x);
		//auto diffY = abs(getPositionY() - originPosition.x);
		if (allowDragX && allowDragY)
		{
			//	if (diffX >= dragStart.x && diffX <= dragEnd.x && diffY >= dragStart.y && diffY <= dragEnd.y)
			setPosition(getPosition() + touch->getDelta());
		}
		else if (allowDragX)
		{
			//	if (diffX >= dragStart.x && diffX <= dragEnd.x)
			setPositionX(getPositionX() + touch->getDelta().x);
		}
		else if (allowDragY)
		{
			//	if (diffY >= dragStart.y && diffY <= dragEnd.y)
			setPositionY(getPositionY() + touch->getDelta().y);
		}
	};


	listener->onTouchEnded = [&](Touch* touch, Event* event) {
		touchedTimes++;
		touchedRole = 0;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

bool Role::checkTileSlopes(const unsigned short& positionIndex, BoundRect  boundRects[TileBoundNumber])
{
	if (getMovingY() == MovingDirection::toTop)
		return false;

	auto slopeRect = boundRects[positionIndex];
	auto tileProperty = getMap()->getProperty(slopeRect.gid);

	if (tileProperty.slopeTowards == SlopeTowards::faceLeft && tileProperty.slopeDegree == SlopeDegree::degree45)
	{
		if (getPositionX() >= slopeRect.getMidX())
			if ((boundRects[BoundPosition::bottomRight].gid != slopeRect.gid && boundRects[BoundPosition::bottomRight].gid > 0) || (boundRects[BoundPosition::bottom].gid != slopeRect.gid && boundRects[BoundPosition::bottom].gid > 0))
				return false;

		float xDiff = desiredPosition.x - slopeRect.origin.x;
		float maxSlopeY = getContentSize().height > slopeRect.size.height ? slopeRect.getMinY() + getContentSize().height : slopeRect.getMaxY();
		float slopeY = maxSlopeY + xDiff - tileProperty.roleOffsetY;

		if (desiredPosition.y > slopeY)
			gravity.y = tileProperty.slopeGravity;
		else
		{
			resetGravity();
			desiredPosition.y = slopeY;
			onSlopeBottom = true;
			resetVelocity();
		}
		return true;
	}
	else if (tileProperty.slopeTowards == SlopeTowards::faceRight && tileProperty.slopeDegree == SlopeDegree::degree45)
	{
		if (getPositionX() <= slopeRect.getMidX())
			if ((boundRects[BoundPosition::bottomLeft].gid != slopeRect.gid && boundRects[BoundPosition::bottomLeft].gid > 0) || (boundRects[BoundPosition::bottom].gid != slopeRect.gid && boundRects[BoundPosition::bottom].gid > 0))
				return false;

		float xDiff = (desiredPosition.x - slopeRect.origin.x) - getMap()->getTileSize().width;
		float maxSlopeY = getContentSize().height > slopeRect.size.height ? slopeRect.getMinY() + getContentSize().height : slopeRect.getMaxY();
		float slopeY = maxSlopeY - xDiff - tileProperty.roleOffsetY;
		//float slopeY = slopeRect.origin.y + this->getContentSize().height - xDiff - tileProperty.roleOffsetY;
		if (desiredPosition.y > slopeY)
			gravity.y = tileProperty.slopeGravity;
		else
		{
			resetGravity();
			desiredPosition.y = slopeY;
			onSlopeBottom = true;
			resetVelocity();
		}
		return true;
	}
	else if (tileProperty.slopeTowards == SlopeTowards::faceLeft && tileProperty.slopeDegree == SlopeDegree::degree22_5)
	{
		if (getPositionX() >= slopeRect.getMidX())
			if ((boundRects[BoundPosition::bottomRight].gid != slopeRect.gid && boundRects[BoundPosition::bottomRight].gid > 0) || (boundRects[BoundPosition::bottom].gid != slopeRect.gid && boundRects[BoundPosition::bottom].gid > 0))
				return false;
		// 	m = (y2 - y1) / (x2 - x1);
		float m = (slopeRect.getMaxY() - slopeRect.origin.y) / (slopeRect.getMaxX() - slopeRect.origin.x);

		float x = desiredPosition.x * m;
		float interception = getMap()->getTileSize().height / 2;
		float initSlopeY = slopeRect.getProperty().initSlopeY ? interception : 0.f;//22度角的第二个tile
		float degree = interception / getMap()->getTileSize().height;
		float maxSlopeY = getContentSize().height > slopeRect.size.height ? slopeRect.getMinY() + getContentSize().height : slopeRect.getMaxY();
		//加法 y1 = y + (x1 - x) * (v / u)
		float formulation = maxSlopeY + initSlopeY + interception + (getPositionX() - slopeRect.origin.x)* degree;
		float slopeY = formulation - interception - tileProperty.roleOffsetY;

		if (desiredPosition.y >= slopeY)
			gravity.y = tileProperty.slopeGravity;
		else
		{
			resetGravity();
			desiredPosition.y = slopeY;
			onSlopeBottom = true;
			resetVelocity();
		}
		return true;
	}
	else if (tileProperty.slopeTowards == SlopeTowards::faceRight && tileProperty.slopeDegree == SlopeDegree::degree22_5)
	{
		if (getPositionX() <= slopeRect.getMidX())
			if ((boundRects[BoundPosition::bottomLeft].gid != slopeRect.gid && boundRects[BoundPosition::bottomLeft].gid > 0) || (boundRects[BoundPosition::bottom].gid != slopeRect.gid && boundRects[BoundPosition::bottom].gid > 0))
				return false;

		float m = (slopeRect.origin.y - slopeRect.getMaxY()) / (slopeRect.origin.x - slopeRect.getMaxX());
		float x = desiredPosition.x *m;
		float interception = getMap()->getTileSize().height / 2;
		float degree = interception / getMap()->getTileSize().height;
		float initSlopeY = slopeRect.getProperty().initSlopeY ? interception : 0.f; //22度角的第二个tile
		//y1 = y + (x1 - x) * (v / u)
		//减法
		float maxSlopeY = getContentSize().height > slopeRect.size.height ? slopeRect.getMinY() + getContentSize().height : slopeRect.getMaxY();
		float formulation = maxSlopeY + initSlopeY + interception - (getPositionX() - slopeRect.origin.x) * degree - tileProperty.roleOffsetY - (getContentSize().height - slopeRect.size.height);
		if (desiredPosition.y >= formulation)
		{
			gravity.y = tileProperty.slopeGravity;
		}
		else
		{
			resetGravity();
			desiredPosition.y = formulation;
			onSlopeBottom = true;
			resetVelocity();
		}
		return true;
	}

	return false;
}

/*
自身碰撞
*/

Rect Role::getCollisionBound(const Vec2& inset, const bool& isOriginBound)
{
	if (inset.x == 0 && inset.y == 0)
		return getBoundingBox();

	Rect originBox = isOriginBound ? originBound : getBoundingBox();

	auto collisionBox = RectTools::RectInset(originBox, inset.x, inset.y);
	auto diff = desiredPosition - this->getPosition(); //玩家当前距离与目的地的差距
	auto boundingBox = RectTools::RectOffset(collisionBox, diff.x, diff.y); //计算调整后的碰撞盒,即包围盒x,y轴方向上移动diff.x, diff.y个单位
	//boundingBox.origin.x = boundingBox.origin.x + 20;
	return boundingBox;
}

void Role::checkSelfCollision()
{

}

void Role::update()
{
	if (framePrefix != "")
		updateFrame(framePrefix.c_str());
	updatePosition();
	//开关，子弹关闭
	if (onCheckTile)
		checkTileCollision();
	else
		//重力不开也要更新位置,进行运算
		gotoDesirePosition();

}
//如果重力打开则执行,根据onGravity判断
void Role::updatePosition()
{
	//log("isJumped:%d;  onObject:%d;", isJumped, onObject);
	 //if (getTag() == ID_Player && velocity.y != 0)
	 //	log("velocity:%lf;", velocity.y);
	//是否下落
	onFalling = velocity.y < 0 ? true : false;
	//如果当前玩家按了跳跃键并且在 物体上
	 //加上onFalling 暂时解决了在从物体下落后速度< 0 还能跳的问题&& !onFalling&& (velocity.y > -1 && velocity.y <= 0 )
	if (isJumped && !inAir(true))
	{//!onFalling
		isJumping = true;
		//jumpTimes++;
		//重置重力，斜坡用
		//resetGravity();
		velocity = velocity + jumpForce;
		if (getTag() == ID_Player)
			SimpleAudioEngine::getInstance()->playEffect("sound/jump.wav");
	}

	//玩家没有按住跳跃键，并且向上的速度已经超过了设定的值，就限定向上跳跃速度		
	if (!isJumped  && velocity.y >= jumpCutoff.y)
		velocity = Vec2(velocity.x, jumpCutoff.y);
	//下落的时候才能重置跳跃速率 
	//还原，碰到弹簧以后 
	if (onFalling)
	{
		if (jumpCutoff != defaultJumpCutOff)
		{
			isJumped = false;
			jumpForce = defaultJumpForce;
			jumpCutoff = defaultJumpCutOff;
		}
	}
	//这里0.09疑问?
	//velocity = Vec2(velocity.x * 0.09f, velocity.y);
	velocity = Vec2(0.f, velocity.y);

	//更新方向
	updateDirection();

	if (isGravityOn)
	{
		velocity = velocity + gravity;
		desiredPosition = getPosition() + velocity;
	}
	else
	{
		desiredPosition = getPosition() + velocity;
		//如果没有重力则清空
		velocity.y = 0;
	}

}
/*
*位置判断放在重力前面
*/
void Role::updateDirection()
{
	//log("md:%d", moveDirection);
	switch (moveDirection)
	{
	case MovingDirection::toTop:
		facedDirection = MovingDirection::toTop;
		velocity.y += moveStep.y;
		break;
	case MovingDirection::toBottom:
		facedDirection = MovingDirection::toBottom;
		velocity.y += -moveStep.y;
		break;
	case MovingDirection::toLeft:
		facedDirection = MovingDirection::toLeft;
		if (allowFlip)
			setFlippedX(true);
		velocity.x += -moveStep.x;
		break;
	case MovingDirection::toRight:
		facedDirection = MovingDirection::toRight;
		if (allowFlip)
			setFlippedX(false);
		velocity.x += moveStep.x;
		break;
	case MovingDirection::toTopLeft:
		//facedDirection = MovingDirection::toTopLeft;
		velocity += moveAngle == 0 ? moveStep : Vec2(moveStep.x*cos(moveAngle), moveStep.y*sin(moveAngle));
		if (allowFlip)
			setFlippedX(true);
		break;
	case MovingDirection::toTopRight:
		//facedDirection = MovingDirection::toTopRight;
		velocity += moveAngle == 0 ? moveStep : Vec2(moveStep.x*cos(moveAngle), moveStep.y*sin(moveAngle));
		if (allowFlip)
			setFlippedX(false);
		break;
	case MovingDirection::toBottomLeft:
		//facedDirection = MovingDirection::toBottomLeft;
		velocity += moveAngle == 0 ? moveStep : Vec2(moveStep.x*cos(moveAngle), -moveStep.y*sin(moveAngle));
		if (allowFlip)
			setFlippedX(true);
		break;
	case MovingDirection::toBottomRight:
		//facedDirection = MovingDirection::toBottomRight;
		velocity += moveAngle == 0 ? moveStep : Vec2(moveStep.x*cos(moveAngle), -moveStep.y*sin(moveAngle));
		if (allowFlip)
			setFlippedX(false);
		break;
	case MovingDirection::stayStill:

		break;
	}
}

void Role::updateFrame(const char* frameName)
{
	//只有没有过时就刷新
	if (!delayFrame.isTimeUp())
		return;

	auto frameStyle = FrameStyle::idle;
	if (BodyStatus::dead == getBodyStatus())
	{
		frameStyle = FrameStyle::die;
	}
	else if (BodyStatus::hurting == getBodyStatus())
	{
		frameStyle = FrameStyle::hurt;
	}
	else if (getWeapon()->isFiring())
	{
		switch (getTowards())
		{
		case MovingDirection::toTop:
			frameStyle = FrameStyle::fireUP;
			break;
		case MovingDirection::toBottom:
			frameStyle = FrameStyle::fireDown;
			break;
		case MovingDirection::toLeft:
		case MovingDirection::toRight:
			frameStyle = FrameStyle::fire;
			break;
		}
		//在这里关掉射击状态
		getWeapon()->setFireOff();
	}
	else if (isJumping)
	{
		frameStyle = FrameStyle::jump;
	}
	else if (moveDirection != MovingDirection::stayStill)
	{
		switch (getTowards())
		{
		case MovingDirection::toTop:
			frameStyle = FrameStyle::walkUP;
			break;
		case MovingDirection::toBottom:
			frameStyle = FrameStyle::walkDown;
			break;
		case MovingDirection::toLeft:
		case MovingDirection::toRight:
			frameStyle = FrameStyle::walkX;
		default:
			break;
		}
	}
	else
	{
		switch (getTowards())
		{
		case MovingDirection::toTop:
			frameStyle = isLocating ? FrameStyle::walkUP : FrameStyle::faceUP;
			break;
		case MovingDirection::toBottom:
			frameStyle = isLocating ? FrameStyle::walkDown : FrameStyle::faceDown;
			break;
		case MovingDirection::toLeft:
		case MovingDirection::toRight:
			frameStyle = isLocating ? FrameStyle::walkX : FrameStyle::idle;
			break;
		}
	}

	//从frame管理器里面调用
	//auto frameIndexes = FrameManager::getFrameSheet(frameName);
	//如果没有
	if (nullptr == frameIndexes)
		return;
	auto index = static_cast<unsigned short>(frameStyle);
	//如果只有一帧，则指定一次，否则 
	auto frame = frameIndexes[index].getFrame(frameName);
	//=0 可能frameindexes没有设置
	if (frameStyle == FrameStyle::die)
	{
		//死亡倒下的帧
		if (RoleType::player == getType())
			delayFrame.delaySecond = 0.08f;
		if (frameIndexes[index].index == frameIndexes[index].end)
		{
			setSpriteFrame(frame);
			return;
		}
	}

	if (frameIndexes[index].index > 0 && nullptr != frame)
	{
		setSpriteFrame(frame);
		//放到外面来++，用0判断是否设置frameindexes
		frameIndexes[index].index++;
	}

	delayFrame.reset();

}
void Role::registerFrameIndexes(const LuaRef & ref)
{
	frameIndexes = new FrameIndexes[FrameLength];

	for (auto iter = ref.begin(); iter != ref.end(); ++iter)
	{
		CCASSERT(iter.value().has(Luaf_Type), "请设置Frame type");
		auto iFrameStyle = static_cast<short>(iter.value()[Luaf_Type].value<FrameStyle>());
		auto rangeStart = iter.value()[Luaf_RangeStart].value<short>();
		auto rangeEnd = iter.value()[Luaf_RangeEnd].value<short>();

		short index = 1;
		if (iter.value().has(Luaf_Index))
			index = iter.value()[Luaf_Index].value<short>();
		else
			index = rangeStart;
		//数组第一个name为framePrefix
		if (iter.value().has(Luaf_Name) && framePrefix == "")
			framePrefix = iter.value()[Luaf_Name].value<std::string>();
		frameIndexes[iFrameStyle] = FrameIndexes(index, rangeStart, rangeEnd);
	}

}
void Role::limiteScreenBorder(const BorderLimited& limitedOption)
{
	Vec2 mapPosition = getMap()->getPosition();
	if (BorderLimited::notLimited == limitedOption)
		return;

	float horizonStart = abs(mapPosition.x - getBoundingBox().size.width / 2);
	float horizonEnd = abs(mapPosition.x) + ScreenWidth - getBoundingBox().size.width / 2;
	float hDesired = 0.f;

	if (getPositionX() <= horizonStart)
		hDesired = horizonStart - getPositionX();
	else if (getPositionX() >= horizonEnd)
		hDesired = horizonEnd - getPositionX();

	float verticalStart = abs(mapPosition.y - getBoundingBox().size.height / 2);
	float verticalEnd = abs(mapPosition.y) + ScreenHeight - getBoundingBox().size.height / 2;
	float vDesired = 0.f;

	if (getPositionY() <= verticalStart)
		vDesired = verticalStart - getPositionY();
	else if (getPositionY() >= verticalEnd)
		vDesired = verticalEnd - getPositionY();

	if (BorderLimited::limiteHorizon == limitedOption)
		addDesiredX(hDesired);
	else if (BorderLimited::limiteVertical == limitedOption)
		addDesiredY(vDesired);
	else
	{
		addDesiredX(hDesired);
		addDesiredY(vDesired);
	}
	gotoDesirePosition();
}
BodyStatus& Role::gotHurt(const short& decreasedHP, const char* soundOuch)
{ 
	//只有在健康状态减去hp
	// if (BodyStatus::healthy == getBodyStatus())
	// {
 
	Resh::playSound(soundOuch);
	hp -= decreasedHP;
	// }

	//如果hp大于0，则处于受伤阶段
	if (hp > 0)
	{
		//重置延迟来控制刷帧
		delayRecovery.reset();
		bodyStatus = BodyStatus::hurting;
	}
	else
		bodyStatus = BodyStatus::dead;

	return bodyStatus;
}

void Role::gotoHell(const char* soundScream)
{
	if (bodyStatus != BodyStatus::dead)
		SimpleAudioEngine::getInstance()->playEffect(soundScream);
	stop();
	bodyStatus = BodyStatus::dead;
}

BodyStatus& Role::getBodyStatus()
{
	if (bodyStatus == BodyStatus::hurting)
	{
		if (delayRecovery.isTimeUp())
			bodyStatus = BodyStatus::healthy;
	}
	else if (getPositionY() < -getMap()->getTileSize().height)
		bodyStatus = BodyStatus::fall;
	//如果hp小于0或者调到陷阱里面去则死亡
	else if (hp <= 0)
		bodyStatus = BodyStatus::dead;

	return bodyStatus;
}

RangeType Role::getRange(Role* opponent)
{
//	 return getMap()->getRange(getPosition(), opponent->getPosition());
	float distance = getPosition().distance(opponent->getPosition());
	float threatened = opponent->getCollisionBound().size.width + opponent->getCollisionBound().size.height;

	if (distance <= threatened / 2)
		return RangeType::rangeEndangered;
	else if (distance <= threatened)
		return RangeType::rangeThreatened;
	else if (distance <= threatened * 2)
		return RangeType::rangeSensed;
	else if (distance <= ScreenWidth)
		return RangeType::rangeOutOfSight;

	return RangeType::rangeOut;
 }

ObbRect Role::getObb(const bool& flagVertex)
{
	ObbRect obb;
	auto pt = convertToWorldSpace(Vec2::ZERO);

	obb.setVertex(0, pt.x, pt.y);

	pt = convertToWorldSpace(flagVertex ? Vec2(originBound.size.width, 0) : Vec2(0, originBound.size.height));
	obb.setVertex(1, pt.x, pt.y);

	pt = convertToWorldSpace(Vec2(originBound.size.width, originBound.size.height));
	obb.setVertex(2, pt.x, pt.y);

	pt = convertToWorldSpace(flagVertex ? Vec2(0, originBound.size.height) : Vec2(originBound.size.width, 0));
	obb.setVertex(3, pt.x, pt.y);

	return obb;
}
void Role::gotPushed(Role& collider)
{
	if (collider.getCollidedDirection() == CollisionDirection::atLeft && sideLeft.allowPush)
		addDesiredX(sideLeft.pushSteps.x);
	if (collider.getCollidedDirection() == CollisionDirection::atRight && sideRight.allowPush)
		addDesiredX(-sideRight.pushSteps.x);
	if (collider.getCollidedDirection() == CollisionDirection::atBottom && sideBottom.allowPush)
		addDesiredY(sideBottom.pushSteps.y);
	//往下推的时候速度快，除以2？？？？？？？？？
	if (collider.getCollidedDirection() == CollisionDirection::atTop && sideTop.allowPush)
		addDesiredY(-sideTop.pushSteps.y / 2);
	gotoDesirePosition();
}