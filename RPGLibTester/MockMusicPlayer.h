#pragma once

#include "IMusicPlayer.h"

class MockMusicPlayer : public IMusicPlayer
{
private:
public:
	bool playFile(const std::string& filePath);
	bool pause();
	bool stop();
	bool play();
};