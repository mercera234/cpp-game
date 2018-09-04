#pragma once
#include "SimpleControlCommand.h"

enum class ConfirmMethod
{
	NEW,
	OPEN,
	SAVE,
	QUIT
};


template <typename Receiver>
class ConfirmCommand : public SimpleControlCommand<Receiver>
{
	ConfirmMethod method;
public:
	ConfirmCommand();
	~ConfirmCommand();
	void setMethod(ConfirmMethod methodIn) { method = methodIn; }
	ConfirmMethod getMethod() { return method; }
};

template <typename Receiver>
ConfirmCommand<Receiver>::ConfirmCommand()
{
}

template <typename Receiver>
ConfirmCommand<Receiver>::~ConfirmCommand()
{
}

