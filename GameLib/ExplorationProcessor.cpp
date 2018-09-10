#include "ExplorationProcessor.h"
#include <sstream>
#include "GameItem.h"
#include "defaults.h"

ExplorationProcessor::ExplorationProcessor()
{	
}


void ExplorationProcessor::setControlActor(Actor* controlActorIn)
{
	controlSprite.thing = controlActorIn;
	controlSprite.quantity = 1;
}

void ExplorationProcessor::setCursor(Pos cursorIn)
{
	cursor = cursorIn;

	MovementProcessor::setCursor(&cursor.y, &cursor.x);
	map.setCursor(&cursor.y, &cursor.x);
	setCurrMap(map.getCurrMapRoomId());
}



void ExplorationProcessor::setCurrMap(int id)
{
	if(screen == nullptr)
		screen = newwin(map.getUnitHeight(), map.getUnitWidth(), 0, 0);

	currMap = &resourceManager->mapRooms.find(id)->second;
	currMap->setWindow(screen);

	moveControl = currMap->getDisplay();
}

bool ExplorationProcessor::findMapExit(Boundary& edge, Movement& move )
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


bool ExplorationProcessor::processMovement(Movement& move)
{
	//move cursor by step
	moveCursor(move);


	//keep cursor within bounds for right now
	if (*curX < 0 ||
		*curY < 0 ||
		*curX >= map.getRealWidth() ||
		*curY >= map.getRealHeight())
	{
		reverseMovement(move); //later this should be replaced with searching for an exit to another map
		return false;
	}

	//check what new map id is and compare it to old one
	int currMapId = map.getCurrMapRoomId();

	if (currMapId != currMap->getId()) //map has changed
	{
		setCurrMap(currMapId);
	}
	else if (clipMode == false)//still on same map, and we're not clipping
	{
		if (processThingCollisions(move) == false) //false means move could not be completed
			return false;
		
		
		if (processTileEffect(move) == false) //false means move could not be completed
			return false;



	}

	adjustView();


	return true;
}

bool ExplorationProcessor::processThingCollisions(Movement& move)
{
	Pos roomPos = map.getMapRoomPos();
	Sprite* s = currMap->checkCollisionDetection(roomPos);
	if (s == nullptr)
		return true;

	if (GameItem* item = dynamic_cast<GameItem*>(s->thing))
	{
		switch (item->type)
		{
		case GameItemType::MONEY:
			resourceManager->theData.alterIntData(GOLD$, item->cost);
			currMap->things.remove(s);
			break;

			//all other types should be transferred to inventory
		/*case GameItemType::CONSUMABLE: 
			((Actor*)controlSprite.thing)->alterStat(StatType::HP, item->value);*/
			break;
		}
		currMap->things.remove(s);
	}
	else if (Actor* actor = dynamic_cast<Actor*>(s->thing))
	{
		//control.pos.x++; //do not allow movement on top of npc
		//move could not occur
		reverseMovement(move);
		return false;
	}
	
	return true;
}

bool ExplorationProcessor::processTileEffect(Movement& move)
{
	//check what character stepped on
	TwoDStorage<EffectType>& eLayer = currMap->getEffectsLayer();
	Pos mapCoords = map.getMapRoomPos();
	EffectType type = eLayer.getDatum(mapCoords.y, mapCoords.x);

	switch (type)
	{
	case EffectType::OBSTR:
		reverseMovement(move);
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
			reverseMovement(move);
			return false;
		}

		jumpGap++;
		Movement jumpMove(move.dir, move.magnitude);
		if (processMovement(jumpMove) == false) //move again to test next block
		{
			jumpGap = 0; //reset jumpGap
			reverseMovement(move);

			return false;
		}
		jumpGap = 0;
	}
		break;
	case EffectType::UP_EXIT: 
	{
		map.changeLayer(1);
		int currMapId = map.getCurrMapRoomId();

		if (currMapId != currMap->getId()) //map has changed
		{
			setCurrMap(currMapId);
		}
	}
		break;
	case EffectType::DOWN_EXIT: 
	{
		map.changeLayer(-1);
		int currMapId = map.getCurrMapRoomId();

		if (currMapId != currMap->getId()) //map has changed
		{
			setCurrMap(currMapId);
		}
	}
		break;
	}
	

	return true;
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



void ExplorationProcessor::moveActorAcrossMapSeam(MapExit& fromMap, MapExit& toMap)
{
	//!!method is either broken or obsolete now
//	Actor* controlActor = currMap->getControlActor(); //save controlActor

	setCurrMap(toMap.mapId);

	int unitMapSize = (toMap.edge == Boundary::WEST || toMap.edge == Boundary::EAST ) ? map.getUnitHeight() : map.getUnitWidth();

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
	currMap->draw();
	Image* display = currMap->getDisplay();

	if (controlSprite.thing != nullptr) //draw actor if present
	{
		Pos mapCoords = map.getMapRoomPos();
		int y = mapCoords.y - display->getUlY();
		int x = mapCoords.x - display->getUlX();
		
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