#ifndef Trigger_Respawning_H
#define Trigger_Respawning_H

#include "Trigger.h"


class TriggerRespawning : public Trigger
{
protected:

  //When a sprite comes within this trigger's area of influence it is triggered
  //but then becomes inactive for a specified amount of time. These values
  //controlByKey the amount of time required to pass before the trigger becomes 
  //active once more.
  int   numUpdatesBetweenRespawns;
  int   numUpdatesRemainingUntilRespawn;

  //sets the trigger to be inactive for numUpdatesBetweenRespawns 
  //update-steps
  void deactivate()
  {
    setInactive();
    numUpdatesRemainingUntilRespawn = numUpdatesBetweenRespawns;
  }

public:

  TriggerRespawning(TriggerRegion* region):Trigger(region),
                             numUpdatesBetweenRespawns(0),
                             numUpdatesRemainingUntilRespawn(0)
  {}

  virtual ~TriggerRespawning(){}

  //to be implemented by child classes
   void  scan(Role& role)  override
  {
  };
  //this is called each game-tick to update the trigger's internal state
  virtual void update()
  {
    if ( (--numUpdatesRemainingUntilRespawn <= 0) && !isActive())
    {
      setActive();
    }
  }
  
  inline void setRespawnDelay(const unsigned int& numTicks)   {numUpdatesBetweenRespawns = numTicks;}
};




#endif