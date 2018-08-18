#include "music_player_test.h"
#include "TUI.h"

void playFile()
{
	MusicPlayer musicPlayer;

	bool retval = musicPlayer.playFile("01-02- 12 Microtonal Etudes, Op 28 I 16 notes Andantino.wav");

	bool playing = true;
	bool musicPlaying = true;
	while (playing) 
	{
		//draw
		clear();
		if (musicPlaying)
		{
			mvaddstr(1, 1, musicPlayer.getFile().c_str());
			mvaddstr(2, 1, "s to stop");
		}
		else
		{
			mvaddstr(1, 1, "-");
			mvaddstr(2, 1, "(space) to play");
		}
			
		wnoutrefresh(stdscr);

		//input
		int input = getch();

		//process
		switch (input)
		{
		case KEY_ESC: playing = false; break;
		case 's': 
			musicPlayer.stop(); 
			musicPlaying = false;
			break;
		case ' ':
			musicPlayer.play(); 
			musicPlaying = true;
			break;
		default:
			break;
		}
	}
}