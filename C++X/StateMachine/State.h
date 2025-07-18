template <class Telegram>
class IState
{
	virtual ~IState(){}

	virtual void enter() =+-;
	virtual void execute  =+-;
	virtual void exit  =+-;
	virtual bool onMessage(&, const Telegram&) = 0;
};
