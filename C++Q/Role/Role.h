#ifndef ROLE_H
#define ROLE_H

#include "cocos2d.h"
#include "../Utils/RectTools.h"
#include <string>
#include "Property.h"
#include "RoleFrame.h"
#include "../Utils/OBB.h"
#include "../../Common/Misc/FrameCounter.h"
#include "../Map/MapProperty.h"
#include "../Map/MapManager.h"
#include "../Map/TiledMap.h"
#include "../Triggers/TriggerSystem.h"
#include "WeaponSystem.h"
#include "Weapon.h"
#include "ui/UIScale9Sprite.h"
#include "Dialogue.h"
#include "RObjectPhase.h"
using namespace cocos2d;
#define CREATED_END \
	{ \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = nullptr; \
        return nullptr; \
    } 
#define CREATE_WITH_FILE(__TYPE__,NAME) \
    auto *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->initWithFile(NAME)) \
	CREATED_END

#define CREATE_WITH_FRAMENAME(__TYPE__,NAME) \
    auto *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->initWithSpriteFrameName(NAME)) \
	CREATED_END

#define CREATE_WITH_FRAME(__TYPE__,FRAME) \
    auto *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->initWithSpriteFrame(FRAME)) \
	CREATED_END
enum BorderLimited
{
	notLimited = 0,
	limiteHorizon = 1,
	limiteVertical = 2,
	limiteBoth = 3
};

enum class BodyStatus
{
	healthy,
	hurting,
	dead,
	fall
};
enum class RoleType :unsigned short
{
	player = 1,
	npc,
	robject,
	projectTile
};
enum class Anistyle
{
	toast = 1,
	fade,
	rollOut,
	explosion,
	hovering,
	rebound,
	rotate,
	jump
};

static const Vec2 InsetTile = Vec2(12.f, 0.0f);
static const Vec2 InsetObject = Vec2(3.5f, 0.f);
static const short AniCount = static_cast<short>(Anistyle::rotate) + 1;
class Role : public Property
{
	//需要访问私有变量lua脚本读取
	friend class RoleManager;
private:
	Role* collidedOpponent;
	Rect objectIntersection;
	//存取上一次碰撞前的位置
	std::unordered_map<int, CollisionDirection> lastCollideCollection;
	std::unordered_map<short, LineInfo> lastCollideLine;
	//碰撞后之前的位置
	CollisionDirection lastCollideDirection;
	ui::Scale9Sprite* scaleDialog;
	Sprite* anchor;
	BodyStatus 	bodyStatus = BodyStatus::healthy;
	Vec2 dragStart = Vec2::ZERO;
	Vec2 dragEnd = ScreenSize;
	DrawNode* drawCollision;
	TriggerSystem sensorSystem;
	DelayPhase delayFrame;
	DelayPhase delayRecovery;
	vector<MovePhase> moves;
	 
	//刷帧前缀可能更改
	std::string framePrefix;
	//方向移动完成个数配合 moves 使用
	unsigned short mobileFinished = 0;
	//以后改动？数组
	bool isAnimateFinished;// [AniCount];
	//根据是否定位状态来执行刷帧
	bool isLocating = false;
	//给初始值，否则lua端数据不准确
	int touchedTimes = 0;
	void updateDirection();
	//初始化的时候allowTouh=true才会注册，否则不能直接注册，提高效率
	void registerTouch();
	void rebound();
	//限制一次只能触摸一个精灵，位置倒了从里到外，以后调试
	static int touchedRole;	
	//刷帧有先后顺序
	virtual void updateFrame(const char*);
protected:
	RoleType type;
	short hp = 1;
	short hpMax = 1;
	bool isHazardous;
	bool isSolid;
	bool allowDisposal;
	bool allowFollow;
	bool allowFlip = true;
	bool allowTouch = false;
	bool allowDragX = false;
	bool allowDragY = false;
	bool onCheckTile = true;
	bool onFire;
	int mapTag = 1;

	float moveAngle = 0;

	MovingDirection facedDirection;
	MovingDirection getTowards() const;
	WeaponSystem weaponSystem;
	//用来显示在内部显示文字 robject 对话框
	Label* label;
	//Tile 设置的pos
	Vec2 originMapPosition;
	Vec2 insetTile = InsetTile;
	Vec2 insetObject = InsetObject;
	//记录初始大小 obb碰撞使用,敲击砖块 JumpBy 防止走位
	Rect originBound;
	//其他角色也可能使用
	BoundRect* boundRects = nullptr; 
	//伸入伸出
	ReboundInfo reboundInfo;
	//inline void openDialogBox(const DialogueText& value) { openDialogBox(value.t) }
	//唯一索引
	bool checkTileSlopes(const unsigned short&, BoundRect[TileBoundNumber]);
	void setDesirePosition(const BoundPosition&, const Rect&);
	void resizeContent();
	FrameIndexes* frameIndexes = nullptr;

public:
	static int nextTag;
	Side sideLeft;
	Side sideRight;
	Side sideTop;
	Side sideBottom;
	Role();
	virtual ~Role();
	virtual void update();
	//重力如果为true的时候执行
	virtual void  updatePosition();
	virtual void loadScript();
	void registerFrameIndexes(const LuaRef&);
	inline virtual  bool isDead() { return (getBodyStatus() == BodyStatus::dead || getBodyStatus() == BodyStatus::fall); }
	MovingDirection scanSensor(Role* opponent);
	void spawn(const Vec2&, const Vec2& = Vec2::ZERO);

	void openDialogBox(const HintText&);
	Rect getCollisionBound(const Vec2& = Vec2::ZERO, const bool& isOriginBound = false);
	//瓦片和本身的碰撞
	void checkSelfCollision();
	//瓦片碰撞
	void checkTileCollision();
	//调用碰撞后，把对手赋给collidedOpponent以便碰撞细节调用
	bool checkObjectCollision(Role&, const bool& isOriginBound = false);
	//刺儿上
	inline void checkSpike()
	{
		//从新刷新
		auto boundRect = getMap()->getCenterTile(getPosition());
		if (boundRect.getProperty().isHarmful)
		 	gotoHell("sound/sting.wav");
	}
	void checkObjectSide();
	//目前是在线段平衡的情况下
	void checkLine(const short& tag,const Vec2& start, const Vec2& end);
	void checkFollowing();
	void checkBorder(const Vec2&, const Vec2&);
	void checkHints(const bool& = true);
	void showHints(const LuaRef&);
	//绝对定位到 
	bool locate(const Vec2& target, const bool& allowStop = true);
	void registerRebound(const LuaRef&);
	inline bool locateTile(const Vec2& tile, const bool& allowStop = true)
	{
		return locate(MAP_STAGE->getPositionByTileCoordinate(tile), allowStop);
	}
	void closeDialog();
	//角色到达地图边界限制
	void limiteScreenBorder(const BorderLimited& = BorderLimited::limiteHorizon);
	//受伤了要减去的HP
	BodyStatus& gotHurt(const short& = 0, const char* = "");
	//直接设置死亡
	virtual void gotoHell(const char* = "");
	void registerMoves(const LuaRef&);
	virtual void setAnimation(const LuaRef&);
	//伤害性碰撞检测
	inline bool gotHit(Role& opponent) { return getCollisionBound(insetObject).intersectsRect(opponent.getCollisionBound(insetObject)); };
	BodyStatus& getBodyStatus();

	RangeType getRange(Role*) ;
 
	//直接调用boundRect会产生tile position误差导致,从新刷新一边
	inline BoundRect getBoundSelf()
	{
		return getMap()->getCenterTile(getPosition());
	}
	/*
	*属性
	*/
	inline bool& getIsSolid() { return  isSolid; };
	//注意range距离如果太近的话，有些地方不能碰撞
	ObbRect getObb(const bool& = true);
	inline RoleType& getType() { return this->type; };
	inline bool& isDisposed() { return this->allowDisposal; };
	inline bool isAlive() { return getBodyStatus() == BodyStatus::healthy; }
	inline bool& getOnFire() { return this->onFire; };
	inline bool& getAllowFollow() { return this->allowFollow; };
	inline int& getTouchTimes() { return this->touchedTimes; };

	inline void recover() { bodyStatus = BodyStatus::healthy; }
	inline short& getHP() { return this->hp; };
	inline short& getHPMax() { return this->hpMax; };
	inline Vec2& getInsetObject() { return this->insetObject; }
	inline Vec2& getInsetTile() { return this->insetTile; }
	inline 	TriggerSystem& getSensor() { return sensorSystem; };
	inline CollisionDirection& getCollidedDirection() { return this->lastCollideDirection; };

	inline void setIsSolid(const bool& val) { this->isSolid = val; };
	//inline void setDisposal(const bool& val) { this->allowDisposal = val; };
	//在lua端调用
	inline void setDisposal(const bool& val = true) { LUAH->setDisposal(getTag()); };
	inline void setOnFire(const bool& val) { this->onFire = val; };
	inline void setAllowFollow(const bool& val) { this->allowFollow = val; };
	inline void setAllowTouch(const bool& val)
	{
		this->allowTouch = val;
		registerTouch();
	};
	inline void setAllowDrag(const bool& val)
	{
		allowTouch = val;
		allowDragX = val;
		allowDragY = val;
		//registerTouch();
	}
	inline void setAllowDragX(const bool& val)
	{
		allowTouch = val;
		allowDragX = val;
		//registerTouch();
	};
	inline void setAllowDragY(const bool& val)
	{
		allowTouch = val;
		allowDragY = val;
		//registerTouch();
	};
	inline void setOriginMap(const Vec2& val) { originMapPosition = val; }
	inline void setInsetObject(const Vec2& val) { this->insetObject = val; };
	inline void setInsetObjectX(const float& val) { this->insetObject.x = val; };
	inline void setInsetObjectY(const float& val) { this->insetObject.y = val; };
	inline void setInsetTile(const Vec2& val) { this->insetTile = val; }
	inline void setInsetTileX(const float& val) { this->insetTile.x = val; }
	inline void setInsetTileY(const float& val) { this->insetTile.y = val; }
	inline void setCheckTile(const bool& val) { this->onCheckTile = val; };
	inline void setHP(const short& val) { this->hp = val; };
	inline void setHPMax(const short& val) { this->hpMax = val; };
	inline void setTexture(const std::string& frameFile) { Sprite::setTexture(frameFile); }
	//注意png，不要刷帧 updateFrame
	inline void setFrame(const char* frameName)
	{
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		if (nullptr != frame)
			setSpriteFrame(frame);
	}
	inline void clearFrame() { framePrefix.clear(); }
	inline void stopAnimation()
	{ 
		stopAllActions();
		isAnimateFinished = true;
	}
	inline bool isTouched(const Vec2& touched) { return inRange(touched, getCollisionBound()); }
	inline void setFrameIndex(const FrameStyle& fs, const short& start, const short& end)
	{
		frameIndexes[static_cast<short>(fs)] = FrameIndexes(start, end);
	}
	inline void setFrameSpeed(const float& val) { this->delayFrame.delaySecond = val; };
	//property类里面有个setDirection
	void control(const MovingDirection&, const float& = 0);
	unsigned short& moveObject(const bool& = false);
	inline  MovingDirection& getFacedDirection() { return facedDirection; }
	inline void setFaceDirection(const MovingDirection& val) { facedDirection = val; }
	inline	CollisionDirection getCollisionDirection(const Rect& rect) { return RectTools::getCollsionDirection(getCollisionBound(insetObject), rect); }
	inline Role* getCollider() { return collidedOpponent; }
	inline  void gotoDesirePosition() { setPosition(desiredPosition); }
	inline WeaponSystem& getWeaponSystem() { return weaponSystem; }
	inline Weapon* getWeapon() { return weaponSystem.getCurrentWeapon(); }
	inline TiledMap* getMap() { return MAP_WITHTAG(mapTag); }
	//lua-intf不能注册重载函数，所以只能在外面包一层 
	inline Vec2 getPositionByLua() const { return getPosition(); }
	inline void allowCollision(CollisionDirection cd = CollisionDirection::intersected)
	{
		if (cd == CollisionDirection::intersected)
		{
			sideLeft.allowCollision = true;
			sideRight.allowCollision = true;
			sideTop.allowCollision = true;
			sideBottom.allowCollision = true;
			return;
		}
		sideLeft.allowCollision = CollisionDirection::atLeft == cd;
		sideRight.allowCollision = CollisionDirection::atRight == cd;
		sideTop.allowCollision = CollisionDirection::atTop == cd;
		sideBottom.allowCollision = CollisionDirection::atBottom == cd;
	}
	inline void disallowCollision(CollisionDirection cd = CollisionDirection::intersected)
	{
		if (cd == CollisionDirection::intersected)
		{
			sideLeft.allowCollision = false;
			sideRight.allowCollision = false;
			sideTop.allowCollision = false;
			sideBottom.allowCollision = false;
			return;
		}
		sideLeft.allowCollision = CollisionDirection::atLeft == cd ? false : true;
		sideRight.allowCollision = CollisionDirection::atRight == cd ? false : true;
		sideTop.allowCollision = CollisionDirection::atTop == cd ? false : true;
		sideBottom.allowCollision = CollisionDirection::atBottom == cd ? false : true;
	}	
	void gotPushed(Role&);
	 inline bool hasPushes()
	 {
		 return sideLeft.allowPush || sideRight.allowPush || sideTop.allowPush || sideBottom.allowPush;
	 }
	inline void allowPush(CollisionDirection cd = CollisionDirection::intersected)
	{
		if (cd == CollisionDirection::intersected)
		{
			sideLeft.allowPush = true;
			sideRight.allowPush = true;
			sideTop.allowPush = true;
			sideBottom.allowPush = true;
			return;
		}
		sideLeft.allowPush = CollisionDirection::atLeft == cd;
		sideRight.allowPush = CollisionDirection::atRight == cd;
		sideTop.allowPush = CollisionDirection::atTop == cd;
		sideBottom.allowPush = CollisionDirection::atBottom == cd;
	}
	inline void disallowPush(CollisionDirection cd = CollisionDirection::intersected)
	{
		if (cd == CollisionDirection::intersected)
		{
			sideLeft.allowPush = false;
			sideRight.allowPush = false;
			sideTop.allowPush = false;
			sideBottom.allowPush = false;
			return;
		}
		sideLeft.allowPush = CollisionDirection::atLeft == cd ? false : true;
		sideRight.allowPush = CollisionDirection::atRight == cd ? false : true;
		sideTop.allowPush = CollisionDirection::atTop == cd ? false : true;
		sideBottom.allowPush = CollisionDirection::atBottom == cd ? false : true;
	}
	inline bool isHarmful()
	{
		return sideLeft.isHarmful && sideRight.isHarmful && sideTop.isHarmful && sideBottom.isHarmful;
	}
	inline void setHarmful(const bool& value = true)
	{
		sideLeft.isHarmful = value;
		sideRight.isHarmful = value;
		sideTop.isHarmful = value;
		sideBottom.isHarmful = value;
	}

	template<class T>
	static T* createWithFrame(SpriteFrame* spriteFrame)
	{
		CREATE_WITH_FRAME(T, spriteFrame);
		return nullptr;
	}
	template<class T>
	static T* createWithFrameName(const std::string& fileName)
	{
		CREATE_WITH_FRAMENAME(T, fileName);
		return nullptr;
	}
	template<class T>
	static T* createWithFileName(const std::string& fileName)
	{
		CREATE_WITH_FILE(T, fileName);
		return nullptr;
	}
	//如果自动生成tag 都被Rolemanager进行管理
	//不执行此函数则不被lua中的Role表处理,生产的角色对象可能单独使用
	inline void generateTag()
	{
		setTag(nextTag);
		id = nextTag;
		nextTag++;
	}
};
#endif