#pragma once
#include <functional>
#include "Repository.h"
#include "MapExit.h"
#include "MapRoom.h"

class MapRepository : public Repository<MapRoom>
{
private:
	std::map<MapExit*, MapExit*, std::function<bool(MapExit*, MapExit*)>> exits; //a collection of all the exits between maps
	std::map<unsigned short, MapRoom*> maps; //all maps that can be explored. It is assumed that all maps are joined in some way

	unsigned short unitMapHeight;
	unsigned short unitMapWidth;
public:
	MapRepository();
	MapRepository(unsigned short unitMapHeight, unsigned short unitMapWidth);
	void addExit(MapExit* source, MapExit* dest); //add one pair of connecting exits (implies no 1-way paths)
	MapExit* getExit(MapExit* sourceExit);

	void add(MapRoom& obj); //override
	MapRoom* find(unsigned short id); //override

	void addMapSeam(MapRoom& negMap, MapRoom& posMap, //negative map is either N or W, positive is S or E
		Axis axis, //true if seam is between West and East maps
		unsigned short negUnit, unsigned short posUnit, //the unit portion of the map to join together
		unsigned short length);
	
	std::map<unsigned short, MapRoom*>* getMaps() { return &maps; }

	unsigned short getUnitMapHeight() { return unitMapHeight; }
	unsigned short getUnitMapWidth() { return unitMapWidth; }
};