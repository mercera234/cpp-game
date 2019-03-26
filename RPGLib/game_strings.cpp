#include "game_strings.h"

std::string toString(StatType type)
{
	std::string str = "";

	switch (type)
	{
	case StatType::HP: str = "HP"; break;
	case StatType::MP: str = "MP"; break;
	case StatType::STRENGTH: str = "Strength"; break;
	case StatType::DEFENSE: str = "Defense"; break;
	case StatType::AGILITY: str = "Agility"; break;
	case StatType::WILL: str = "Will"; break;
	case StatType::INTELLIGENCE: str = "Intelligence"; break;
	case StatType::EXP: str = "Exp"; break;
	case StatType::LEVEL: str = "Level"; break;
	}

	return str;
}

std::string toString(EquipPart part)
{
	std::string str = "";

	switch (part)
	{
	case EquipPart::WEAPON: str = "Weapon"; break;
	case EquipPart::HEAD: str = "Head"; break;
	case EquipPart::BODY: str = "Torso"; break;
	case EquipPart::HANDS: str = "Hands"; break;
	case EquipPart::FEET: str = "Feet"; break;
	case EquipPart::ACCESSORY: str = "Accessory"; break;
	}

	return str;
}
