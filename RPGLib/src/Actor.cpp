#include "Actor.h"

int ActorDef::save(std::ofstream& saveFile)
{
	if (saveFile.is_open() == false)
		return false;

	char buf[MAX_NAME_LENGTH];
	memset(buf, 0, MAX_NAME_LENGTH);
	memcpy_s(buf, MAX_NAME_LENGTH, name.data(), name.length());

	std::streampos startPos = saveFile.tellp();

	saveFile.write(buf, MAX_NAME_LENGTH);
	saveFile.write((char*)&symbol, sizeof(char));
	saveFile.write((char*)&level, sizeof(short));
	saveFile.write((char*)&exp, sizeof(int));
	saveFile.write((char*)&money, sizeof(int));
	saveFile.write((char*)&maxHp, sizeof(int));
	saveFile.write((char*)&maxMp, sizeof(int));
	saveFile.write((char*)&strength, sizeof(short));
	saveFile.write((char*)&defense, sizeof(short));
	saveFile.write((char*)&intelligence, sizeof(short));
	saveFile.write((char*)&will, sizeof(short));
	saveFile.write((char*)&agility, sizeof(short));
	saveFile.write((char*)&accuracy, sizeof(float));
	saveFile.write((char*)&luck, sizeof(float));
	
	std::streampos endPos = saveFile.tellp();

	return (int)(endPos - startPos);
}


int ActorDef::load(std::ifstream& loadFile)
{
	if (loadFile.is_open() == false)
		return false;

	std::streampos startPos = loadFile.tellg();

	char buf[MAX_NAME_LENGTH + 1];
	loadFile.read(buf, MAX_NAME_LENGTH);
	name.assign(buf);

	loadFile.read((char*)&symbol, sizeof(char));
	loadFile.read((char*)&level, sizeof(short));
	loadFile.read((char*)&exp, sizeof(int));
	loadFile.read((char*)&money, sizeof(int));
	loadFile.read((char*)&maxHp, sizeof(int));
	loadFile.read((char*)&maxMp, sizeof(int));
	loadFile.read((char*)&strength, sizeof(short));
	loadFile.read((char*)&defense, sizeof(short));
	loadFile.read((char*)&intelligence, sizeof(short));
	loadFile.read((char*)&will, sizeof(short));
	loadFile.read((char*)&agility, sizeof(short));
	loadFile.read((char*)&accuracy, sizeof(float));
	loadFile.read((char*)&luck, sizeof(float));

	std::streampos endPos = loadFile.tellg();

	return (int)(endPos - startPos);
}

//return true if first is before second alphabetically or equal
//bool actorKeyCompare(const std::string& first, const std::string& second)
//{
//	return first.compare(second) < 0;
//}