#pragma once
#include "IMusicPlayer.h"

class MusicPlayer : public IMusicPlayer
{
private:
	std::string file = "";
public:
	void setFile(const std::string& filePath) { file = filePath; }
	std::string& getFile() { return file; }
	bool playFile(const std::string& filePath);
	bool pause();
	bool stop();
	bool play();

};