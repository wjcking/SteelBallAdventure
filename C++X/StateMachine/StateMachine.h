#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#include <cassert>
#include <string>
#include "State.h"
#include "../Messaging/Telegram.h"
template <class entity_type>
class StateMachine
{
private:

	//a pointer to the agent that owns this instance
	entity_type*          owner;

	IState<entity_type>*   currentState;
	//a record of the last state the agent was in
	IState<entity_type>*   previousState;
	//this is called every time the FSM is updated
	IState<entity_type>*   globalState;
public:

	StateMachine(entity_type& owner) :owner(&owner),
		currentState(nullptr),
		previousState(nullptr),
		globalState(nullptr)
	{
	
	}
	StateMachine() 
	{

	}
	virtual ~StateMachine()
	{
	/*	delete owner;*/ 
		//delete currentState;
		//currentState = nullptr;
		//delete previousState;
		//previousState = nullptr;
		//delete globalState;
		//globalState = nullptr;
	}

	//use these methods to initialize the FSM
	inline void setCurrentState(IState<entity_type>&s){ currentState = &s; }
	inline void setGlobalState(IState<entity_type>& s) { globalState = &s; }
	inline void setPreviousState(IState<entity_type>& s){ previousState = &s; }


	IState<entity_type>&  getCurrentState()  const{ return currentState; }
	IState<entity_type>&  getGlobalState()   const{ return globalState; }
	IState<entity_type>&  getPreviousState() const{ return previousState; }

	//call this to update the FSM
	void  update()const
	{
		//if a global state exists, call its execute method, else do nothing
		if (globalState)
			globalState->execute(*owner);

		//same for the current state
		if (currentState)
			currentState->execute(*owner);
	}

	bool  handleMessage(const Telegram& msg)const
	{
		//first see if the current state is valid and that it can handle
		//the message
		if (currentState && currentState->onMessage(*owner, msg))
		{
			return true;
		}

		//if not, and if a global state has been implemented, send 
		//the message to the global state
		if (globalState && globalState->onMessage(*owner, msg))
		{
			return true;
		}

		return false;
	}
 
	//change to a new state
	void  changeState(IState<entity_type>& pNewState)
	{   
		 
		assert(nullptr != &pNewState && "<StateMachine::ChangeState>:trying to assign null state to current");

		//keep a record of the previous state
		previousState =  currentState;

		//call the exit method of the existing state
		if (nullptr != currentState)
		currentState->exit(*owner);

		//change state to the new state
		currentState =  &pNewState;
		 
		//call the entry method of the new state
		currentState->enter(*owner);
	}

	//change state back to the previous state
	void  revertToPreviousState()
	{
		changeState(*previousState);
	}

	//returns true if the current state's type is equal to the type of the
	//class passed as a parameter. 
	bool  isInState(const IState<entity_type>& st)const
	{
		if (typeid(*currentState) == typeid(st))
			return true;
		return false;
	}

	//only ever used during debugging to grab the name of the current state
	std::string  getNameOfCurrentState()const
	{
		std::string s(typeid(*currentState).name());

		//remove the 'class ' part from the front of the string
		if (s.size() > 5)
		{
			s.erase(0, 6);
		}

		return s;
	}
};
#endif


