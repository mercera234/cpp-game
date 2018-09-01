#include "misc_tests.h"
#include "MainMenu.h"
#include "InputManager.h"
#include "defaults.h"
#include "GameInput.h"
#include "actor_helper.h"
#include "BattleProcessor.h"
#include "ResourceManager.h"

void mainMenuTest()
{
	Actor player1;
	initTestActor(player1);
	player1.name = "Test guy9012345";
	player1.stats.hp.setMax(100);
	player1.stats.hp.setCurr(97);

	Actor player2;
	initTestActor(player2);
	player2.name = "2nd test guy   ";
	player2.stats.hp.setMax(68);
	player2.stats.hp.setCurr(62);

	std::vector<Actor*> playerParty;
	playerParty.push_back(&player1);
	playerParty.push_back(&player2);

	MainMenu mm;
	mm.setWindow(newwin(screenHeight, screenWidth, 0, 0));
	mm.addPlayerParty(playerParty);

	InputManager inputManager;
	defaultGameInputs(inputManager);

	bool playing = true;
	while (playing)
	{
		mm.draw();
		doupdate();

		int input = getInput(inputManager);

		switch (input)
		{
		case KEY_ESC:
			playing = false;
			break;
		default:
		{
			playing = (mm.processInput(input) == HANDLED);
			
		}
		break;
		}
	}

	



}


void battleProcessorTest()
{
	bool testing = true;
	while (testing)
	{
		InputManager inputManager;
		defaultGameInputs(inputManager);

		//retrieve all actors from a wad file (just for testing, this should be refined)
		ResourceManager rm;

		std::ifstream is(actorFile);
		rm.loadActorsFromTextFile(is);

		Actor p1, p2, p3, p4;
		Actor e1, e2, e3, e4;

		p1 = rm.actors.find("Hero")->second;
		p2 = rm.actors.find("Lab Tech")->second;
		p3 = rm.actors.find("Navigator")->second;
		p4 = rm.actors.find("Mechanic")->second;

		p1.type = ActorType::HUMAN;
		p2.type = ActorType::HUMAN;
		p3.type = ActorType::HUMAN;
		p4.type = ActorType::HUMAN;

		std::list<Actor*> players;

		players.push_back(&p1);
	/*	players.push_back(&p2);
		players.push_back(&p3);*/
		//players.push_back(&p4);
		

		std::list<Actor*> enemies;

		e1 = rm.actors.find("Toadie")->second;
		e2 = rm.actors.find("Bigbug")->second;
		e3 = rm.actors.find("Skittler")->second;
		e4 = rm.actors.find("Wispwing")->second;
		
		e1.type = ActorType::CPU;
		e2.type = ActorType::CPU;
		e3.type = ActorType::CPU;
		e4.type = ActorType::CPU;

		enemies.push_back(&e1);
		/*enemies.push_back(&e2);
		enemies.push_back(&e3);
		enemies.push_back(&e4);*/
	

		int totalRows = 23;
		int totalCols = 51;
		resize_term(totalRows, totalCols);

		BattleProcessor bp;
		bp.setWindow(newwin(totalRows, totalCols, 0, 0));
		bp.addParticipants(players, enemies);
		bp.begin();

		

		int playing = NOT_HANDLED;
		while (playing <= 0)
		{
			bp.draw();
			doupdate();

			int input = getInput(inputManager);

			switch (input)
			{
			case GameInput::QUIT_INPUT:
				playing = TERMINATE;
				testing = false;
				break;
			default:
			{
				playing = bp.processInput(input);
			}
			break;
			}
		}
	}
}