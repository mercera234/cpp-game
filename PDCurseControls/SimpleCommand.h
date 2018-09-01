#pragma once

#include "Command.h"

template <class Receiver>
class SimpleCommand : public Command
{
private:
	typedef void (Receiver::* Action)(); //Action is a pointer to a method of Receiver
	typedef int (Receiver::* OneArgAction)(int);

	Receiver* receiver;
	Action action;
	OneArgAction oneArgAction;
public:
	SimpleCommand() {}
	~SimpleCommand() {}

	virtual void execute();
	virtual int execute(int);
	
	void setReceiver(Receiver* r);
	void setAction(Action a);
	void setOneArgAction(OneArgAction);
};

template <class Receiver>
void SimpleCommand<Receiver>::setReceiver(Receiver* r)
{
	receiver = r;
}

template <class Receiver>
void SimpleCommand<Receiver>::setAction(Action a)
{
	action = a;
}

template <class Receiver>
void SimpleCommand<Receiver>::setOneArgAction(OneArgAction a)
{
	oneArgAction = a;
}

template <class Receiver>
void SimpleCommand<Receiver>::execute()
{
	(receiver->*action)();
}

template <class Receiver>
int SimpleCommand<Receiver>::execute(int val)
{
	return (receiver->*oneArgAction)(val);
}

