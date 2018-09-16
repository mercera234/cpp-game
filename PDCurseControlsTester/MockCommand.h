#pragma once
#include "Controllable.h"

template <class Receiver>
class MockCommand : public Command
{
private:
	typedef void (Receiver::* Action)();

	Receiver* receiver;
	Action action;
public:
	MockCommand() {}
	~MockCommand() {}

	virtual void execute();

	void setReceiver(Receiver* r);
	void setAction(Action a);
};


template <class Receiver>
void MockCommand<Receiver>::setReceiver(Receiver* r)
{
	receiver = r;
}

template <class Receiver>
void MockCommand<Receiver>::setAction(Action a)
{
	action = a;
}


template <class Receiver>
void MockCommand<Receiver>::execute()
{
	(receiver->*action)();
}
