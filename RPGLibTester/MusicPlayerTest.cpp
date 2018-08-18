#include "CppUnitTest.h"
#include "MockMusicPlayer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(MusicPlayerTest)
	{
		MockMusicPlayer musicPlayer;

		TEST_METHOD(playFile) //play a new sound file
		{
			Assert::IsTrue(musicPlayer.playFile("bossMusic.wav"));
		}

		TEST_METHOD(pause) //pause a currently playing track
		{
			musicPlayer.playFile("bossMusic.wav");
			Assert::IsTrue(musicPlayer.pause());
		}

		TEST_METHOD(play) //play a track that has been paused
		{
			musicPlayer.playFile("bossMusic.wav");
			musicPlayer.pause();
			Assert::IsTrue(musicPlayer.play());
		}

		TEST_METHOD(stop) //stop playing completely
		{
			musicPlayer.playFile("bossMusic.wav");
			Assert::IsTrue(musicPlayer.stop());
		}

		//optional, volume testing?
	};
}