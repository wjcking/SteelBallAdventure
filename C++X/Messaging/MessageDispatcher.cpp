#include "MessageDispatcher.h"
#include "../Role/Role.h"
#include "../Utils/Constant.h" 
#include "../Role/RoleManager.h"
#include "../Role/Locations.h"
#include "MessageTypes.h"
#include "../Role/EntityNames.h"

#include <iostream>
using std::cout;
using std::set;

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif



//------------------------------ getInstance -------------------------------------

MessageDispatcher* MessageDispatcher::Instance()
{
	static MessageDispatcher instance;

	return &instance;
}


//----------------------------- Dispatch ---------------------------------
//  
//  see description in header
//------------------------------------------------------------------------
void MessageDispatcher::discharge(const int& receiver,
	const Telegram& telegram)
{
	auto pReceiver = ROLE_MANAGER->getRoleByTag(receiver);
	if (!pReceiver->handleMessage(telegram))
	{
		//telegram could not be handled
		cout << "Message not handled";
	}
}

//---------------------------- DispatchMessage ---------------------------
//
//  given a message, a receiver, a sender and any time delay , this function
//  routes the message to the correct agent (if no delay) or stores
//  in the message queue to be dispatched at the correct time
//------------------------------------------------------------------------
void MessageDispatcher::dispatchMessage(double  delay,
	int    sender,
	int    receiver,
	int    msg,
	void*  ExtraInfo )
{
	//SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	//get pointers to the sender and receiver
 

	//make sure the receiver is valid
	if (receiver  < 0)
	{
		cout << "\nWarning! No Receiver with ID of " << receiver << " found";

		return;
	}
	// double tc = DateUtils::getTickCount();
	//create the telegram
	Telegram telegram(0, sender, receiver, msg, ExtraInfo);

	//if there is no delay, route telegram immediately                       
	if (delay <= 0.0f)
	{
		cout << "\nInstant telegram dispatched at time: " << Clock::getTickFloat()
			<< " by " << getNameOfEntity(sender) << " for " << getNameOfEntity(receiver)
			<< ". Msg is " << getMessageText(msg);

		//send the telegram to the recipient

		discharge(receiver, telegram);
	}

	//else calculate the time when the telegram should be dispatched
	else
	{

		telegram.dispatchTime = Clock::getTickFloat() + delay;

		//and put it in the queue
		PriorityQ.insert(telegram);

		cout << "\nDelayed telegram from " << getNameOfEntity(sender) << " recorded at time "
			<< 1//Clock->GetCurrentTime()
			<< " for " << getNameOfEntity(receiver)
			<< ". Msg is " << getMessageText(msg);

	}
}


//---------------------- DispatchDelayedMessages -------------------------
//
//  This function dispatches any telegrams with a timestamp that has
//  expired. Any dispatched telegrams are removed from the queue
//------------------------------------------------------------------------
void MessageDispatcher::dispatchDelayedMessages()
{
	//SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	//get current time
	double CurrentTime = Clock::getTickFloat();

	//now peek at the queue to see if any telegrams need dispatching.
	//remove all telegrams from the front of the queue that have gone
	//past their sell by date
	while (!PriorityQ.empty() &&
		(PriorityQ.begin()->dispatchTime < CurrentTime) &&
		(PriorityQ.begin()->dispatchTime > 0))
	{
		//read the telegram from the front of the queue
		const Telegram& telegram = *PriorityQ.begin();

		//find the recipient 

		cout << "\nQueued telegram ready for dispatch: Sent to "
			<< getNameOfEntity(telegram.receiverID) << ". Msg is " << getMessageText(telegram.message);

		//send the telegram to the recipient
		discharge(telegram.receiverID, telegram);

		//remove it from the queue
		PriorityQ.erase(PriorityQ.begin());
	}
}



