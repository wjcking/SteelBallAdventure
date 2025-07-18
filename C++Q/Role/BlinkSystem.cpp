#include "BlinkSystem.h"
#include "RObject.h"
#include "RoleManager.h"
std::unordered_map<unsigned short, BlinkInfo> BlinkSystem::blinkGroup = std::unordered_map<unsigned short, BlinkInfo>();
void BlinkSystem::registerBlinks(LuaIntf::LuaRef table)
{

	blinkGroup.clear();
	RObject* robject;
	int tag = 0;
	LuaRef ref;
	for (auto iter = table.begin(); iter != table.end(); ++iter)
	{
		ref = iter.value();
		//1.检测
		if (!ref.has(Luaf_Gid))
			return;
		if (!ref.has(Luaf_Tag))
			return;
		//2.info
		BlinkInfo bi;
		tag = ref[Luaf_Tag].value<int>();
		bi.groupID = ref[Luaf_Gid].value<short>();
		blinkGroup[bi.groupID].tags.push_back(tag);
		//3 phase
		robject = ROLE_MANAGER->getRole<RObject>(tag);

		if (nullptr == robject)
			return;
		BlinkPhase bp;
		bp = BlinkPhase(ref);
		bp.groupID = bi.groupID;//这里要赋值否则不能重置
		
		robject->setBlink(bp);
	}

}

void BlinkSystem::resetGroup(const unsigned short& gid)
{
	for (auto tag : blinkGroup[gid].tags)
	{
		auto object = ROLE_MANAGER->getRole<RObject>(tag);
		object->getBlink().reset();
	}

	blinkGroup[gid].reset();
}