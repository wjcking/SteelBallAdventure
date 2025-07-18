#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <iostream>
#include <math.h>
//#include "Role\RoleManager.h"
struct Telegram
{
	int senderID;
	int receiverID;
	//"MessageTypes.h"
	int message;
	//messages can be dispatched immediately or delayed for a specified amount
	//of time. If a delay is necessary this field is stamped with the time 
	//the message should be dispatched.
	double dispatchTime;
	void* extraInfo;

 //   Role& getSender()  
	//{
	//	return *ROLE_MANAGER->getRoleByTag(senderID);
	//}
	//inline const Role& getReceiver()  
	//{
	//	return *ROLE_MANAGER->getRoleByTag(receiverID);
	//}
	Telegram() :dispatchTime(-1),
		senderID(-1),
		receiverID(-1),
		message(-1)
	{}


	Telegram(double time,
		int    sender,
		int    receiver,
		int    msg,
		void*  info = nullptr) : dispatchTime(time),
		senderID(sender),
		receiverID(receiver),
		message(msg),
		extraInfo(info)
	{}

};


//these telegrams will be stored in a priority queue. Therefore the >
//operator needs to be overloaded so that the PQ can sort the telegrams
//by time priority. Note how the times must be smaller than
//SmallestDelay apart before two Telegrams are considered unique.
const double SmallestDelay = 0.25;

inline bool operator==(const Telegram& t1, const Telegram& t2)
{
	return (fabs(t1.dispatchTime - t2.dispatchTime) < SmallestDelay) &&
		(t1.senderID == t2.senderID) &&
		(t1.receiverID == t2.receiverID) &&
		(t1.message == t2.message);
}

inline bool operator<(const Telegram& t1, const Telegram& t2)
{
	if (t1 == t2)
	{
		return false;
	}

	else
	{
		return  (t1.dispatchTime < t2.dispatchTime);
	}
}

inline std::ostream& operator<<(std::ostream& os, const Telegram& t)
{
	os << "time: " << t.dispatchTime << "  Sender: " << t.senderID << "   Receiver: " << t.receiverID << "   Msg: " << t.message;

	return os;
}

//handy helper function for dereferencing the ExtraInfo field of the Telegram 
//to the required type.
template <class T>
inline T dereferenceToType(void* p)
{ 
	return *(T*)(p);
}


#endif