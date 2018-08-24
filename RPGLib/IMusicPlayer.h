#pragma once
#include <string>

struct IMusicPlayer
{
	virtual int playFile(const std::string& filePath) = 0;
	virtual bool pause() = 0;
	virtual int stop() = 0;
	virtual int play() = 0;
};