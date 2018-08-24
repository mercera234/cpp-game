#pragma once
#include "IMusicPlayer.h"

class MusicPlayer : public IMusicPlayer
{
private:
	std::string file = "";
public:
	void setFile(const std::string& filePath) { file = filePath; }
	std::string& getFile() { return file; }
	int playFile(const std::string& filePath);
	bool pause();
	int stop();
	int play();

};