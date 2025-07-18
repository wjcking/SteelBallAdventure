#ifndef STATE_H
#define STATE_H

struct Telegram;

template <class entity_type>
class IState
{
public:

	virtual ~IState(){}

	virtual void enter(entity_type&) = 0;
	virtual void execute(entity_type&) = 0;
	virtual void exit(entity_type&) = 0;
	virtual bool onMessage(entity_type&, const Telegram&) = 0;
};

#endif