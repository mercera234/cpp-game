#include "ExplorationProcessor.h"
#include <sstream>
#include "GameItem.h"
#include "GameInput.h"
#include "defaults.h"
#include "Reactor.h"
#include "actor_helper.h"
#include "Barrier.h"
#include "DialogWindow.h"
#include "TextBoard.h"
#include "LineFormat.h"

ExplorationProcessor::ExplorationProcessor()
{	
	exploreCmd.setReceiver(this);
	exploreCmd.setAction(&ExplorationProcessor::processExploreCmd);

	cm.registerControl(&room, KEY_LISTENER, &exploreCmd);
	cm.setFocusedControl(&room);
	
	dialogCmd.setReceiver(this);
	dialogCmd.setAction(&ExplorationProcessor::processDialogCmd);

	encounterTracker.setMinSteps(minPeaceSteps);
	encounterTracker.setMaxSteps(maxPeaceSteps);
	encounterTracker.setEncounterChance(encounterChance);
}

void ExplorationProcessor::processExploreCmd()
{
	int input = cm.getInput();

	if (isAlive(*resourceManager->getPlayer1()) == false && input != GameInput::OK_INPUT) //only accept ok input if player has died
		return;

	switch (input)
	{
	case GameInput::UP_INPUT:
	case GameInput::DOWN_INPUT:
	case GameInput::LEFT_INPUT:
	case GameInput::RIGHT_INPUT:
		processDirectionalInput(input);
		break;
	case GameInput::FIGHT_TRIGGER: //we will remove this later, but this is for triggering a fight immediately
	{
		inFight = true;
	}
	break;
	case GameInput::TOGGLE_ENCOUNTERS:
	{
		encounterTracker.setTracking(!encounterTracker.getTracking());
	}

	break;
	/*case GameInput::OK_INPUT:
		if (isAlive(player1) == false)
		{
			GameState* state = TitleScreenState::getInstance();
			manager.setState(state);
		}
		break;*/
	case GameInput::OPEN_MENU_INPUT:
	{
		inMenu = true;
	}

	break;
	}



}

void ExplorationProcessor::processDialogCmd()
{
	int input = cm.getInput();

	if (input == GameInput::OK_INPUT)
	{
		Controllable* c = cm.popControl();
		delete c;
		cm.setFocusedControl(&room);
	}
}

void ExplorationProcessor::processDirectionalInput(int input)
{
	int dirKey = -1;
	switch (input)
	{
		//These should all be only one move at a time
	case GameInput::UP_INPUT: dirKey = KEY_UP; break;
	case GameInput::DOWN_INPUT: dirKey = KEY_DOWN; break;
	case GameInput::LEFT_INPUT: dirKey = KEY_LEFT; break;
	case GameInput::RIGHT_INPUT: dirKey = KEY_RIGHT; break;
	}

	std::vector<Movement> moveChain = processMovementInput(dirKey);

	//verify that actor is still alive
	Actor* player1 = resourceManager->getPlayer1();
	if (isAlive(*player1) == false)
	{
		player1->symbol = nullSymbol;
		return;
	}

	Movement stepTaken = moveChain.back();
	if (moveChain.size() > 0)
	{
		processStepTaken(stepTaken);
	}
}

void ExplorationProcessor::processStepTaken(Movement& stepTaken)
{
	resourceManager->getData().alterIntData(STEPS, 1);

	encounterTracker.takeStep(stepTaken.axis);
	if (encounterTracker.didEncounterOccur())
	{
		inFight = true;
		
		encounterTracker.resetSteps(); //prepare for next time

		//get enemy pool from current map

		//get specific enemy group

		//pass enemies into BattleState
	}
}

ExplorationProcessor::~ExplorationProcessor()
{
	delwin(screen);
}

void ExplorationProcessor::setResourceManager(ResourceManager* resourceManagerIn)
{
	resourceManager = resourceManagerIn;
	map = resourceManager->currMap;
}

void ExplorationProcessor::setControlActor(Actor* controlActorIn)
{
	controlSprite.thing = controlActorIn;
	controlSprite.quantity = 1;
}

void ExplorationProcessor::setCursor(Pos& cursorIn)
{
	cursor = cursorIn;

	MovementProcessor::setCursor(&cursor.y, &cursor.x);
	map->setCursor(&cursor.y, &cursor.x);
	setCurrRoomId(map->getCurrMapRoomId());
}

void ExplorationProcessor::setCurrRoomId(int id)
{
	if(screen == nullptr)
		screen = newwin(map->getUnitHeight(), map->getUnitWidth(), 0, 0);

	MapRoom* currRoom = &resourceManager->getData().getRoom(id);
	currRoom->setWindow(screen);
	
	if (currRoom->getTotalTiles() <= 0)
	{
		FilePath path("data", currRoom);
		path.load(currRoom->name + ".map");
	}
	
	moveControl = &currRoom->getDisplay();

	room.control = currRoom;
}

int ExplorationProcessor::processInput(int input)
{
	cm.handleInput(input);
	return cm.getExitCode();
}




bool ExplorationProcessor::processMovement()
{
	//move cursor by step
	moveCursor();


	//keep cursor within map bounds for right now
	if (*curX < 0 ||
		*curY < 0 ||
		*curX >= map->getRealWidth() ||
		*curY >= map->getRealHeight())
	{
		reverseMovement(); //later this should be replaced with searching for an exit to another map
		return false;
	}

	//check what new room id is and compare it to old one
	int currMapId = map->getCurrMapRoomId();

	if (currMapId != ((MapRoom*)(room.control))->id) //map has changed
	{
		setCurrRoomId(currMapId);
	}
	else if (clipMode == false)//still on same map, and we're not clipping
	{
		if (processThingCollisions() == false) //false means move could not be completed
			return false;
		
		
		if (processTileEffect() == false) //false means move could not be completed
			return false;



	}

	//move was completed
	adjustView();

	//populate automap data

	return true;
}

bool ExplorationProcessor::processThingCollisions()
{
	Pos roomPos = map->getMapRoomPos();
	MapRoom* currRoom = (MapRoom*)room.control;

	objectSprite = currRoom->checkCollisionDetection(roomPos);
	if (objectSprite == nullptr) //no collisions
		return true;

	//check if s is passable
	if (objectSprite->impassible) //push into sprite
	{
		if (Actor* actor = dynamic_cast<Actor*>(objectSprite->thing))
		{
			//run through actor dialog then reverse the move
			reverseMovement();
			return false;
		}
	
		Reactor<ExplorationProcessor>* r = (Reactor<ExplorationProcessor>*)objectSprite->thing;
		r->cmd->execute();
	}
	else //interact with sprite
	{
		if (GameItem* item = dynamic_cast<GameItem*>(objectSprite->thing))
		{
			switch (item->type)
			{
			case GameItemType::MONEY:
				resourceManager->getData().alterIntData(GOLD$, (item->cost) * objectSprite->quantity);
				currRoom->sprites.remove(objectSprite);
				break;

			default:
			{
				resourceManager->acquireItem(item, objectSprite->quantity);
			}
				
				break;
			}
			currRoom->sprites.remove(objectSprite);
		}
	}
	return true;
}

void ExplorationProcessor::barrierRoutine()
{
	reverseMovement();
}

void ExplorationProcessor::signRoutine()
{
	reverseMovement();
	DialogWindow* post = new DialogWindow();
	
	post->setWindow(newwin(6, gameScreenWidth, 0, 0));
	TextBoard* board = new TextBoard;
	post->setControl(board);
	TextPiece* piece = new TextPiece(new LineFormat(1, Justf::CENTER), "The Test Region");
	board->addPiece(piece);

	cm.registerControl(post, KEY_LISTENER, &dialogCmd);
	cm.setFocusedControl(post);
}

void ExplorationProcessor::blockRoutine()
{
	//check if space behind block is unobstructed
	int pushAmount = currMove.magnitude;

	MapRoom* currRoom = (MapRoom*)room.control;
	ITwoDStorage<EffectType>& eLayer = currRoom->getEffectsLayer();
	Pos mapCoords = map->getMapRoomPos();

	Pos pushSpace = mapCoords;
	if (currMove.axis == Axis::HORIZONTAL)
	{
		pushSpace.x += pushAmount;
	}
	else 
	{
		pushSpace.y += pushAmount;
	}
	
	EffectType type = eLayer.getDatum(pushSpace.y, pushSpace.x);

	if (type == EffectType::NONE) //if so, then move is legal and block is pushed
	{
		objectSprite->pos = pushSpace;
	}
	else
		reverseMovement();
}

bool ExplorationProcessor::processTileEffect()
{
	MapRoom* currRoom = (MapRoom*)room.control;
	//check what character stepped on
	ITwoDStorage<EffectType>& eLayer = currRoom->getEffectsLayer();
	Pos mapCoords = map->getMapRoomPos();
	EffectType type = eLayer.getDatum(mapCoords.y, mapCoords.x);

	switch (type)
	{
	case EffectType::OBSTR:
		reverseMovement();
		return false;
		break;
	case EffectType::HP_DECREASE: alterActorHP(-1); break;
	case EffectType::HP_INCREASE:
	{
		Actor* actor = (Actor*)controlSprite.thing;
		alterActorHP(actor->getStat(StatType::HP).getCurrMax());
	}
	break;
	case EffectType::JUMPABLE: 
	{
		//TODO check that map control actor has jump capacity
		//check if tile beyond is passable
		if (jumpGap > 0) //can't cross 2 jump blocks in a row
		{
			reverseMovement();
			return false;
		}

		jumpGap++;
		movementChain.push_back(currMove);
		//Movement jumpMove(move.axis, move.magnitude);
		if (processMovement() == false) //move again to test next block
		{
			jumpGap = 0; //reset jumpGap
			reverseMovement();

			return false;
		}
		jumpGap = 0;
	}
		break;
	case EffectType::UP_EXIT: changeFloor(1); break;
	case EffectType::DOWN_EXIT: changeFloor(-1); break;
	}
	

	return true;
}

void ExplorationProcessor::changeFloor(int amount)
{
	MapRoom* currRoom = (MapRoom*)room.control;
	map->changeLayer(amount);
	int currMapId = map->getCurrMapRoomId();

	if (currMapId != currRoom->id) //map has changed
	{
		setCurrRoomId(currMapId);
	}
}

void ExplorationProcessor::alterActorHP(int amount)
{
	if (controlSprite.thing == nullptr)
		return;

	damage += amount;
	Actor* actor = (Actor*)controlSprite.thing;
	BoundInt& hp = actor->getStat(StatType::HP);

	hp.alterCurr(amount);
}

//bool ExplorationProcessor::processMovement_old(Movement& move)
//{
//	//move cursor by step
//	moveCursor(move);
//
//	//verify that cursor is within bounds
//	Boundary edge = inBounds();
//	if (edge != Boundary::IN_BOUNDS) //walked off edge
//	{
//		//search for exit for this map using the mapId, edge, and unit
//		findMapExit(edge, move);
//	}

bool ExplorationProcessor::findMapExit(Boundary& edge, Movement& move)
{
	//MapExit* sourceExit = new MapExit();
	//sourceExit->mapId = currMap->getId();
	//sourceExit->edge = edge;

	//Axis axis = getAxis(move.dir);

	//int* perpAxis = (axis == Axis::HORIZONTAL) ? curY : curX;
	//int unitMapSize = (axis == Axis::HORIZONTAL) ? mapRepo.getUnitMapHeight() : mapRepo.getUnitMapWidth();
	//sourceExit->unit = *perpAxis / unitMapSize;

	//MapExit* destExit = mapRepo.getExit(sourceExit);

	//if (destExit == nullptr) //no opening found, reverse the step
	//{
	//	reverseMovement(move);
	//	return false;
	//}

	//moveActorAcrossMapSeam(*sourceExit, *destExit); //it->second is destination map
	return true;
}



void ExplorationProcessor::moveActorAcrossMapSeam(MapExit& fromMap, MapExit& toMap)
{
	//!!method is either broken or obsolete now
//	Actor* controlActor = currMap->getControlActor(); //save controlActor

	setCurrRoomId(toMap.mapId);

	int unitMapSize = (toMap.edge == Boundary::WEST || toMap.edge == Boundary::EAST ) ? map->getUnitHeight() : map->getUnitWidth();

	int unitMultiplier = fromMap.unit > toMap.unit ? -unitMapSize : unitMapSize;
	int unitOffset = abs(fromMap.unit - toMap.unit) * unitMultiplier;

	switch (toMap.edge)
	{
	case Boundary::WEST:
		*curX = 0;
		*curY += unitOffset;
		break; //others below not done yet
	case Boundary::EAST:
		*curX = moveControl->getTotalCols() - 1;
		*curY += unitOffset;
		break;
	case Boundary::NORTH:
		*curY = 0;
		*curX += unitOffset;
		break;
	case Boundary::SOUTH:
		*curY = moveControl->getTotalRows() - 1;
		*curX += unitOffset;
		break;
	default:
		break;
	}

	setViewMode(ViewMode::DYNAMIC);
	//currMap->setControlActor(controlActor); //make sure new map has the same controlActor
}


void ExplorationProcessor::draw()
{
	cm.draw();
	
	MapRoom* currRoom = (MapRoom*)room.control;
	Image& display = currRoom->getDisplay();

	if (controlSprite.thing != nullptr) //draw actor if present
	{
		Pos mapCoords = map->getMapRoomPos();
		int y = mapCoords.y - display.getUlY();
		int x = mapCoords.x - display.getUlX();
		
		chtype mainCImageNormal = controlSprite.thing->symbol;

		TUI::printOnBkgd(mainCImageNormal, screen, y, x);

		if (damage != 0)
		{
			//build string from damage
			std::ostringstream oss;

			if (damage > 0) oss << '+'; //append + for positive values
			oss << damage;

			TUI::printStrOnBkgd(oss.str(), screen, y - 1, x + 1);

			damage = 0;
		}

		wnoutrefresh(screen);
	}
}