#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum MessageType
{
  msgTurnAround,
  msgJump,
  msgHitByBullets,
  msgHitTheHead,
  msgBeingPushed,
  msgObjectTouched,
  msgThreatened
};


inline std::string getMessageText(int msg)
{
  switch (msg)
  {
  case msgTurnAround:
    
    return "Turn around"; 

  case msgJump:
    
    return "Jump";

  default:

    return "Not recognized!";
  }
}

#endif