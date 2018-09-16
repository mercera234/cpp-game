#pragma once
#include <iostream>

struct Input
{
	std::string name;
	int code;
	bool secret; //true if input should not be seen by user(for debugging only)
	Input() {}
	Input(const std::string& nameIn, int codeIn, bool secretIn = false)
	{
		name = nameIn;
		code = codeIn;
		secret = secretIn;
	}
};

