#pragma once
#include "Thing.h"
#include "SimpleCommand.h"


template <typename Receiver>
struct Reactor : public Thing
{
	SimpleCommand<Receiver>* cmd; // a pointer to a pre-existing command
};

