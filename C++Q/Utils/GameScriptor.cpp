#include "GameScriptor.h"
#include "../Role/Role.h"
#include "../Role/RoleFrame.h"
#include "../Role/Player.h"
#include "../Role/ProjectTile.h"
#include "../Role/RObjectPhase.h" 
#include "../Role/DialogueSystem.h"
#include "../../Common/StateMachine/State.h"
#include "../../Common/Misc/FrameCounter.h"
#include "../Map/TiledMap.h"
#include "../Map/CheckPoint.h"
#include "../Role/RoleManager.h"
#include "../Role/RObject.h"
#include "../Role/Npc.h"
#include "../Scene/StageScene.h"
#include "../Scene/MainScene.h"
#include "../Triggers/Trigger.h"
#include "../Ads/Ads.h"

#include "ActionExtend.h"
#include "SimpleAudioEngine.h"
#include "PopupLayer.h"
#include "RectTools.h"
#include "ResourceHelper.h"
#include "JoyStick.h"
using namespace cocos2d;
const std::string Luah::ScriptFolder = "script/";
unsigned short Luah::CurrentStage = 1;
void Luah::setRequirePath(const char* path)
{
	luaL_openlibs(l);
	lua_getglobal(l, "package");
	lua_getfield(l, -1, "path"); // get field "path" from table at top of stack (-1)
	std::string cur_path = lua_tostring(l, -1); // grab path string from top of stack
	cur_path.append(";"); // do your path magic here
	cur_path.append(path);
	cur_path.append("?.lua");//�����������lua�ű����������
	lua_pop(l, 1); // get rid of the string on the stack we just pushed on line 5
	lua_pushstring(l, cur_path.c_str()); // push the new one
	lua_setfield(l, -2, "path"); // set the field "path" in table at -2 with value at top of stack
	lua_pop(l, 1); // get rid of package table from top of stack
}

void Luah::loadPackages()
{
	//����
	auto path = FileUtils::getInstance()->getWritablePath() + ScriptFolder;

	//�ж��ļ����Ƿ����
	if (!FileUtils::getInstance()->isDirectoryExist(path))
		FileUtils::getInstance()->createDirectory(path);

	//doFile(ScriptFolder + "robjectstatus.lua");

	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		return;
	//����������ļ��У���ִ��lua require
	setRequirePath(path.c_str());
	CCASSERT(FileUtils::getInstance()->isFileExist(ScriptFolder + "packages.lua"), "non exsits packages");
	auto luaText = __String::createWithContentsOfFile(ScriptFolder + "packages.lua")->getCString();

	FileUtils::getInstance()->writeStringToFile(luaText, path + "packages.lua");
	doFile(path + "packages.lua");
	auto packages = getGlobal("Packages");
	CCASSERT(packages.isTable(), "������Packages");

	for (auto iter = packages.begin(); iter != packages.end(); ++iter)
	{
		CCASSERT(iter.value().has(Luaf_File), "file������");
		auto file = iter.value()[Luaf_File].value<std::string>();

		luaText = String::createWithContentsOfFile(ScriptFolder + file)->getCString();
		FileUtils::getInstance()->writeStringToFile(luaText, path + file);
		//	doFile(path + file);
	}
}
std::string Luah::doStage()
{
	//�ֻ��豸�洢Ŀ¼
	auto stage = CurrentStage;
	auto stageFile = StringUtils::format("stage%d.lua", stage);
	//���û��stage�ļ�ֱ���˳�
	if (!FileUtils::getInstance()->isFileExist(ScriptFolder + stageFile))
		return "";
	auto language = UserDefault::getInstance()->getIntegerForKey(User_Language, Lang_Chinwan);

	const auto globalFile = "global.lua";
	auto sceneFile = StringUtils::format("scene%d.lua", language);
	auto popupFile = StringUtils::format("popup%d.lua", language);
	auto dialogFile = StringUtils::format("dialog%d-%d.lua", stage, language);
	auto stateFile = StringUtils::format("state%d.lua", stage);
	////�����windowsƽ̨
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	{
		setRequirePath("script\\");
		doFile(ScriptFolder + globalFile);
		doFile(ScriptFolder + sceneFile);
		doFile(ScriptFolder + dialogFile);
		doFile(ScriptFolder + popupFile);
		doFile(ScriptFolder + stageFile);
		doFile(ScriptFolder + stateFile);

		return ScriptFolder + stageFile;
	}

	auto path = FileUtils::getInstance()->getWritablePath() + ScriptFolder;
	//���ѡ������Թۿ���ǰ���Ժ͹ؿ�һ�£����˳�
	if (language == currentLanguage && stage == currentStage)
	{
		doFile(path + globalFile);
		doFile(path + sceneFile);
		doFile(path + dialogFile);
		doFile(path + popupFile);
		doFile(path + stageFile);
		doFile(path + stateFile);

		return path + stageFile;
	}

	//params ȫ�ֱ���ҲҪ����
	auto luaText = String::createWithContentsOfFile(ScriptFolder + globalFile)->getCString();
	auto writtenResult = FileUtils::getInstance()->writeStringToFile(luaText, path + globalFile);
	doFile(path + globalFile);

	CCASSERT(FileUtils::getInstance()->isFileExist(ScriptFolder + dialogFile), "dialogFile lua non exists");
	CCASSERT(FileUtils::getInstance()->isFileExist(ScriptFolder + popupFile), "popupFile lua non exists");
	CCASSERT(FileUtils::getInstance()->isFileExist(ScriptFolder + stateFile), "stateFile lua non exists");
	CCASSERT(FileUtils::getInstance()->isFileExist(ScriptFolder + dialogFile), "dialogFile lua non exists");
	//�Ի� language
	luaText = String::createWithContentsOfFile(ScriptFolder + dialogFile)->getCString();
	FileUtils::getInstance()->writeStringToFile(luaText, path + dialogFile);
	doFile(path + dialogFile);
	//������ 
	luaText = String::createWithContentsOfFile(ScriptFolder + popupFile)->getCString();
	FileUtils::getInstance()->writeStringToFile(luaText, path + popupFile);
	doFile(path + popupFile);
	//�ؿ�
	luaText = String::createWithContentsOfFile(ScriptFolder + stageFile)->getCString();
	FileUtils::getInstance()->writeStringToFile(luaText, path + stageFile);
	doFile(path + stageFile);
	//ÿһ�ص�״̬��
	luaText = String::createWithContentsOfFile(ScriptFolder + stateFile)->getCString();
	FileUtils::getInstance()->writeStringToFile(luaText, path + stateFile);
	doFile(path + stateFile);
	//��¼��һ�ε�ѡ��
	currentStage = stage;
	currentLanguage = language;
	return path + stageFile;

}

void Luah::doScene()
{
	//�ֻ��豸�洢Ŀ¼
	auto language = UserDefault::getInstance()->getIntegerForKey(User_Language, Lang_Chinwan);
	//�����ǰ����һ����ʲô������
	auto sceneFile = StringUtils::format("scene%d.lua", language);
	//�����windowƽ̨
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	{
		setRequirePath("script\\");
		doFile(ScriptFolder + "global.lua");
		doFile(ScriptFolder + sceneFile);
		return;
	}

	auto path = FileUtils::getInstance()->getWritablePath() + ScriptFolder;


	//��������ļ���
	//FileUtils::getInstance()->addSearchPath(path);
	//params ȫ�ֱ���ҲҪ����
	auto luaText = String::createWithContentsOfFile(ScriptFolder + "global.lua")->getCString();
	auto writtenResult = FileUtils::getInstance()->writeStringToFile(luaText, path + "global.lua");
	doFile(path + "global.lua");
	//scene
	luaText = String::createWithContentsOfFile(ScriptFolder + sceneFile)->getCString();
	writtenResult = FileUtils::getInstance()->writeStringToFile(luaText, path + sceneFile);
	doFile(path + sceneFile);

}
void Luah::registerClasses()
{
	using namespace LuaIntf;

	//������̨
	LuaBinding(l).beginClass<Layer>("Layer")
		.addFunction("getName",&Layer::getName)
		.endClass()
		.beginExtendClass<MainScene, Layer>("MainScene")
		.addFunction("promptAchieve", &MainScene::promptAchieve)
		.endClass()
		.beginExtendClass<StageScene, Layer>("StageScene")
		.addFunction("gotoNext", &StageScene::gotoNext)
		.addFunction("getBeginDelta", &StageScene::getBeginDelta)
		.addFunction("getEndDelta", &StageScene::getEndDelta)
		.addFunction("isSlided", &StageScene::isSlided)
		.addFunction("isTypeDone", &StageScene::isTypeDone )
		.addFunction("prompt", &StageScene::prompt)
		.addFunction("promptAchieve", &StageScene::promptAchieve)
		.addFunction("output", &StageScene::output)
		.addFunction("loadCurtain", &StageScene::loadCurtain)
		.addFunction("dropCurtain", &StageScene::dropCurtain)
		.addFunction("shutCurtain", &StageScene::shutCurtain)
		.addFunction("setToolbar", &StageScene::setToolbar)
		.endClass()
		.beginExtendClass<PopupLayer, Layer>("PopupLayer")	//�Ի���
		.addFunction("getChild", &PopupLayer::getChild)
		.addFunction("isSlided", &PopupLayer::isSlided)
		.addFunction("allowSmack", &PopupLayer::allowSmack)
		.addFunction("setSmack", &PopupLayer::setSmack)
		.addFunction("setString", &PopupLayer::setString)
		.addFunction("setButtonName", &PopupLayer::setButtonName)
		.addFunction("setButtonVisible", &PopupLayer::setButtonVisible)
		.endClass();

	//��ɫ
	LuaBinding(l).beginClass<Side>("Side")
		.addVariableRef("KnockMax", &Side::KnockMax)
		.addVariableRef("pushSteps", &Side::pushSteps)
		.addVariableRef("knockedTimes", &Side::knockedTimes)
		.addVariableRef("isHarmful", &Side::isHarmful)
		.addVariableRef("isShowThorn", &Side::isShowThorn)
		.addVariableRef("allowPush", &Side::allowPush)
		.addVariableRef("allowCollision", &Side::allowCollision)
		.addVariableRef("isCollided", &Side::isCollided)
		.addFunction("getAllowKnock", &Side::getAllowKnock)
		.endClass();


	LuaBinding(l).beginClass<Node>("Node")
		.addFunction("getTag", &Node::getTag)
		.addFunction("setZOrder", &Node::setLocalZOrder)
		.addFunction("getPositionX", &Node::getPositionX)
		.addFunction("getPositionY", &Node::getPositionY)
		.addFunction("getBoundingBox",&Node::getBoundingBox)
		
		.addFunction("getContentSize", &Node::getContentSize)
		.endClass()
		.beginExtendClass<Sprite, Node>("Sprite")
		
		.addProperty("isFlippedX", &Sprite::isFlippedX, &Sprite::setFlippedX)
		.addProperty("isFlippedY", &Sprite::isFlippedY, &Sprite::setFlippedY)
		.addProperty("isVisible", &Sprite::isVisible, &Sprite::setVisible)
		.endClass()
		.beginExtendClass<Property, Sprite>("Property")
		.addFunction("getDistance", &Property::getDistance)
		.addFunction("isMoving", &Property::isMoving)
		.addFunction("getDirection", &Property::getDirection)
		.addFunction("getMovingX", &Property::getMovingX)
		.addFunction("getMovingY", &Property::getMovingY)
		.addFunction("setDirection", &Property::setDirection)
		.addFunction("setForward", &Property::setForward)
		.addFunction("setBackward", &Property::setBackward)
		.addFunction("setUpward", &Property::setUpward)
		.addFunction("setDownward", &Property::setDownward)
		.addFunction("refreshOrigin", &Property::refreshOrigin)
		.addFunction("respawn", &Property::respawn)
		.addFunction("stop", &Property::stop)
		.addFunction("resetVelocity", &Property::resetVelocity)
		.addFunction("resetGravity", &Property::resetGravity)
		.addFunction("setGravity", &Property::setGravity)
		.addFunction("setGravityOn", &Property::setGravityOn)
		.addFunction("setGravityOff", &Property::setGravityOff)
		.addFunction("resetMoveSteps", &Property::resetMoveSteps)
		.addFunction("setJumpForce", &Property::setJumpForce)
		.addFunction("setJumpForceOnWall", &Property::setJumpForceOnWall)
		.addProperty("originPosition", &Role::getOrigin, &Role::setOrigin)
		.addProperty("moveStep", &Property::getMoveStep, &Property::setMoveStep)
		.addProperty("onObject", &Property::getOnObject, &Property::setonObject)
		.addProperty("onJump", &Property::getOnJump, &Property::setOnJump)
		.addProperty("allowGravity", &Property::getGravityStatus, &Property::setGravityStatus)
		.addFunction("getJumpStatus", &Property::getJumpStatus)
		.addFunction("inAir", &Property::inAir)
		.addFunction("displace", &Property::displace)
		.endClass()
		.beginExtendClass<Role, Property>("Role")
		.addVariableRef("sideLeft", &Role::sideLeft)
		.addVariableRef("sideRight", &Role::sideRight)
		.addVariableRef("sideTop", &Role::sideTop)
		.addVariableRef("sideBottom", &Role::sideBottom)
		.addProperty("face", &Role::getFacedDirection, &Role::setFaceDirection)
		.addFunction("getBoundSelf", &Role::getBoundSelf)
		.addFunction("getType", &Role::getType)
		.addFunction("getInsetObject", &Role::getInsetObject)
		.addFunction("getRange", &Role::getRange)
		.addFunction("getPosition", &Role::getPositionByLua)
		.addFunction("getCollisionBound", &Role::getCollisionBound)
		.addFunction("getCollisionDirection", &Role::getCollisionDirection)
		.addFunction("getWeaponSystem", &Role::getWeaponSystem)
		.addFunction("getWeapon", &Role::getWeapon)
		.addFunction("getCollidedDirection", &Role::getCollidedDirection)
		.addFunction("getBodyStatus", &Role::getBodyStatus)
		.addFunction("getTouchTimes", &Role::getTouchTimes)
		.addFunction("gotPushed", &Role::gotPushed)
		.addFunction("recover", &Role::recover)
		.addFunction("spawn", &Role::spawn)
		.addFunction("setFrame", &Role::setFrame)
		.addFunction("setFrameIndex", &Role::setFrameIndex)
		.addFunction("setOnFire", &Role::setOnFire)
		.addFunction("setAllowFollow", &Role::setAllowFollow)

		.addFunction("setAnimation", &Role::setAnimation)
		.addFunction("setFrameSpeed", &Role::setFrameSpeed)
		.addFunction("setAllowTouch", &Role::setAllowTouch)
		.addFunction("setAllowDrag", &Role::setAllowDrag)
		.addFunction("setAllowDragX", &Role::setAllowDragX)
		.addFunction("setAllowDragY", &Role::setAllowDragY)
		.addFunction("setCheckTile", &Role::setCheckTile)
		.addFunction("setTexture", &Role::setTexture)

		.addFunction("clearFrame", &Role::clearFrame)
		.addFunction("stopAnimation", &Role::stopAnimation)
		.addFunction("locate", &Role::locate)
		.addFunction("locateTile", &Role::locateTile)
		.addFunction("control", &Role::control)
		.addFunction("scanSensor", &Role::scanSensor)
		.addFunction("allowFollow", &Role::getAllowFollow)
		.addFunction("gotoDesirePosition", &Role::gotoDesirePosition)
		.addFunction("updatePosition", &Role::updatePosition)

		.addFunction("checkHints", &Role::checkHints)
		.addFunction("showHints", &Role::showHints)
		.addFunction("closeDialog", &Role::closeDialog)
		.addFunction("checkLine", &Role::checkLine)
		.addFunction("checkSpike", &Role::checkSpike)
		.addFunction("checkObjectCollision", &Role::checkObjectCollision)
		.addFunction("checkObjectSide", &Role::checkObjectSide)
		.addFunction("checkFollowing", &Role::checkFollowing)
		.addFunction("checkTileCollision", &Role::checkTileCollision)
		.addFunction("checkBorder", &Role::checkBorder)
		.addFunction("limiteScreenBorder", &Role::limiteScreenBorder)
		.addFunction("gotHit", &Role::gotHit)
		.addFunction("gotHurt", &Role::gotHurt)
		.addFunction("gotoHell", &Role::gotoHell)
		.addFunction("allowCollision", &Role::allowCollision)
		.addFunction("disallowCollision", &Role::disallowCollision)
		.addFunction("hasPushes", &Role::hasPushes)
		.addFunction("allowPush", &Role::allowPush)
		.addFunction("disallowPush", &Role::disallowPush)
		.addProperty("isSolid", &Role::getIsSolid, &Role::setIsSolid)

		.addProperty("isHarmful", &Role::isHarmful, &Role::setHarmful)
		.addFunction("isAlive", &Role::isAlive)
		.addFunction("isTouched", &Role::isTouched)
		.addFunction("isDead", &Role::isDead)
		.addProperty("allowFollow", &Role::getAllowFollow, &Role::setAllowFollow)
		.addProperty("hp", &Role::getHP, &Role::setHP)
		.addProperty("hpMax", &Role::getHPMax, &Role::setHPMax)
		.endClass()
		.beginExtendClass<Player, Role>("Player")
		.addFunction("isDead", &Player::isDead)
		.addFunction("gotoHell", &Player::gotoHell)
		.addFunction("carryObject", &Player::carryObject)
		.addFunction("isCarrying", &Player::isCarrying)
		.addFunction("setCarriedOffset", &Player::setCarriedOffset)
		.addFunction("dropObject", &Player::dropObject)
		.addFunction("isFreezed", &Player::isFreezed)
		.addFunction("freeze", &Player::freeze)
		.addFunction("unfreeze", &Player::unfreeze)
		.addFunction("setAnimation", &Player::setAnimation)
		.endClass()
		.beginExtendClass<RObject, Role>("RObject")
		.addFunction("registerChar", &RObject::registerChar)
		.addFunction("registerSpring", &RObject::registerSpring)
		.addFunction("registerRebound", &RObject::registerRebound)
		.addFunction("registerMoves", &RObject::registerMoves)
		.addFunction("registerKnocks", &RObject::registerKnocks)
		.addFunction("registerText", &RObject::registerText)
		.addFunction("registerSwitch", &RObject::registerSwitch)
		.addFunction("setAnimation", &RObject::setAnimation)
		.addFunction("hasKnocks", &RObject::hasKnocks)
		.addFunction("checkObb", &RObject::checkObb)
		.addFunction("switchFrame", &RObject::switchFrame)
		.addFunction("gotKnocked", &RObject::gotKnocked)
		.addFunction("checkKnocks", &RObject::checkKnocks)
		.addFunction("moveObject", &RObject::moveObject)
		.addFunction("pollChar", &RObject::pollChar)
		.addFunction("getChar", &RObject::getChar)
		.addFunction("getSwitch", &RObject::getSwitch)
		.addFunction("blinkGroup", &RObject::blinkGroup)
		.addFunction("blinkSelf", &RObject::blinkSelf)
		.addFunction("bounce", &RObject::bounce)
		.endClass()
		.beginExtendClass<Npc, Role>("Npc")
		.addFunction("setAnimation", &Npc::setAnimation)
		.addFunction("setTowards", &Npc::setTowards)
		.addFunction("patrollingX", &Npc::patrollingX)
		.addFunction("patrollingY", &Npc::patrollingY)
		.addFunction("wanderingX", &Npc::wanderingX)
		//������
		.endClass().beginExtendClass<ProjectTile, Role>("ProjectTile")
		.addFunction("allowThrough", &ProjectTile::allowThrough)
		.addFunction("getProjectType", &ProjectTile::getProjectType)
		.addFunction("getOwner", &ProjectTile::getOwner)
		.addFunction("getTarget", &ProjectTile::getTarget)
		.addFunction("isDestiny", &ProjectTile::isDestiny)
		.addFunction("isDead", &ProjectTile::isDead)
		.endClass()
		.beginExtendClass<SlugObject, ProjectTile>("SlugObject")
		.addFunction("isDead", &SlugObject::isDead)
		.endClass()
		.beginExtendClass<Footboard, ProjectTile>("Footboard")
		.addFunction("isDead", &Footboard::isDead)
		.endClass()
		.beginExtendClass<Deadshot, ProjectTile>("Deadshot")
		.addFunction("isDead", &Deadshot::isDead)
		.endClass()
		.beginExtendClass<Trace, ProjectTile>("Trace")
		.addFunction("isDead", &Trace::isDead)
		.endClass();
	//������
	LuaBinding(l).beginClass<Trigger>("Trigger")
		.addFunction("isTouching", &Trigger::isTouching)
		.addFunction("scan", &Trigger::scanTouching)
		.addFunction("setActivation", &Trigger::setActivation)
		.endClass()
		.beginExtendClass<TriggerLine, Trigger>("TriggerLine")
		.addFunction("scan", &Trigger::scanTouching)
		.endClass()
		.beginExtendClass<TriggerCircle, Trigger>("TriggerCircle")
		.addFunction("scan", &Trigger::scanTouching)
		.endClass()
		.beginExtendClass<TriggerRect, Trigger>("TriggerRect")
		.addFunction("scan", &Trigger::scanTouching)

		.endClass();
	LuaBinding(l).beginClass<WeaponSystem>("WeaponSystem")
		.addFunction("shootAt", &WeaponSystem::shootAt)
		.addFunction("getCurrent", &WeaponSystem::getCurrentWeapon)
		.endClass();
	LuaBinding(l).beginClass<Weapon>("Weapon")
		.addFunction("unload", &Weapon::unload)
		.addFunction("setFireOn", &Weapon::setFireOn)
		.addFunction("setFireOff", &Weapon::setFireOff)
		.addFunction("isFiring", &Weapon::isFiring)
		.addFunction("registerWeapon", &Weapon::registerWeapon)
		.addFunction("reload", &Weapon::reload)
		.addFunction("getRef", &Weapon::getRef)
		.addFunction("getSlugLeft", &Weapon::getSlugLeft)
		.addFunction("shootAt", &Weapon::shootAt)
		//.endClass()
		//.beginExtendClass<Gun,Weapon>("Gun")
		.endClass();
	//��Դ��
	LuaBinding(l).beginClass<std::string>("String")
		.endClass();
	LuaBinding(l).beginClass<Luah>("Luah")
		.addStaticFunction("output", &Luah::output)
		.endClass();
	LuaBinding(l).beginClass<Clock>("Clock")
		.addStaticFunction("getCounter", &Clock::getTickInteger)
		.addStaticFunction("getTick", &Clock::getTickFloat)
		.endClass();
	//�ӳ�
	LuaBinding(l).beginClass<DelayCollection>("Delay")
		.addStaticFunction("isTimeUp", &DelayCollection::isTimeUp)
		.addStaticFunction("reset", &DelayCollection::reset)
		.endClass();
	//��Դ��������������¼
	LuaBinding(l).beginClass<Resh>("Resh")
		.addStaticVariable("Kingsure", &Resh::Kingsure)
		.addStaticFunction("openURL", &Resh::openURL)
		.addStaticFunction("playSound", &Resh::playSound)
		.addStaticFunction("playMusic", &Resh::playMusic)
		.addStaticFunction("resetAchieve", &Resh::resetAchieve)
		.addStaticFunction("setMusicVolume", &Resh::setMusicVolume)
		.addStaticFunction("getLanguage", &Resh::getLanguage)
		.addStaticFunction("getNumber", &Resh::getNumber)
		.addStaticFunction("addNumber", &Resh::addNumber)
		.addStaticFunction("setNumber", &Resh::setNumber)
		.addStaticFunction("getAchieveCount", &Resh::getAchieveCount)
		.addStaticFunction("getPlatform", &Resh::getPlatform)
		.addStaticFunction("reset", &Resh::reset)
			.addStaticFunction("getVisibleSize", &Resh::getVisibleSize)
		.endClass();
	LuaBinding(l).beginClass<Rect>("Rect")
		.addConstructor(LUA_ARGS(float, float, float, float))
		.addVariable("origin", &Rect::origin)
		.addFunction("getMinX", &Rect::getMinX)
		.addFunction("getMidX", &Rect::getMidX)
		.addFunction("getMaxX", &Rect::getMaxX)
		.addFunction("getMinY", &Rect::getMinY)
		.addFunction("getMidY", &Rect::getMidY)
		.addFunction("getMaxY", &Rect::getMaxY)
		//.endClass()
		//.beginExtendClass<BoundRect, Rect>("BoundRect")
		//.addVariableRef("gid", &BoundRect::gid)
		//.addVariableRef("tilePosition", &BoundRect::tilePosition)
		//.addFunction("getProperty", &BoundRect::getProperty)
		.endClass();
	LuaBinding(l).beginClass<BoundRect>("BoundRect")
		.addVariable("gid", &BoundRect::gid)
		.addVariable("tilePosition", &BoundRect::tilePosition)
		.addFunction("getProperty", &BoundRect::getProperty)
		.endClass();
	LuaBinding(l).beginClass<Clock>("Clock")
		.addStaticProperty("tick", &Clock::getTickFloat)
		.endClass();

	LuaBinding(l).beginClass<Vec2>("Vec2")
		.addVariableRef("x", &Vec2::x)
		.addVariableRef("y", &Vec2::y)
		.addStaticVariable("ZERO", &Vec2::ZERO)
		.addConstructor(LUA_ARGS(float, float))
		.endClass();
	//��ͼ
	LuaBinding(l).beginClass<TiledMap>("TiledMap")
		.addFunction("moveCameraX", &TiledMap::moveCameraX)
		.addFunction("setCameraY", &TiledMap::setCameraY)
		.addFunction("setCameraCenter", &TiledMap::setCameraCenter)
		.addFunction("setCameraSlide", &TiledMap::setCameraSlide)
		.addFunction("setCameraFrame", &TiledMap::setCameraFrame)
		.addFunction("setCameraRepeat", &TiledMap::setCameraRepeat)
		.addFunction("collapse", &TiledMap::collapse)
		.addFunction("exchangeMap", &TiledMap::exchangeMap)
		.addFunction("getWalls", &TiledMap::getWalls)
		.addFunction("setLayerGid", &TiledMap::setLayerGid)
		.addFunction("setObjectGid", &TiledMap::setObjectGid)
		.addFunction("setGid", &TiledMap::setGid)
		.addFunction("setGidRange", &TiledMap::setGidRange)
		//	.addFunction("reorderChild", &TiledMap::reorderChild)
		.addFunction("removeTile", &TiledMap::removeTile)
		.addFunction("removeTileRange", &TiledMap::removeTileRange)
		.addFunction("removeObjectRange", &TiledMap::removeObjectRange)
		.addFunction("removeLayerRange", &TiledMap::removeLayerRange)
		.addFunction("registerKnocks", &TiledMap::registerKnocks)
		.addFunction("getPtc", &TiledMap::getPositionByTileCoordinate)
		.addFunction("setPtc", &TiledMap::setPtc)
		.addFunction("getPositionY", &TiledMap::getPositionY)
		.addFunction("getPositionX", &TiledMap::getPositionX)
		.addFunction("getRange", &TiledMap::getRange)
		.endClass();
	//�Ի�ϵͳ
	LuaBinding(l).beginClass<DialogueSystem>("DialogueSystem")
		.addStaticFunction("proceed", &DialogueSystem::proceed)
		.addStaticFunction("update", &DialogueSystem::update)
		.addStaticFunction("close", &DialogueSystem::close)
		.addStaticFunction("setIndex", &DialogueSystem::setIndex)
		.addStaticFunction("isDone", &DialogueSystem::isDone)
		.endClass();
	//��ɫ������
	LuaBinding(l).beginClass<RoleManager>("RoleManager")
		.addStaticFunction("appendRole", &RoleManager::appendRole)
		.addStaticFunction("updateRole", &RoleManager::updateRole)
		.endClass();
	//��̬����
	LuaBinding(l).beginModule("Constant")
		.addVariable("User_Trial", &User_Trial)
		.addVariable("User_LastStage", &User_LastStage)


		.endModule();
	//��̬����
	LuaBinding(l).beginClass<Checkpoint>("Checkpoint")
		.addStaticFunction("setPosition", &Checkpoint::setPosition)
		.addStaticFunction("setPtc", &Checkpoint::setPtc)
		.addStaticFunction("setObject", &Checkpoint::setObject)
		.addStaticFunction("getObject", &Checkpoint::getObject)
		.addStaticFunction("addObject", &Checkpoint::addObject)
		.addStaticFunction("setTile", &Checkpoint::setTile)
		.addStaticFunction("addLife", &Checkpoint::addLife)
		.addStaticVariableRef("id", &Checkpoint::id)
		.addStaticVariableRef("position", &Checkpoint::position)
		.addStaticVariableRef("gid", &Checkpoint::gid)
		.addStaticVariableRef("mapID", &Checkpoint::mapID)
		.addStaticVariableRef("rangeStart", &Checkpoint::rangeStart)
		.addStaticVariableRef("rangeEnd", &Checkpoint::rangeEnd)
		.addStaticVariableRef("life", &Checkpoint::life)
		.addStaticVariableRef("trial", &Checkpoint::trialTimes)
		.addStaticFunction("updateTrial", &Checkpoint::updateTrial)
		.endClass();
	//��̬��������
	LuaBinding(l).beginClass<ActionExtend>("ActionExtend")
		.addStaticFunction("corrupt", &ActionExtend::corrupt)
		.addStaticFunction("toast", &ActionExtend::toast)
		.endClass();
	//��̬��������
	LuaBinding(l).beginClass<Ads>("Ads")
			.addStaticFunction("popup", &Ads::showInterstitial)
			.addStaticFunction("updateBanner", &Ads::updateBanner)
			.addStaticFunction("updateInterstitial", &Ads::updateInterstitial)
			.addStaticFunction("showBanner", &Ads::showBanner)
			.addStaticFunction("showReward", &Ads::showReward)
			.addStaticFunction("showVideo", &Ads::showVideo)
			.endClass();
	LuaBinding(l).beginClass<Size>("Size")
		.addConstructor(LUA_ARGS(float, float))
			.addVariable("width", &Size::width)
			.addVariable("height", &Size::height)
		.endClass();
	LuaBinding(l).beginClass<Joystick>("Joystick")
		.addFunction("getDirection", &Joystick::getDirection)
		.addFunction("getAngle", &Joystick::getAngle)
		.addFunction("getTouchedPad", &Joystick::getTouchedPad)
		.addFunction("getTouches", &Joystick::getTouches)
		.addFunction("setString", &Joystick::setString)
		.addFunction("setVisible", &Joystick::setVisible)
		.addFunction("setAllVisible", &Joystick::setAllVisible)
		.addFunction("hideRocker", &Joystick::hideRocker)
		.addFunction("showRocker", &Joystick::showRocker)
		.addFunction("hideJoystick", &Joystick::hideRocker)
		.addFunction("showJoystick", &Joystick::showRocker)
		.endClass();

	//�ṹ�� ֡ �û�ש�� �ƶ� ��˸ ��ǩ
	LuaBinding(l).beginClass<BlinkSystem>("BlinkSystem")
		.addStaticFunction("registerBlinks", &BlinkSystem::registerBlinks)
		.endClass();
	LuaBinding(l).beginClass<KnockPhase>("KnockPhase")
		.addVariable("direction", &KnockPhase::colliderDirection)
		.addVariable("knockAction", &KnockPhase::knockAction)
		.addVariable("times", &KnockPhase::times)
		.addVariable("isColliding", &KnockPhase::isColliding)
		.endClass();
	//LuaBinding(l).beginClass<FrameIndexes>("Frame")
	//	.addConstructor(LUA_ARGS(unsigned short, unsigned short, unsigned short))
	//	.endClass();
	LuaBinding(l).beginClass<FrameSwitch>("FrameSwitch")

		.addConstructor(LUA_ARGS(void))
		//.addVariable("frameIndex", &FrameIndexes::index)
		//armeabi gcc �����ø���FrameIndexesע��lua����������
		.addFunction("getIndex", &FrameSwitch::getIndex)
		//.endClass()
		//.beginExtendClass<FrameSwitch, FrameIndexes>("FrameSwitch")
		.addVariable("direction", &FrameSwitch::direction)
		.addVariable("touchedTimes", &FrameSwitch::touchedTimes)
		.addVariable("framePrefix", &FrameSwitch::framePrefix)
		.addVariable("isTouched", &FrameSwitch::isTouched)
		.endClass();
	
	LuaBinding(l).beginClass<SpringInfo>("SpringInfo") 
		.addVariable("isBounced", &SpringInfo::isBounced)
		.addVariable("force", &SpringInfo::force)
		.addVariable("times", &SpringInfo::times)
		.endClass();
	LuaBinding(l).beginClass<TilePropertyInfo>("TileInfo")
		.addVariable("isOneWay", &TilePropertyInfo::isOneWay)
		.addVariable("isHarmful", &TilePropertyInfo::isHarmful)
		.addVariable("allowThrough", &TilePropertyInfo::allowThrough)
		.addVariable("gid", &TilePropertyInfo::gid)
		.addVariable("inset", &TilePropertyInfo::inset)
		.endClass();

#pragma region Register enums
	//KnockAction
	lua_newtable(l);
	lua_pushliteral(l, "normal");
	lua_pushnumber(l, static_cast<short>(KnockAction::normal));
	lua_settable(l, -3);
	lua_pushliteral(l, "hollow");
	lua_pushnumber(l, static_cast<short>(KnockAction::hollow));
	lua_settable(l, -3);
	lua_pushliteral(l, "hidden");
	lua_pushnumber(l, static_cast<short>(KnockAction::hidden));
	lua_settable(l, -3);
	lua_pushliteral(l, "corrupt");
	lua_pushnumber(l, static_cast<short>(KnockAction::corrupt));
	lua_settable(l, -3);
	lua_pushliteral(l, "popup");
	lua_pushnumber(l, static_cast<short>(KnockAction::popup));
	lua_settable(l, -3);
	lua_pushliteral(l, "fetchChar");
	lua_pushnumber(l, static_cast<short>(KnockAction::fetchChar));
	lua_settable(l, -3);
	lua_setglobal(l, "KnockAction");

	lua_newtable(l);
	LUA_ENUM(l, limiteHorizon);
	LUA_ENUM(l, limiteVertical);
	LUA_ENUM(l, limiteBoth);
	lua_setglobal(l, "BorderLimited");

	lua_newtable(l);
	LUA_ENUM(l, stayStill);
	LUA_ENUM(l, toTop);
	LUA_ENUM(l, toBottom);
	LUA_ENUM(l, toLeft);
	LUA_ENUM(l, toRight);
	LUA_ENUM(l, toTopLeft);
	LUA_ENUM(l, toTopRight);
	LUA_ENUM(l, toBottomLeft);
	LUA_ENUM(l, toBottomRight);
	lua_setglobal(l, "MovingDirection");

	lua_newtable(l);
	LUA_ENUM(l, notLimited);
	LUA_ENUM(l, limiteHorizon);
	LUA_ENUM(l, limiteVertical);
	LUA_ENUM(l, limiteBoth);
	lua_setglobal(l, "BorderLimited");

	lua_newtable(l);
	LUA_ENUM(l, rangeOut);
	LUA_ENUM(l, rangeOutOfSight);
	LUA_ENUM(l, rangeSensed);
	LUA_ENUM(l, rangeThreatened);
	LUA_ENUM(l, rangeEndangered);
	lua_setglobal(l, "RangeType");

	lua_newtable(l);
	LUA_ENUM(l, atLeft);
	LUA_ENUM(l, atRight);
	LUA_ENUM(l, atTop);
	LUA_ENUM(l, atBottom);
	LUA_ENUM(l, intersected);
	lua_setglobal(l, "CollisionDirection");
	//Tile boundPosition
	lua_newtable(l);
	LUA_ENUM(l, top);
	LUA_ENUM(l, bottom);

	lua_pushliteral(l, "left");
	lua_pushnumber(l, BoundPosition::left);
	lua_settable(l, -3);

	lua_pushliteral(l, "right");
	lua_pushnumber(l, BoundPosition::right);
	lua_settable(l, -3);

	LUA_ENUM(l, topLeft);
	LUA_ENUM(l, topRight);
	LUA_ENUM(l, bottomLeft);
	LUA_ENUM(l, bottomRight);
	LUA_ENUM(l, centerSelf);
	lua_setglobal(l, "BoundPosition");
	//RoleType
	lua_newtable(l);
	lua_pushliteral(l, "player");
	lua_pushnumber(l, static_cast<short>(RoleType::player));
	lua_settable(l, -3);

	lua_pushliteral(l, "npc");
	lua_pushnumber(l, static_cast<short>(RoleType::npc));
	lua_settable(l, -3);

	lua_pushliteral(l, "robject");
	lua_pushnumber(l, static_cast<short>(RoleType::robject));
	lua_settable(l, -3);

	lua_pushliteral(l, "projectTile");
	lua_pushnumber(l, static_cast<short>(RoleType::projectTile));
	lua_settable(l, -3);

	lua_setglobal(l, "RoleType");

	//body status
	lua_newtable(l);
	lua_pushliteral(l, "healthy");
	lua_pushnumber(l, static_cast<short>(BodyStatus::healthy));
	lua_settable(l, -3);

	lua_pushliteral(l, "hurting");
	lua_pushnumber(l, static_cast<short>(BodyStatus::hurting));
	lua_settable(l, -3);

	lua_pushliteral(l, "dead");
	lua_pushnumber(l, static_cast<short>(BodyStatus::dead));
	lua_settable(l, -3);

	lua_pushliteral(l, "fall");
	lua_pushnumber(l, static_cast<short>(BodyStatus::fall));
	lua_settable(l, -3);

	lua_setglobal(l, "BodyStatus");

	//FrameStyle
	lua_newtable(l);
	lua_pushliteral(l, "idle");
	lua_pushnumber(l, static_cast<short>(FrameStyle::idle));
	lua_settable(l, -3);
	lua_pushliteral(l, "faceUP");
	lua_pushnumber(l, static_cast<short>(FrameStyle::faceUP));
	lua_settable(l, -3);
	lua_pushliteral(l, "faceDown");
	lua_pushnumber(l, static_cast<short>(FrameStyle::faceDown));
	lua_settable(l, -3);
	lua_pushliteral(l, "jump");
	lua_pushnumber(l, static_cast<short>(FrameStyle::jump));
	lua_settable(l, -3);
	lua_pushliteral(l, "fire");
	lua_pushnumber(l, static_cast<short>(FrameStyle::fire));
	lua_settable(l, -3);
	lua_pushliteral(l, "fireUP");
	lua_pushnumber(l, static_cast<short>(FrameStyle::fireUP));
	lua_settable(l, -3);
	lua_pushliteral(l, "fireDown");
	lua_pushnumber(l, static_cast<short>(FrameStyle::fireDown));
	lua_settable(l, -3);
	lua_pushliteral(l, "walkX");
	lua_pushnumber(l, static_cast<short>(FrameStyle::walkX));
	lua_settable(l, -3);
	lua_pushliteral(l, "walkUP");
	lua_pushnumber(l, static_cast<short>(FrameStyle::walkUP));
	lua_settable(l, -3);
	lua_pushliteral(l, "walkDown");
	lua_pushnumber(l, static_cast<short>(FrameStyle::walkDown));
	lua_settable(l, -3);
	lua_pushliteral(l, "hurt");
	lua_pushnumber(l, static_cast<short>(FrameStyle::hurt));
	lua_settable(l, -3);
	lua_pushliteral(l, "touched");
	lua_pushnumber(l, static_cast<short>(FrameStyle::touched));
	lua_settable(l, -3);
	lua_pushliteral(l, "die");
	lua_pushnumber(l, static_cast<short>(FrameStyle::die));
	lua_settable(l, -3);
	lua_setglobal(l, "FrameStyle");
	//Anistyle
	lua_newtable(l);

	lua_pushliteral(l, "toast");
	lua_pushnumber(l, static_cast<short>(Anistyle::toast));
	lua_settable(l, -3);

	lua_pushliteral(l, "fade");
	lua_pushnumber(l, static_cast<short>(Anistyle::fade));
	lua_settable(l, -3);
	lua_pushliteral(l, "rollOut");
	lua_pushnumber(l, static_cast<short>(Anistyle::rollOut));
	lua_settable(l, -3);
	lua_pushliteral(l, "explosion");
	lua_pushnumber(l, static_cast<short>(Anistyle::explosion));
	lua_settable(l, -3);
	lua_pushliteral(l, "hovering");
	lua_pushnumber(l, static_cast<short>(Anistyle::hovering));
	lua_settable(l, -3);
	lua_pushliteral(l, "rebound");
	lua_pushnumber(l, static_cast<short>(Anistyle::rebound));
	lua_settable(l, -3);
	lua_pushliteral(l, "rotate");
	lua_pushnumber(l, static_cast<short>(Anistyle::rotate));
	lua_settable(l, -3);
	lua_pushliteral(l, "jump");
	lua_pushnumber(l, static_cast<short>(Anistyle::jump));
	lua_settable(l, -3);
	lua_setglobal(l, "Anistyle");

	// CurtainType
	lua_newtable(l);

	lua_pushliteral(l, "black");
	lua_pushnumber(l, static_cast<short>(CurtainType::black));
	lua_settable(l, -3);
	lua_pushliteral(l, "bilateral");
	lua_pushnumber(l, static_cast<short>(CurtainType::bilateral));
	lua_settable(l, -3);
	lua_pushliteral(l, "updown");
	lua_pushnumber(l, static_cast<short>(CurtainType::updown));
	lua_settable(l, -3);
	lua_pushliteral(l, "drop");
	lua_pushnumber(l, static_cast<short>(CurtainType::drop));
	lua_settable(l, -3);
	lua_setglobal(l, "CurtainType");
	//SlidedResult
	lua_newtable(l);
	lua_pushliteral(l, "missed");
	lua_pushnumber(l, static_cast<short>(SlidedResult::missed));
	lua_settable(l, -3);
	lua_pushliteral(l, "left");
	lua_pushnumber(l, static_cast<short>(SlidedResult::left));
	lua_settable(l, -3);
	lua_pushliteral(l, "right");
	lua_pushnumber(l, static_cast<short>(SlidedResult::right));
	lua_settable(l, -3);
	lua_setglobal(l, "SlidedResult");

	//CameraView
	lua_newtable(l);
	lua_pushliteral(l, "horizontal");
	lua_pushnumber(l, static_cast<short>(CameraView::horizontal));
	lua_settable(l, -3);
	lua_pushliteral(l, "vertical");
	lua_pushnumber(l, static_cast<short>(CameraView::vertical));
	lua_settable(l, -3);
	lua_pushliteral(l, "both");
	lua_pushnumber(l, static_cast<short>(CameraView::both));
	lua_settable(l, -3);
	lua_setglobal(l, "CameraView");

	//MapView
	lua_newtable(l);
	lua_pushliteral(l, "panorama");
	lua_pushnumber(l, static_cast<short>(MapView::panorama));
	lua_settable(l, -3);
	lua_pushliteral(l, "horizontal");
	lua_pushnumber(l, static_cast<short>(MapView::horizontal));
	lua_settable(l, -3);
	lua_setglobal(l, "MapView");

	//CameraView
	lua_newtable(l);
	lua_pushliteral(l, "deadshot");
	lua_pushnumber(l, static_cast<short>(ProjectType::deadshot));
	lua_settable(l, -3);
	lua_pushliteral(l, "footboard");
	lua_pushnumber(l, static_cast<short>(ProjectType::footboard));
	lua_settable(l, -3);
	lua_pushliteral(l, "object");
	lua_pushnumber(l, static_cast<short>(ProjectType::object));
	lua_settable(l, -3);
	lua_pushliteral(l, "trace");
	lua_pushnumber(l, static_cast<short>(ProjectType::trace));
	lua_settable(l, -3);
	lua_setglobal(l, "ProjectType");
#pragma endregion
}
void Luah::preloadResources(const char* resTable)
{
	using namespace CocosDenshion;
	//��ԴԤ�ȼ���
	auto resources = getGlobal(resTable);

	CCASSERT(resources.isTable(), StringUtils::format("[Luah:preloadResources]������%s", resTable).c_str());
	for (auto iter = resources.begin(); iter != resources.end(); ++iter)
	{
		if (iter.value().has(Luaf_Sound))
			SimpleAudioEngine::getInstance()->preloadEffect(iter.value()[Luaf_Sound].value<const char*>());
		//ios �����ж������Ƿ񲥷�
		else if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() && iter.value().has(Luaf_Music))
			SimpleAudioEngine::getInstance()->preloadBackgroundMusic(iter.value()[Luaf_Music].value<const char*>());
		else if (iter.value().has(Luaf_Image))
			Director::getInstance()->getTextureCache()->addImage(iter.value()[Luaf_Image].value<std::string>());
		//	else if (iter.value().has(Luaf_Frame))
		 //
			//	assert(iter.value().has(Luaf_Rect) && "������rect");
				//	Resh::preloadFrame(iter.value()[Luaf_Frame].value<std::string>(), iter.value()[Luaf_Rect].value<Rect>());

		else if (iter.value().has(Luaf_Plist))
		{
			auto plist = iter.value()[Luaf_Plist].value<std::string>();
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist + ".plist", plist + ".png");
		}
	}
}
void Luah::registerRole(const char * className, const char * funcName, Role * role)
{
	LuaRef ref = createTable();
	ref.set(Luaf_CppRef, role);
	flush();
	callback(className, funcName, ref);
}

void Luah::processTable(const char * tableName, std::function<void(LuaIntf::LuaRef)> each, const bool& needAssert)
{
	auto luaTable = getGlobal(tableName);
	if (needAssert)
	{
		if (!luaTable.isTable())
		{//ע������еĻ����׳�
			CCLOG("������processTable%s", tableName);
			return;
		}
		
	}
	else
	{
		if (!luaTable.isTable())
			return;
	}

	//auto len = luaTable.len();
	for (auto iter = luaTable.begin(); iter != luaTable.end(); ++iter)
	{
		//�������ÿһ����table ��
	//	auto isValid = iter.value().isValid();
		if (iter.value().isTable())
			each(iter.value());
	}
}

void Luah::processTableFont(const char * tableName, std::function<void(LuaIntf::LuaRef, const FontInfo&)> each, const bool & needAssert)
{
	auto luaTable = getGlobal(tableName);
	if (needAssert)
	{
		if (!luaTable.isTable())
		{//ע������еĻ����׳�
			CCLOG("������processTable%s", tableName);
			return;
		}
	}
	else
	{
		if (!luaTable.isTable())
			return;
	}

	for (auto iter = luaTable.begin(); iter != luaTable.end(); ++iter)
	{
		FontInfo font(iter.value());
		each(iter.value(), font);
	}
}

LuaIntf::LuaRef Luah::getRole(const int & tag, const std::string & name)
{
	auto role = getGlobal(Luat_Role);
	CCASSERT(role.len() > 0, "role�����������1");

	if (0 == tag)
		return role;

	return "" == name ? role[tag].value() : role[tag].value()[name].value();
}

void Luah::callback(const char* className, const char* funcName)
{
	try
	{
		auto luaFunc = getGlobal( Luac_Callback);
		if (luaFunc.isFunction())
			luaFunc(className, funcName);

	}
	catch (LuaIntf::LuaException  e)
	{
		//gcc ++ format ����������
#if  CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT
		log(StringUtils::format("[%s:%s]%s", className, funcName, e.what()).c_str());
#else
		//gcc ++ format ����������
		log("callback error");
#endif
	}
}

void Luah::callback(const char* className, const char* funcName, const LuaRef&  params)
{
	try
	{
		auto luaFunc = getGlobal(Luac_Callback);
		if (luaFunc.isFunction())
			luaFunc(className, funcName, params);

	}
	catch (LuaIntf::LuaException  e)
	{

#if  CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT
		log(StringUtils::format("[%s:%s]%s", className, funcName, e.what()).c_str());
#else
		//gcc ++ format ����������
		log("callback error");
#endif
	}
}

void Luah::callmenu(const char* className, const char* funcName, const LuaRef& params)
{
	try
	{
		auto luaFunc = getGlobal( Luac_CallbackMenu);
		if (luaFunc.isFunction())
			luaFunc(className, funcName, params);

	}
	catch (LuaIntf::LuaException  e)
	{

#if  CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT
		log(StringUtils::format("[%s:%s]%s", className, funcName, e.what()).c_str());
#else
		//gcc ++ format ����������
		log("callmenu error");
#endif
	}
}
