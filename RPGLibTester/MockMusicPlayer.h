#pragma once

#include "IMusicPlayer.h"

class MockMusicPlayer : public IMusicPlayer
{
private:
public:
	int playFile(const std::string& filePath);
	bool pause();
	int stop();
	int play();
};