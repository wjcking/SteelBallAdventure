#include "Role.h"
#include "RoleManager.h"
#include "../Map/MapManager.h"
#include "../Map/TiledMap.h"
#include "../Map/CheckPoint.h"
#include "ProjectTile.h"
#include "Player.h"
#include "Npc.h"
#include "RObject.h"
#include "../Utils/Constant.h"
#include "../Triggers/Trigger.h"
#include "../Triggers/TriggerSystem.h"
Player* RoleManager::player = nullptr;

/*��̬���ʼ��*/

RoleManager::RoleManager()
{
	entityMap.reserve(Reserve_Role);
}
RoleManager* RoleManager::getInstance()
{
	static RoleManager instance;
	return &instance;
}
Player& RoleManager::getPlayerRefer()
{
	if (nullptr == player)
		player = dynamic_cast<Player*>(ROLE_MANAGER->getRoleByTag(ID_Player));

	return *player;
}

void RoleManager::process(function<void(Role&)> roleAction)
{
	for (auto role : entityMap)
		roleAction(*role.second);
}

/*����id�ͷ����õ�����*/
Role* RoleManager::getRoleByTag(const int& id, bool allowAssert)const
{
	//find the entity
	auto ent = entityMap.find(id);

	if (allowAssert)
	{
		assert((ent != entityMap.end()) && "<RoleManager::GetEntityFromID>: û�д�ID");
		assert((ent->second != nullptr) && "role ��mapmanager ����Ϊ��");
		return  ent->second;
	}

	return ent != entityMap.end() ? ent->second : nullptr;
}

void RoleManager::removeRole(Role* entity)
{
	entityMap.erase(entityMap.find(entity->getTag()));
}

void RoleManager::registerRole(Role* newEntity)
{
	newEntity->generateTag();
	entityMap.insert(std::make_pair(newEntity->getTag(), newEntity));
}

void RoleManager::updateRole(const int & tag, const bool & isDisposed)
{
 
	auto role = ROLE_MANAGER->getRoleByTag(tag,false);
	if (nullptr == role)
		return;

	//һ��Ҫ��lua��ɾ���������ֻ��ᱨ��

	if (RoleType::player == role->getType())
	{
		auto player = dynamic_cast<Player*>(role);
		player->update();
	}
	else if (RoleType::npc == role->getType())
	{
		auto npc = dynamic_cast<Npc*>(role);
		npc->update();
	}
	else if (RoleType::robject == role->getType())
	{
		auto object = dynamic_cast<RObject*>(role);
		object->update();
	}
	else if (RoleType::projectTile == role->getType())
	{
		auto projectTile = dynamic_cast<ProjectTile*>(role);
		projectTile->update();

		if (isDisposed)
			role->getWeaponSystem().clear();
	}

	if (isDisposed)
	{
		if (nullptr != role->frameIndexes)
			delete[] role->frameIndexes;
		role->getMap()->removeChild(role);
		ROLE_MANAGER->removeRole(tag);
	}
}
void RoleManager::registerLuaRole(LuaIntf::LuaRef ref)
{
	assert(ref.has(Luaf_Type) && "�����ý�ɫ����");
	auto type = ref[Luaf_Type].value<RoleType>();
	unsigned short mapTag = 1;
	if (ref.has(Luaf_MapTag))
		mapTag = ref[Luaf_MapTag].value<unsigned short>();
	Role* role = nullptr;

	//1)����objectͼƬ�ļ�

	if (RoleType::player == type)
	{
		//1)����objectͼƬ�ļ�
		if (ref.has(Luaf_File))
		{
			auto file = ref[Luaf_File].value<std::string>();
			assert(FileUtils::getInstance()->isFileExist(file) && "�ļ�������");
			role = Role::createWithFileName<Player>(file);
		}

		else if (ref.has(Luaf_Frame))
		{
			auto frame = ref[Luaf_Frame].value<std::string>();
			role = Role::createWithFrameName<Player>(frame);
		}
		else
		{
			auto tile = ref.has(Luaf_Tile) ? ref[Luaf_Tile].value<Vec2>() : Vec2(0, 1);
			auto tileFrame = MAP_WITHTAG(mapTag)->getFrameWithTile(tile);
			role = Role::createWithFrame<Player>(tileFrame);
		}

		role->setLocalZOrder(Z_ROLE);
	}
	else if (RoleType::npc == type)
	{
		//1)����objectͼƬ�ļ�
		if (ref.has(Luaf_File))
		{
			auto file = ref[Luaf_File].value<std::string>();
			assert(FileUtils::getInstance()->isFileExist(file) && "�ļ�������");
			role = Role::createWithFileName<Npc>(file);
		}
		else if (ref.has(Luaf_Frame))
		{
			auto frame = ref[Luaf_Frame].value<std::string>();
			role = Role::createWithFrameName<Npc>(frame);
		}
		else
		{
			auto tile = ref.has(Luaf_Tile) ? ref[Luaf_Tile].value<Vec2>() : Vec2(0, 1);
			auto tileFrame = MAP_WITHTAG(mapTag)->getFrameWithTile(tile);
			role = Role::createWithFrame<Npc>(tileFrame);
		}

		role->setLocalZOrder(Z_Enemy);
	}
	else if (RoleType::robject == type)
	{
		//1)����objectͼƬ�ļ�
		if (ref.has(Luaf_File))
		{
			auto file = ref[Luaf_File].value<std::string>();
			assert(FileUtils::getInstance()->isFileExist(file) && "�ļ�������");
			role = Role::createWithFileName<RObject>(file);
		}
		else if (ref.has(Luaf_Frame))
		{
			auto frame = ref[Luaf_Frame].value<std::string>();
			role = Role::createWithFrameName<RObject>(frame);
		}
		else
		{
			auto tile = ref.has(Luaf_Tile) ? ref[Luaf_Tile].value<Vec2>() : Vec2(0, 1);
			auto tileFrame = MAP_WITHTAG(mapTag)->getFrameWithTile(tile);
			role = Role::createWithFrame<RObject>(tileFrame);
		}
		auto robject = dynamic_cast<RObject*>(role);
		//��ʼ������
		if (ref.has(Luaf_Spring))
			robject->registerSpring(ref[Luaf_Spring]);
		if (ref.has(Luaf_Knocks))
			robject->registerKnocks(ref.get(Luaf_Knocks));
		if (ref.has(Luaf_Text))
			robject->registerText(ref[Luaf_Text]);
		if (ref.has(Luaf_Char))
			robject->registerChar(ref[Luaf_Char]);
		if (ref.has(Luaf_Switch))
			robject->registerSwitch(ref[Luaf_Switch]);
		role->setLocalZOrder(Z_RObject);
	}
	else if (RoleType::projectTile == type)
	{
		//1)����objectͼƬ�ļ�
		if (ref.has(Luaf_File))
		{
			auto file = ref[Luaf_File].value<std::string>();
			assert(FileUtils::getInstance()->isFileExist(file) && "�ļ�������");
			role = Role::createWithFileName<ProjectTile>(file);
		}
		else if (ref.has(Luaf_Frame))
		{
			auto frame = ref[Luaf_Frame].value<std::string>();
			role = Role::createWithFrameName<ProjectTile>(frame);
		}
		else
		{
			auto tile = ref.has(Luaf_Tile) ? ref[Luaf_Tile].value<Vec2>() : Vec2(0, 1);
			auto tileFrame = MAP_WITHTAG(mapTag)->getFrameWithTile(tile);
			role = Role::createWithFrame<ProjectTile>(tileFrame);
		}
		role->setLocalZOrder(Z_ProjectTile);
	}
	role->mapTag = mapTag;
	role->getMap()->addChild(role);
	ROLE_MANAGER->registerRole(role);

	//���������������checkpoint����λ
	if (ref.has(Luaf_Pos))
		role->originMapPosition = ref[Luaf_Pos].value<Vec2>();
	//������Ƿ����� ,ע��offset
	if (RoleType::projectTile != type)
		role->spawn(role->originMapPosition, ref.get(Luaf_Offset, Vec2::ZERO));

	//1.���м��ص�ʱ������ 2.����������
	setProperty(ref, role);
	//��ɫ��ԭʼ��ʼ��λ�ã�Ҳ�����ڴ�����
	//������tile ��ȡ�ص�������Ӱ����ʼλ��
	//setAnimation �� ����õ���ʼλ��û�����Զ���ʼ��
	role->originPosition = role->getPosition();
	//[OBB]��ʼ������С��ʼ���Ժ��ܸı�,������ײ��׼ȷ
	role->originBound = role->getCollisionBound(role->insetObject);

	//��thisָ��
	ref.set(Luaf_Tag, role->getTag());
	ref.set(Luaf_CppRef, role);

	//����Ѿ������վ�����վ��ǰ��ȫ������
	if (Checkpoint::isChecked() && ref.has(Luaf_CheckpointID))
	{
		auto checkpointID = ref[Luaf_CheckpointID].value<short>();

		if (Checkpoint::id >= checkpointID)
			ref.set(Luaf_IsDisposed, true);
	}

	LUAH->flush();
}

void RoleManager::setProperty(LuaIntf::LuaRef ref, Role* role)
{
	if (ref.has(Luaf_FrameIndexes))
		role->registerFrameIndexes(ref.get(Luaf_FrameIndexes));
	//�������Ժ��ٲſ��Ը���prefix
	if (ref.has(Luaf_FramePrefix))
		role->framePrefix = ref.get(Luaf_FramePrefix).toValue<std::string>();
	if (ref.has(Luaf_AC))
		role->allowCollision(ref[Luaf_AC].value<CollisionDirection>());
	if (ref.has(Luaf_DC))
		role->disallowCollision(ref[Luaf_DC].value<CollisionDirection>());

	if (ref.has(Luaf_AllowCollision))
		role->allowCollision(ref[Luaf_AllowCollision].value<CollisionDirection>());
	if (ref.has(Luaf_DisallowCollision))
		role->disallowCollision(ref[Luaf_DisallowCollision].value<CollisionDirection>());

	role->setAllowFollow(ref.get(Luaf_AllowFollow, false));
	if (ref.has(Luaf_AllowPush))
		role->allowPush(ref[Luaf_AllowPush].value<CollisionDirection>());
	//��ͼΪˮƽ�ӽ����������������������ȫ�������ⲻ��Ҫ����
	if (role->getMap()->getViewType() == MapView::horizontal)
	{
		auto defGravity = (role->getType() == RoleType::robject) ? false : true;
		role->isGravityOn = ref.has(Luaf_AllowGravity) ? ref.get(Luaf_AllowGravity, defGravity) : defGravity;
		role->originGravity = role->isGravityOn;
	}
	else
	{
		role->isGravityOn = false;
		role->originGravity = false;
	}
	if (ref.has(Luaf_Health))
		role->hp = ref.get(Luaf_Health, 0);
	if (ref.has(Luaf_Gravity))
	{
		role->gravity = ref[Luaf_Gravity].value<Vec2>();
		role->defaultGravity = ref[Luaf_Gravity].value<Vec2>();
	}
	if (ref.has(Luaf_MoveStep))
	{
		role->moveStep = ref[Luaf_MoveStep].value<Vec2>();
		role->defaultMoveStep = role->moveStep;
	}
	if (ref.has(Luaf_JumpForce))
	{
		role->defaultJumpForce = ref[Luaf_JumpForce].value<Vec2>();
		role->jumpForce = ref[Luaf_JumpForce].value<Vec2>();
		role->defaultJumpCutOff = role->jumpForce;
		role->jumpCutoff = role->jumpForce;
	}
	if (ref.has(Luaf_InsetTile))
		role->insetTile = ref[Luaf_InsetTile].value<Vec2>();
	//ע��inset���ȼ�
	if (ref.has(Luaf_InsetObjectX))
		role->setInsetObjectX(ref[Luaf_InsetObjectX].value<float>());
	if (ref.has(Luaf_InsetObjectY))
		role->setInsetObjectY(ref[Luaf_InsetObjectY].value<float>());
	if (ref.has(Luaf_InsetObject))
		role->insetObject = ref[Luaf_InsetObject].value<Vec2>();
	if (ref.has(Luaf_OnCheckTile))
		role->onCheckTile = ref.get(Luaf_OnCheckTile).toValue<bool>();
	if (ref.has(Luaf_InsetTileX))
		role->setInsetTileX(ref[Luaf_InsetTileX].value<float>());
	if (ref.has(Luaf_InsetTileY))
		role->setInsetTileY(ref[Luaf_InsetTileY].value<float>());
	if (ref.has(Luaf_InsetTile))
		role->insetTile = ref[Luaf_InsetTile].value<Vec2>();

	if (ref.has(Luaf_IsSolid))
		role->setIsSolid(ref[Luaf_IsSolid].value<bool>());
	if (ref.has(Luaf_Visible))
		role->setVisible(ref[Luaf_Visible].value<bool>());
	if (ref.has(Luaf_AllowFlip))
		role->allowFlip = ref[Luaf_AllowFlip].value<bool>();
	//������ע���¼�
	if (ref.has(Luaf_AllowTouch))
		role->setAllowTouch(ref[Luaf_AllowTouch].value<bool>());

	if (ref.has(Luaf_AllowDragX))
		role->allowDragX = ref[Luaf_AllowDragX].value<bool>();
	if (ref.has(Luaf_AllowDragY))
		role->allowDragY = ref[Luaf_AllowDragY].value<bool>();
	//��ק
	if (ref.has(Luaf_DragStart))
		role->dragStart = ref[Luaf_DragStart].value<Vec2>();
	if (ref.has(Luaf_DragEnd))
		role->dragEnd = ref[Luaf_DragEnd].value<Vec2>();
	//�����Ǹ���ͼ
	if (ref.has(Luaf_MapTag))
		role->mapTag = ref[Luaf_MapTag].value<bool>();
	if (ref.has(Luaf_ZOrder))
		role->setLocalZOrder(ref[Luaf_ZOrder].value<int>());
	if (ref.has(Luaf_IsHarmful))
		role->setHarmful(ref[Luaf_IsHarmful].value<bool>());
	//��ʼ������
	if (ref.has(Luaf_Weapon))
		role->getWeapon()->registerWeapon(ref[Luaf_Weapon]);
	if (ref.has(Luaf_FrameSpeed))
		role->setFrameSpeed(ref.get(Luaf_FrameSpeed,0.12f));
	//��ʼ������
	if (ref.has(Luaf_Animate))
		role->setAnimation(ref[Luaf_Animate]);
	//��ɫ����
	role->facedDirection = ref.get(Luaf_Face, MovingDirection::toRight);
	if (role->facedDirection == MovingDirection::toLeft)
		role->setFlippedX(true);

}

void RoleManager::release()
{
	//�л�����������
	player = nullptr;

	for (auto w : entityMap)
	{
		w.second->stopAnimation();
		w.second->getWeaponSystem().clear();
		if (nullptr != w.second->frameIndexes)
			delete[] w.second->frameIndexes;
	}
	entityMap.clear();
}

void RoleManager::loadScript()
{
	auto entity = LUAH->getGlobal(Luat_Role);

	assert(entity.isTable() && "������entity");
	assert(entity.len() > 0 && "����Ҫ��һ����ɫ");
	//���ñ�ʶ
	Role::nextTag = 1;
	//���
	release();
	for (auto iter = entity.begin(); iter != entity.end(); ++iter)
	{
		registerLuaRole(iter.value());
	}
	//����ÿ��role�ű��ֿ���
	for (auto role : entityMap)
		role.second->loadScript();

}
//��̬����role
void RoleManager::appendRole(LuaIntf::LuaRef ref)
{
	CCASSERT(ref.isTable(), "RoleManager:appendRole error");
	auto roleList = LUAH->getGlobal(Luat_Role);
	registerLuaRole(ref);
	//���ܶ�ȡroletableֻ�����ӵ�ǰ��ref
}
