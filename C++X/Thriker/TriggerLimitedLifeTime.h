#ifndef TRIGGER_LIMITEDLIFETIME_H
#define TRIGGER_LIMITEDLIFETIME_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//  Desc:     defines a trigger that only remains in the game for a specified
//            number of update steps
//-----------------------------------------------------------------------------
#include "Trigger.h"
template <class entity_type>
class TriggerLimitedLifetime : public Trigger<entity_type>
{
protected:

  //the lifetime of this trigger in update-steps
  int lifeTime;

public:

  TriggerLimitedLifetime(int lifetime):Trigger<entity_type>(1),
                                        lifeTime(lifetime)
  {}

  virtual ~TriggerLimitedLifetime(){}
  //children of this class should always make sure this is called from within
  //their own update method
  virtual void update()
  {
    //if the lifetime counter expires set this trigger to be removed from
    //the game
    if (--lifeTime <= 0)
    {
      remove();
    }
  }

  //to be implemented by child classes
  virtual void  scan(entity_type*) = 0 ;
};




#endif