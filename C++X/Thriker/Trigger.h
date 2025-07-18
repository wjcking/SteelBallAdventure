#ifndef TRIGGER_H
#define TRIGGER_H

#include "TriggerRegion.h"

struct Telegram;

template <class entity_type>
class Trigger
{
private:
	//Every trigger owns a trigger region. If an entity comes within this 
	//region the trigger is activated
	TriggerRegion* regionOfInfluence;
	//if this is true the trigger will be removed from the game
	bool toBeRemoved;
	//it's convenient to be able to deactivate certain types of triggers
	//on an event. Therefore a trigger can only be triggered when this
	//value is true (respawning triggers make good use of this facility)
	bool bActive;
	//some types of trigger are twinned with a graph node. This enables
	//the pathfinding component of an AI to search a navgraph for a specific
	//type of trigger.
	int graphNodeIndex;

protected:

	inline void setGraphNodeIndex(int idx){ graphNodeIndex = idx; }

	inline void remove(){ toBeRemoved = true; }
	inline void setInactive(){ bActive = false; }
	inline void setActive(){ bActive = true; }

	//returns true if the entity given by a position and bounding radius is
	//overlapping the trigger region
	bool isTouchingTrigger(const Rect& rect, double radius = 0) const;

	//child classes use one of these methods to initialize the trigger region
	void addCircular(const Rect&, double radius = 0);
	void addRectangular(const Rect&);
	void addLine(const Vec2&, const Vec2&);

public:

	Trigger(unsigned int id) :
		toBeRemoved(false),
		bActive(true),
		graphNodeIndex(-1),
		regionOfInfluence(nullptr)
	{
	}

	virtual ~Trigger(){ delete regionOfInfluence; }

	//when this is called the trigger determines if the entity is within the
	//trigger's region of influence. If it is then the trigger will be 
	//triggered and the appropriate action will be taken.
	virtual void  scan(entity_type&)
	{
	};

	//called each update-step of the game. This methods updates any internal
	//state the trigger may have
	virtual void  update()
	{
	};

	int  getNodeIndex()const{ return graphNodeIndex; }
	bool isRemoved()const{ return toBeRemoved; }
	bool isActive(){ return bActive; }
};

template <class entity_type>
void Trigger<entity_type>::addCircular(const Rect& center, double radius)
{
	//if this replaces an existing region, tidy up memory
	if (regionOfInfluence) 
		delete regionOfInfluence;

	regionOfInfluence = new TriggerCircle(center, radius);
}

template <class entity_type>
void Trigger<entity_type>::addRectangular(const Rect& rect)
{
	//if this replaces an existing region, tidy up memory
	if (regionOfInfluence) 
		delete regionOfInfluence;

	regionOfInfluence = new TriggerRectangle(rect);
}

template<class entity_type>
inline void Trigger<entity_type>::addLine(const Vec2 &, const Vec2 &)
{
}

template <class entity_type>
bool Trigger<entity_type>::isTouchingTrigger(const Rect& rect, double radius)const
{
	if (regionOfInfluence)
		return regionOfInfluence->isTouching(rect, radius);

	return false;
}


#endif