#pragma once
#include <string>

struct IMusicPlayer
{
	virtual bool playFile(const std::string& filePath) = 0;
	virtual bool pause() = 0;
	virtual bool stop() = 0;
	virtual bool play() = 0;
};