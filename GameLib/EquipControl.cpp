#include <algorithm>
#include "EquipControl.h"
#include "game_strings.h"
#include "LineItem.h"
#include "TextParamValue.h"
#include "LineFormat.h"
#include "defaults.h"
#include "Divider.h"

EquipControl::EquipControl(Actor& actor)
{
	buildEquipMenu(actor);
	buildStatusBoard(actor);



}


void EquipControl::buildEquipMenu(Actor& actor)
{
	//build equipMenu
	auto& equipment = actor.getEquipment();
	equipMenu.resetItems(equipment.size(), 1);

	int i = 0;
	std::for_each(equipment.begin(), equipment.end(),
		[this, &i](std::pair<EquipPart, GameItem*> p)
	{
		std::string lbl = toString(p.first);
		lbl = lbl.substr(0, 6); //only 6 will fit

		equipMenu.setItem(new LineItem(lbl, i, -1), i, 0);
		i++;
	});

	equipMenu.setWrapAround(false);
	//equipMenu->setFocus(true);
	equipMenu.post(true);
	equipMenu.setCurrentItem(0);
}

void EquipControl::buildStatusBoard(Actor& actor)
{
	//build statusContent
	TextParamCurrMaxValue* hpRow, *mpRow;
	TextParamValue<BoundInt>* strengthRow, *defenseRow, *intelRow, *willRow, *agilityRow;

	//values are null, but will be setup later
	hpRow = new TextParamCurrMaxValue(new LineFormat(0, Justf::LEFT), HP, &actor.getStat(StatType::HP));
	mpRow = new TextParamCurrMaxValue(new LineFormat(1, Justf::LEFT), MP, &actor.getStat(StatType::MP), 4);
	strengthRow = new TextParamValue<BoundInt>(new LineFormat(2, Justf::LEFT), STRENGTH.substr(0, 3), &actor.getStat(StatType::STRENGTH));
	defenseRow = new TextParamValue<BoundInt>(new LineFormat(3, Justf::LEFT), DEFENSE.substr(0, 3), &actor.getStat(StatType::DEFENSE));
	intelRow = new TextParamValue<BoundInt>(new LineFormat(4, Justf::LEFT), INTELLIGENCE.substr(0, 3), &actor.getStat(StatType::INTELLIGENCE));
	willRow = new TextParamValue<BoundInt>(new LineFormat(5, Justf::LEFT), WILL.substr(0, 3), &actor.getStat(StatType::WILL));
	agilityRow = new TextParamValue<BoundInt>(new LineFormat(6, Justf::LEFT), AGILITY.substr(0, 3), &actor.getStat(StatType::AGILITY));
	statusBoard.addPiece(hpRow);
	statusBoard.addPiece(mpRow);
	statusBoard.addPiece(strengthRow);
	statusBoard.addPiece(defenseRow);
	statusBoard.addPiece(intelRow);
	statusBoard.addPiece(willRow);
	statusBoard.addPiece(agilityRow);
}

void EquipControl::setWindow(WINDOW* win)
{
	Controllable::setWindow(win);

	int eMenuHeight = equipMenu.getMaxItems();
	Rect eMenuRect(eMenuHeight, leftWidth, Pos(0, 0));

	equipMenu.setWindow(TUI::winMgr.deriveWin(win, eMenuRect));


	//
	Rect statRect(statusBoard.getPieceCount(), leftWidth - 4, Pos(eMenuHeight + 1, 4));
	statusBoard.setWindow(TUI::winMgr.deriveWin(win, statRect));


}

void EquipControl::processInput()
{





}

void EquipControl::draw()
{
	Divider h(Pos(getmaxy(equipMenu.getWindow()), 0), Axis::HORIZONTAL, leftWidth);
	Divider v(Pos(0, getmaxx(equipMenu.getWindow())), Axis::VERTICAL);

	h.draw(win);
	v.draw(win);
	wnoutrefresh(win);

	equipMenu.draw();
	statusBoard.draw();
}