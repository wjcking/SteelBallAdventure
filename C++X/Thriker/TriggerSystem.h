#ifndef TRIGGERSYSTEM_H
#define TRIGGERSYSTEM_H
#include <vector>
template <class entity_type>
class Trigger;
class Role;
#define TRIGGERS  TriggerSystem<Trigger<Role>>::getInstance()

template <class trigger_type>
class TriggerSystem
{
 
private:
	std::vector<trigger_type*>   triggers;
	//this method iterates through all the triggers present in the system and
	//calls their update method in order that their internal state can be
	//updated if necessary. It also removes any triggers from the system that
	//have their m_bRemoveFromGame field set to true.
	void updateTriggers()
	{
		typename  TriggerList::iterator curTrg = triggers.begin();

		while (curTrg != triggers.end())
		{
			//remove trigger if dead
			if ((*curTrg)->isRemoved())
			{
				delete *curTrg;

				curTrg = triggers.erase(curTrg);
			}
			else
			{
				(*curTrg)->update();
				++curTrg;
			}
		}
	}

	//this method iterates through the container of entities passed as a
	//parameter and passes each one to the scan method of each trigger *provided*
	//the entity is alive and provided the entity is ready for a trigger update.
	template <class ContainerOfEntities>
	void scanTriggers(ContainerOfEntities& entities)
	{
		//test each entity against the triggers
		typename   ContainerOfEntities::iterator curEnt = entities.begin();
		for (curEnt; curEnt != entities.end(); ++curEnt)
		{
			//an entity must be ready for its next trigger update and it must be 
			//alive before it is tested against each trigger.
		/*	if ((*curEnt)->isReadyForTriggerUpdate() && (*curEnt)->isAlive())
			{*/
				typename  TriggerList::const_iterator curTrg;
				for (curTrg = triggers.begin(); curTrg != triggers.end(); ++curTrg)
				{
					(*curTrg)->scan(*curEnt);
				}
			//}
		}
	}

public:

	static TriggerSystem* getInstance()
	{
		static TriggerSystem instance;
		return &instance;
	}
	~TriggerSystem()
	{
		clear();
	}

	//this deletes any current triggers and empties the trigger list
	void clear()
	{
	//	typename TriggerList::iterator curTrg;
		for (auto curTrg = triggers.begin(); curTrg != triggers.end(); ++curTrg)
		{
			delete *curTrg;
		}

		triggers.clear();
	}

	//This method should be called each update-step of the game. It will first
	//update the internal state odf the triggers and then try each entity against
	//each active trigger to test if any should be triggered.
	template <class ContainerOfEntities>
	void update(ContainerOfEntities& entities)
	{
		updateTriggers();
		scanTriggers(entities);
	}

	//this is used to register triggers with the TriggerSystem (the TriggerSystem
	//will take care of tidying up memory used by a trigger)
	void addTrigger(trigger_type& trigger)
	{
		 
		triggers.push_back(trigger);
	}

	//some triggers are required to be rendered (like giver-triggers for example)
	//void render()
	//{
	//  TriggerList::iterator curTrg;
	//  for (curTrg = triggers.begin(); curTrg != triggers.end(); ++curTrg)
	//  {
	//    (*curTrg)->Render();
	//  }
	//}

	inline  const std::vector<trigger_type*> & getTriggers()const{ return triggers; }

};


#endif