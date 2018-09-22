#pragma once

#include "Command.h"

template <class Receiver>
class SimpleCommand : public Command
{
private:
	typedef void (Receiver::* Action)(); //Action is a pointer to a method of Receiver

	Receiver* receiver;
	Action action;
public:
	SimpleCommand() {}
	SimpleCommand(Receiver* r, Action a);
	~SimpleCommand() {}

	virtual void execute();
	
	void setReceiver(Receiver* r);
	void setAction(Action a);

};

template <class Receiver>
void SimpleCommand<Receiver>::setReceiver(Receiver* r)
{
	receiver = r;
}

template <class Receiver>
SimpleCommand<Receiver>::SimpleCommand(Receiver* r, Action a)
{
	setReceiver(r);
	setAction(a);
}

template <class Receiver>
void SimpleCommand<Receiver>::setAction(Action a)
{
	action = a;
}

template <class Receiver>
void SimpleCommand<Receiver>::execute()
{
	(receiver->*action)();
}


