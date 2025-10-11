#include "../Role/Role.h"
#include "Trigger.h"
#include "TriggerSystem.h"
#include "../Utils/GameScriptor.h"
unsigned short TriggerSystem::triggerNext = 1;
unsigned short TriggerSystem::sensorNext = 1;
void TriggerSystem::updateTriggers()
{
	for (auto iter = trigerVector.begin(); iter != trigerVector.end(); ++iter)
	{
		//remove trigger if dead
		if ((*iter)->isRemoved())
		{
			delete *iter;
			iter = trigerVector.erase(iter);
			continue;
		}
	}
}

TriggerSystem::~TriggerSystem()
{
	clear();
}

short TriggerSystem::checkTrigger(const short & tag, Role & role)
{
	//	return trigerVector[tag]->scanTouching()
	return 0;
}

void TriggerSystem::scanTriggers(std::function<void(Trigger&)> each)
{
	auto triggers = LUAH->getGlobal(Luat_Triggers);
	LuaRef current;
	for (auto iter = trigerVector.begin(); iter != trigerVector.end();)
	{
		current = triggers[(*iter)->getTag()].value();

		if (current.has(Luaf_IsActive))
			(*iter)->setActivation(current.get(Luaf_IsActive, true));

		//remove trigger if dead
		if ((*iter)->isRemoved())
		{
			delete *iter;
			iter = trigerVector.erase(iter);
			continue;
		}
		each(**iter);
		++iter;
	}

}

void TriggerSystem::scanSensor(Role & opponent, std::function<void(Trigger&)> each)
{
	for (auto& iter : trigerVector)
	{
		if (iter->isActive())
		{	//���Ե����tag��Ϊrole��sensor,zh
			iter->scanTouching(opponent);
			each(*iter);
		}
	}
}
//this deletes any current sensorSystem and empties the trigger list
void TriggerSystem::clear()
{
	for (auto iter = trigerVector.begin(); iter != trigerVector.end(); ++iter)
	{
		delete *iter;
	}

	trigerVector.clear();
}

void TriggerSystem::addTrigger(Trigger* trigger)
{
	trigger->setTag(triggerNext++);
	trigerVector.push_back(trigger);
}

void TriggerSystem::addSensor(Trigger* sensor)
{
	sensor->setTag(sensorNext++);
	trigerVector.push_back(sensor);
}

void TriggerSystem::render(DrawNode* drawNode)
{
	for (auto iter : trigerVector)
	{
		iter->render(drawNode);
	}
}