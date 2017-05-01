#include "BattleProcessor.h"
#include "LineItem.h"
#include <sstream>
#include "MenuItem.h"

BattleProcessor::BattleProcessor(WINDOW* win, list<Actor*>& players, list<Actor*>& enemies)
{
	setWindow(win);
	totalRows = getmaxy(win);//total row/col dimensions same as visible
	totalCols = getmaxx(win);

	humanActors = players;
	cpuActors = enemies;

	initTurnTracker(); 
	initTargetMenu();
	initSkillMenu();
	initMessageDisplay();
	initControlManager();
	
	advanceTurn();
}

/*
Must be done after actors have been setup
*/
void BattleProcessor::initTurnTracker()
{
	int totalPlayers = humanActors.size() + cpuActors.size();
	turnTracker = new TurnTracker(totalPlayers);
	turnTracker->addPlayers(humanActors);
	turnTracker->addPlayers(cpuActors);
}


void BattleProcessor::initTargetMenu()
{
	//setup target menu
	int actorCardHeight = 4; //this should maybe be calculated
	int totalCapacity = humanActors.size() + cpuActors.size();
	targetMenu = new GraphMenu(newwin(actorCardHeight * 4, visibleCols - 2, 1, 1), totalCapacity);


	//setup actor cards
	vector<MenuItem*>humanCards(humanActors.size());
	vector<MenuItem*>cpuCards(cpuActors.size());

	createActorCards(humanActors, 0, humanCards);
	createActorCards(cpuActors, humanCards.size(), cpuCards);

	//link cards
	MenuItem::linkItemGroup(humanCards, DOWN_LINK);
	MenuItem::linkItemGroup(cpuCards, DOWN_LINK);

	MenuItem::linkItemGroups(humanCards, cpuCards, RIGHT_LINK);
	
	//position cards
	int pLeft = 2;
	int eLeft = pLeft + (visibleCols / 2); //midway across screen

	MenuItem::positionItemGroup(humanCards, 0, pLeft);
	MenuItem::positionItemGroup(cpuCards, 0, eLeft);

	//add cards to target menu
	addCardsToTargetMenu(humanCards);
	addCardsToTargetMenu(cpuCards);
}


void BattleProcessor::addCardsToTargetMenu(vector<MenuItem*>& cards)
{
	for (int i = 0; i < cards.size(); i++)
	{
		targetMenu->setItem(cards[i]);
	}
}


void BattleProcessor::createActorCards(list<Actor*>& actors, int startNdx, vector<MenuItem*>& cards)
{
	int i = 0;
	int index = startNdx;
	for each (Actor* actor in actors)
	{
		cards[i++] = new ActorCard(actor, index++, -1);
	}
}


void BattleProcessor::initSkillMenu()
{
	//setup skill menu
	int skillMenuRows = 4;
	skillMenu = new GridMenu(newwin(skillMenuRows, visibleCols - 2, visibleRows - 5, 1), skillMenuRows, 1);

	//we shouldn't initialize and post here, this should be done based on which human's turn it is
	skillMenu->setItem(new LineItem("Attack", 0, -1));
	skillMenu->setItem(new LineItem("Magic", 1, -1));
	skillMenu->setItem(new LineItem("Item", 2, -1));
	skillMenu->setItem(new LineItem("Run", 3, -1));

	skillMenu->post(true);

	int frameHeight = 6;
	skillMenuFrame = new Frame(newwin(frameHeight, visibleCols, visibleRows - frameHeight, 0), skillMenu);
}

void BattleProcessor::initMessageDisplay()
{
	//set message display
	int msgDisplayRows = 4;
	msgDisplay = new TextLabel(newwin(msgDisplayRows, visibleCols - 2, visibleRows - 5, 1), "");
	msgDisplay->setJustification(JUST_CENTER);
}

void BattleProcessor::initControlManager()
{
	//setup controlmanager
	cm = new ControlManager(this);
	setControlManager(cm);
	cm->registerControl(targetMenu, KEY_LISTENER, controlCallback);
	cm->registerControl(skillMenuFrame, KEY_LISTENER, controlCallback);
//	cm->registerControl(skillMenu, KEY_LISTENER, controlCallback);
	cm->registerControl(msgDisplay, KEY_LISTENER, controlCallback); 
}

void BattleProcessor::advanceTurn()
{
	turnHolder = turnTracker->getNext();

	if (turnHolder == NULL) return;

	switch (turnHolder->type)
	{
	case AT_HUMAN:
		//cm->setFocus(skillMenu);
		skillMenuFrame->setText(turnHolder->def->name, 0, 4);
		cm->setFocus(skillMenuFrame);
		break;
	case AT_CPU:
		processCPUTurn();
		cm->setFocus(msgDisplay);
		setMessage();
		break;
	}
}


void BattleProcessor::draw()
{
	bool showMsgs = false;
	bool showSkillMenu = true;
	if (cm->getFocus() == msgDisplay)
	{
		showMsgs = true;
		showSkillMenu = false;
	}
	msgDisplay->setShowing(showMsgs);
	skillMenu->setShowing(showSkillMenu);

	cm->draw();
}


void BattleProcessor::controlCallback(void* caller, void* ptr, int input) //static
{
	BattleProcessor* bp = (BattleProcessor*)caller;
	bp->driver((Controllable*)ptr, input);
}

void BattleProcessor::driver(Controllable* control, int input)
{
	if (control == skillMenuFrame)
		skillMenuDriver(input);
	else if (control == targetMenu)
		targetMenuDriver(input);
	else if (control == msgDisplay)
		displayDriver(input);
}

void BattleProcessor::skillMenuDriver(int input)
{
	MenuItem* item = NULL;

	switch (input)
	{
	case KEY_DOWN: skillMenu->driver(REQ_DOWN_ITEM); break;
	case KEY_UP: skillMenu->driver(REQ_UP_ITEM); break;
	case KEY_LEFT: skillMenu->driver(REQ_LEFT_ITEM); break;
	case KEY_RIGHT: skillMenu->driver(REQ_RIGHT_ITEM); break;
	case 'c':
		item = skillMenu->getCurrentItem();
		break;
	default: break;
	}

	if (item != NULL)
	{
		switch (item->index)
		{
		case 0: //attack
			cm->setFocus(targetMenu);
			targetMenu->setCurrentItem(humanActors.size()); //current item is first enemy to start with
			break;
		case 3: //run
			cm->setActive(false);
			break;
		}
	}
}

void BattleProcessor::targetMenuDriver(int input)
{
	MenuItem* item = NULL;

	switch (input)
	{
	case KEY_DOWN: targetMenu->driver(REQ_DOWN_ITEM); break;
	case KEY_UP: targetMenu->driver(REQ_UP_ITEM); break;
	case KEY_LEFT: targetMenu->driver(REQ_LEFT_ITEM); break;
	case KEY_RIGHT: targetMenu->driver(REQ_RIGHT_ITEM); break;
	case 'c':
		item = targetMenu->getCurrentItem();
		break;
	case 'x':
		//cm->setFocus(skillMenu); 
		cm->setFocus(skillMenuFrame);
		break;
	default: break;
	}


	if (item != NULL) //target was chosen, apply skill
	{
		//only skill right now is attack so use that
		Actor* target = ((ActorCard*) item)->getActor();
		//Actor* turnHolder = humanActors.front(); //should have been set before populating skillMenu
		
		target->currHp -= turnHolder->def->strength;
		if (target->currHp < 0)
			target->currHp = 0;

		ostringstream attackMsg;
		attackMsg << turnHolder->def->name << " hits " << target->def->name;

		messages.push_back(attackMsg.str());

		//check if enemies are defeated
		bool allDead = true;
		for (list<Actor*>::iterator it = cpuActors.begin(); it != cpuActors.end(); it++)
		{
			Actor* enemy = *it;
			if (IS_DEAD(enemy) == false)
			{
				allDead = false; break;
			}

		}

		if (allDead)
		{
			setupVictory();
		}

		skillMenuFrame->setText("", 0, 4); //clear human's name from the frame
		cm->setFocus(msgDisplay);
		setMessage();
	}
}


void BattleProcessor::setupDeath()
{
	//disable TurnTracker
	turnTracker->setActive(false);

	messages.push_back("You died.");
}


void BattleProcessor::setupVictory()
{
	//disable TurnTracker
	turnTracker->setActive(false);

	//add messages
	messages.push_back("You killed the enemies.");

	//calculate experience and money earned
	int totalExp;
	int totalMoney;
	calcRewards(totalExp, totalMoney);

	ostringstream rewardMsg;
	rewardMsg << "Gained " << totalExp << " exp. Earned " << totalMoney << " gold $.";
	messages.push_back(rewardMsg.str());
}


void BattleProcessor::calcRewards(int& totalExp, int &totalMoney)
{
	totalExp = 0;
	totalMoney = 0;
	for (list<Actor*>::iterator it = cpuActors.begin(); it != cpuActors.end(); it++)
	{
		Actor* enemy = *it;
		totalExp += enemy->def->exp;
		totalMoney += enemy->def->money;
	}
}

void BattleProcessor::transferRewards(int totalExp, int totalMoney)
{
	for (list<Actor*>::iterator it = humanActors.begin(); it != humanActors.end(); it++)
	{
		Actor* player = *it;
		player->def->exp += totalExp;

		//not sure how to handle money just yet

		//check for level up here as well (not fully implemented yet)
		int levelsGained = 1;// = cc->checkLevelUp(c);

		//setup message for level ups
		if (levelsGained > 0)
		{
			ostringstream gainLevelMsg;
			gainLevelMsg << player->def->name;

			levelsGained == 1 ? 
			gainLevelMsg << " gained a level." :
			gainLevelMsg << " gained " << levelsGained << " levels.";

			messages.push_back(gainLevelMsg.str());
		}
	}
}

void BattleProcessor::displayDriver(int input)
{
	//input can only come from the acknowledgement key(we'll do others later)
	if (input != 'c')
		return;

	//advance acknowledgements until exhausted
	setMessage();

	if(messages.size() <= 0)  //no more messages left
	{
		advanceTurn();	
	}
}

/*
Return true if there was a message to print
*/
void BattleProcessor::setMessage()
{
	if (messages.size() <= 0)
		return;

	msgDisplay->setText(messages.front());
	messages.pop_front();
}


bool BattleProcessor::processInput(int input)
{
	if (turnHolder == NULL)
		return false; 

	return cm->handleInput(input);	
}

void BattleProcessor::processCPUTurn()
{
	//choose random human target
	int targetId = rand() % humanActors.size();
	list<Actor*>::iterator it;
	int seq = 0;
	Actor* target = NULL;
	for (it = humanActors.begin(); it != humanActors.end(); it++)
	{
		if (seq++ == targetId)
		{
			target = *it;
		}
	}
	//Actor* target = humanActors.front(); //only one for now

	//apply skill (just attack for now)
	target->currHp -= turnHolder->def->strength;

	if (target->currHp < 0)
		target->currHp = 0;

	//setup acknowledgement messages
	ostringstream attackMsg;
	attackMsg << turnHolder->def->name << " hits " << target->def->name;
	
	messages.push_back(attackMsg.str());

	//check if heros are defeated (this should be merged in with the other one!)
	bool allDead = true;
	for (list<Actor*>::iterator it = humanActors.begin(); it != humanActors.end(); it++)
	{
		Actor* human = *it;
		if (IS_DEAD(human) == false)
		{
			allDead = false; break;
		}

	}

	if (allDead)
	{
		setupDeath();
	}

	cm->setFocus(msgDisplay);
	setMessage();
}
