#include "misc_tests.h"
#include "MainMenu.h"
#include "InputManager.h"
#include "defaults.h"
#include "GameInput.h"
#include "actor_helper.h"
#include "BattleProcessor.h"
#include "ResourceManager.h"
#include "ExplorationProcessor.h"
#include "MusicPlayer.h"
#include "GameItem.h"
#include "TitleScreen.h"
#include "ConfigMenu.h"
#include "Barrier.h"
#include "ItemBrowser.h"
#include "CenteredFormat.h"
#include "menu_drivers.h"
#include "EquipControl.h"

void titleScreenTest()
{
	TitleScreen titleScreen;

	ResourceManager rm;

	loadDataFiles(rm);

	bool playing = true;
	while (playing)
	{
		titleScreen.draw();
		doupdate();

		int input = rm.getInputManager().getInput();

		switch (input)
		{
		case GameInput::QUIT_INPUT:
			playing = false;
			break;
		default:
		{
			switch (titleScreen.processInput(input))
			{

			case ExitCode::TERMINATE: playing = false; break;
			}

		}
		break;
		}
	}
}




void inventoryTest()
{
	ItemBrowser browser;
	browser.setWindow(newwin(10, 20, 1, 1));

	Inventory inventory;

	GameItem potion;
	potion.name = "Potion";
	potion.description = "Restores 50 hp";

	GameItem knife;
	knife.name = "Knife";
	knife.description = "A weapon for stabbing";
	
	inventory.alterItemQuantity(&potion, 1);
	inventory.alterItemQuantity(&knife, 1);

	browser.setInventory(&inventory);


	Frame playerFrame;

	DialogBuilder builder;
	
	Frame descFrame;
	TextLabel descLbl;
	descLbl.setFormat(new CenteredFormat);
	descLbl.setWindow(newwin(4, 18, 12, 2));
	descFrame.setControl(&descLbl);	
	descFrame.setWindow(newwin(6, 20, 11, 1));


	InputManager mgr;
	loadHardCodedInputs(mgr.getInputs());
	setupDefaultGameInputs(mgr.getInputs());
	
	bool playing = true;
	while (playing)
	{
		descFrame.draw();
		browser.draw();
		doupdate();

		int cursesKey = getCursesKeyFromInput((GameInput)mgr.getInput());
		::processInput(browser, cursesKey);

		GameItem* item = browser.getCurrentItem();

		std::string desc = "";
		if (item != nullptr)
			desc = item->description;

		descLbl.setText(desc);
	}

}


GameItem* getUsableItem(const std::string& name, const std::string& desc = "")
{
	GameItem* item = new GameItem;
	item->name = name;
	item->description = desc;
	item->type = GameItemType::USABLE;

	return item;
}

GameItem* getEquippableItem(const std::string& name, EquipPart p)
{
	GameItem* item = new GameItem;
	item->name = name;
	item->part = p;
	item->type = GameItemType::EQUIPPABLE;

	return item;
}


void equipTest()
{
	Actor player1;
	initDefaultActor(player1);
	player1.name = "Test guy9012345";
	player1.alterStat(StatType::STRENGTH, 15);
	player1.alterStat(StatType::DEFENSE, 30);
	player1.alterStat(StatType::AGILITY, 25);
	player1.alterStat(StatType::INTELLIGENCE, 40);
	player1.alterStat(StatType::WILL, 51);
	
	GameItem item;
	item.name = "Sword";
	item.part = EquipPart::WEAPON;
	item.type = GameItemType::EQUIPPABLE;
	item.effects.statValues.insert(std::make_pair(StatType::STRENGTH, 11));

	Possession psn;
	psn.item = &item;
	psn.quantity.setCurr(1);
	
	player1.equip(&item);
	
	EquipControl* equipC = new EquipControl(player1);
	equipC->setWindow(TUI::winMgr.newWin(15, 49, 1, 1));
	

	DialogWindow dWin;
	dWin.setControl(equipC);
	dWin.setWindow(TUI::winMgr.newWin(17, 51, 0, 0));
	dWin.setFocus(true);

	ResourceManager rm;
	loadHardCodedInputs(rm.getInputManager().getInputs());
	setupDefaultGameInputs(rm.getInputManager().getInputs());

	Inventory& inv = rm.getInventory();
	

	GameItem* bambooStick = getEquippableItem("Bamboo Stick", EquipPart::WEAPON);
	bambooStick->effects.statValues.insert(std::make_pair(StatType::STRENGTH, 2));
	inv.alterItemQuantity(bambooStick, 5);

	inv.alterItemQuantity(getUsableItem("Potion"), 1);

	GameItem* utilities = getEquippableItem("Utilities", EquipPart::BODY);
	utilities->effects.statValues.insert(std::make_pair(StatType::DEFENSE, 6));
	utilities->effects.statValues.insert(std::make_pair(StatType::AGILITY, 4));
	inv.alterItemQuantity(utilities, 1);

	equipC->setInventory(rm.getInventory());

	bool playing = true;
	while (playing)
	{
		//equipC->draw();
		dWin.draw();
		doupdate();

		int input = rm.getInputManager().getInput();

		switch (input)
		{
		case GameInput::QUIT_INPUT:
			playing = false;
			break;
		default:
		{
			processInput(*equipC, input);
		}
			break;	
		}
	}
}


void mainMenuTest()
{
	Actor player1;
	initDefaultActor(player1);
	player1.name = "Test guy9012345";
	player1.getStat(StatType::HP).setCurrMax(100);
	player1.getStat(StatType::HP).setCurr(97);

	Actor player2;
	initDefaultActor(player2);
	player2.name = "2nd test guy   ";
	player2.getStat(StatType::HP).setCurrMax(68);
	player2.getStat(StatType::HP).setCurr(62);

	ResourceManager rm;
	GameData& data = rm.getData();

	loadDataFiles(rm);

	rm.playerParty.push_back(player1);
	rm.playerParty.push_back(player2);

	//load one megamap so we can test that it's properties display in the main menu
	rm.currMap = &data.getMap("TestRegion");
	rm.currMap->setUnitHeight(gameScreenHeight);
	rm.currMap->setUnitWidth(gameScreenWidth);

	Pos pos(32, 54);
	rm.currMap->setCursor(&pos.y, &pos.x);
	rm.currMap->visitArea();

	GameItem megalixer;
	megalixer.name = "Megalixer";
	megalixer.effects.statValues.insert(std::make_pair(StatType::HP, 9999));
	megalixer.effects.statValues.insert(std::make_pair(StatType::MP, 9999));
	megalixer.description = "Heals all";


	Inventory& inv = rm.getInventory();
	inv.alterItemQuantity(&data.getItem("Potion"), 3);
	inv.alterItemQuantity(&data.getItem("Knife"), 1);
	inv.alterItemQuantity(&megalixer, 5);

	MainMenu mm(&rm);
	mm.setWindow(TUI::winMgr.newWin(gameScreenHeight, gameScreenWidth, 0, 0));

	
	bool playing = true;
	while (playing)
	{
		mm.draw();
		doupdate();

		int input = rm.getInputManager().getInput();

		switch (input)
		{
		case GameInput::QUIT_INPUT:
			playing = false;
			break;
		case GameInput::FIGHT_TRIGGER: //just testing
			data.updateIntData("Gold$", 50);
			break;
		default:
		{
			ExitCode code = processInput(mm, input);
			if (code != ExitCode::HANDLED)
				playing = false;
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
		ResourceManager rm;
		GameData& data = rm.getData();
		loadDataFiles(rm);

		Actor p1, p2, p3, p4;
		Actor e1, e2, e3, e4;

		
		p1 = data.getActor("Hero");
		p2 = data.getActor("Lab Tech");
		p3 = data.getActor("Navigator");
		p4 = data.getActor("Mechanic");

		p1.setType(ActorType::HUMAN);
		p2.setType(ActorType::HUMAN);
		p3.setType(ActorType::HUMAN);
		p4.setType(ActorType::HUMAN);

		std::list<Actor*> players;

		players.push_back(&p1);
		players.push_back(&p2);
		players.push_back(&p3);
		players.push_back(&p4);
		

		std::list<Actor*> enemies;

		e1 = data.getActor("Toadie");
		e2 = data.getActor("Bigbug");
		e3 = data.getActor("Skittler");
		e4 = data.getActor("Wispwing");
		
		e1.setType(ActorType::CPU);
		e2.setType(ActorType::CPU);
		e3.setType(ActorType::CPU);
		e4.setType(ActorType::CPU);

		enemies.push_back(&e1);
		enemies.push_back(&e2);
		enemies.push_back(&e3);
		enemies.push_back(&e4);
	

		int totalRows = 23;
		int totalCols = 51;
		resize_term(totalRows, totalCols);

		BattleProcessor bp;
		bp.setResourceManager(&rm);
		bp.setWindow(newwin(totalRows, totalCols, 0, 0));
		bp.addParticipants(players, enemies);
		bp.begin();

		

		int playing = NOT_HANDLED;
		while (playing <= 0)
		{
			bp.draw();
			doupdate();

			int input = rm.getInputManager().getInput();

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

//only saving this for musicplayer code
void exploreMegaMapTest()
{
	//MusicPlayer musicPlayer;
	//std::string song1 = "01-02- 12 Microtonal Etudes, Op 28 I 16 notes Andantino.wav";
	//std::string song2 = "11 CONTEMPLATIVE INTERMISSION.wav";
	//int ret = musicPlayer.playFile(song1);

	

	//	//update music if changing maps
	//	if ((currId <= 1) && mp.getCurrMap()->getId() == 2)
	//	{
	//		musicPlayer.stop();
	//		musicPlayer.playFile(song2);
	//		currId = mp.getCurrMap()->getId();
	//	}
	//	else if (currId == 2 && mp.getCurrMap()->getId() <= 1)
	//	{
	//		musicPlayer.stop();
	//		musicPlayer.playFile(song1);
	//		currId = mp.getCurrMap()->getId();
	//	}
}


void exploreOneMapTest()
{
	resize_term(gameScreenHeight, gameScreenWidth);

	bool playing = true;

	ResourceManager rm;
	loadDataFiles(rm);
	GameData& data = rm.getData();

	//setup main character
	Actor& player1 = data.getActor(player1Name);
	player1.setType(ActorType::HUMAN);

	rm.playerParty.push_back(player1);

	

	
	rm.currMap = &data.getMap("TestRegion");
	rm.currMap->setUnitHeight(gameScreenHeight);
	rm.currMap->setUnitWidth(gameScreenWidth);
	rm.currMap->setFloorIndex(0);
	
	
	ExplorationProcessor mp;
	mp.setResourceManager(&rm);
	mp.setControlActor(&player1);
	Pos pos(32, 54);
	mp.setCursor(pos);
	mp.setViewMode(ViewMode::DYNAMIC); //position map so character is visible (not sure if this is the best way to do this)

	Sprite s;
	s.pos.y = 13;
	s.pos.x = 23;
	s.quantity = 5;
	s.symbol = '$' | COLOR_YELLOW_BOLD << TEXTCOLOR_OFFSET;
	s.thing = &data.getItem(GOLD$);
	s.impassible = false;

	Sprite s2;
	s2.pos.y = 13;
	s2.pos.x = 33;
	s2.quantity = 1;
	s2.symbol = '*' | COLOR_MAGENTA_BOLD << TEXTCOLOR_OFFSET;
	s2.thing = &data.getItem("Potion");
	s2.impassible = false;

	Sprite s3;
	s3.pos.y = 2;
	s3.pos.x = 2;
	s3.height = 3;
	s3.width = 4;
	s3.impassible = true;
	s3.quantity = 1;
	s3.symbol = '&';

	SimpleCommand<ExplorationProcessor> barrierCmd;
	barrierCmd.setReceiver(&mp);
	barrierCmd.setAction(&ExplorationProcessor::barrierRoutine);

	Reactor<ExplorationProcessor> barrier;
	barrier.cmd = &barrierCmd;

	s3.thing = &barrier;

	Sprite s4;
	s4.pos.y = 7;
	s4.pos.x = 40;
	s4.impassible = true;
	s4.quantity = 1;
	s4.symbol = '!';

	SimpleCommand<ExplorationProcessor> signCmd;
	signCmd.setReceiver(&mp);
	signCmd.setAction(&ExplorationProcessor::signRoutine);

	Reactor<ExplorationProcessor> sign;
	sign.cmd = &signCmd;

	s4.thing = &sign;

	Sprite s5;
	s5.pos.y = 7;
	s5.pos.x = 30;
	s5.impassible = true;
	s5.quantity = 1;
	s5.symbol = '#' | COLOR_BLUE << TEXTCOLOR_OFFSET;

	SimpleCommand<ExplorationProcessor> blockCmd;
	blockCmd.setReceiver(&mp);
	blockCmd.setAction(&ExplorationProcessor::blockRoutine);

	Reactor<ExplorationProcessor> block;
	block.cmd = &blockCmd;

	s5.thing = &block;

	MapRoom& itemRoom = data.getRoom("Labyrinth2");
	
	WINDOW* screen = mp.getScreen();
	while (playing)
	{
		//draw map
		mp.draw();

		//add y,x coordinates to screen
		Pos cursor = mp.getCursor();
		mvwprintw(screen, gameScreenHeight - 2, gameScreenWidth - 16, "y:%+4u x:%+4u", cursor.y, cursor.x);
		wnoutrefresh(screen);

		doupdate();

		//process input
		int input = rm.getInputManager().getInput();
		switch (input)
		{
		case GameInput::QUIT_INPUT: playing = false; break;
		//case '\t': //toggle automap
		//	break;
	/*	case '+': mp.setClipMode(true); break;
		case '-': mp.setClipMode(false); break;*/
		default:
			mp.processInput(input); break;
			break;
		}
	}
}



void configMenuTest()
{
	ResourceManager rm;
	loadDataFiles(rm);

	ConfigMenu menu;
	WINDOW* win = TUI::winMgr.newWin(10, 20, 1, 1);
	wbkgd(win, COLOR_RED << BKGDCOLOR_OFFSET);
	menu.setWindow(win);
	menu.setResourceManager(&rm);
	

	bool playing = true;
	while (playing)
	{
		menu.draw();
		doupdate();
		int c = rm.getInputManager().getInput();

		switch(c)
		{
		case QUIT_INPUT: playing = false; continue;
		default:
			menu.processInput(c);
		}

		
	}
	
}