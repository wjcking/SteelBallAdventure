#ifndef TRIGGERSYSTEM_H
#define TRIGGERSYSTEM_H 

#include "cocos2d.h"
class Role;
class Trigger;
class TriggerRegion;

class TriggerSystem
{
private:
	std::vector<Trigger*>   trigerVector; 
	void updateTriggers();
public:
	static unsigned short triggerNext;
	static unsigned short sensorNext ;
	~TriggerSystem();

	short checkTrigger(const short& tag, Role& role);
	void scanTriggers(std::function<void(Trigger&)>);
	//忽略掉相关tag作为role的sensor
	void scanSensor(Role& entity, std::function<void(Trigger&)>);
	void clear(); 
	void addTrigger(Trigger*);
	void addSensor(Trigger*);
	void render(DrawNode* drawNode);
	inline int  getCount() const { return  trigerVector.size(); };
};


#endif