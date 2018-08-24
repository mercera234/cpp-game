#include "Windows.h"
#include "MMSystem.h"
#include "MusicPlayer.h"

int MusicPlayer::playFile(const std::string& filePath)
{
	setFile(filePath);
	return play();
}


bool MusicPlayer::pause() //not implemented yet
{
	return true;
}


int MusicPlayer::stop()
{
	return PlaySound(NULL, 0, 0);
}

int MusicPlayer::play()
{
	if (file.compare("") == 0)
		return false;

	return PlaySound(TEXT(file.c_str()), 0, SND_FILENAME | SND_LOOP | SND_ASYNC);
}