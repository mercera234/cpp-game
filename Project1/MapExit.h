#pragma once

//
//struct InMapExit : public MapExit
//{
//	int destY;
//	int destX;
//
//	InMapExit(int y, int x, int id) : MapExit(id)
//	{ destY = y; destX = x; }
//};

//struct BorderExit : public MapExit
//{
//	int edge;
//	int start;
//	int length;
//	BorderExit(int edge, int start, int length, int id) : MapExit(id)
//	{
//		this->edge = edge; 
//		this->start = start;
//		this->length = length;
//	}
//};

//struct MapExit
//{
//	int sourceId;
//	int edge;
//	int destId;
//	int destY;
//	int destX;
//};

struct MapExit
{
	int mapId;
	int edge;
	int unit; //identifies a piece of the edge (characters axis coordinate divided by map unit size)
};

struct MapOpening
{
	int mapId;
	int location;
};
//
//struct MapSeam
//{
//	MapExit* negExit;
//	MapExit* posExit;
//	MapSeam(Map* negMap, Map* posMap, //negative map is either N or W, positive is S or E
//		bool xAxis, //true if seam is between West and East maps
//		unsigned short negUnit, unsigned short posUnit, //the unit portion of the map to join together
//		unsigned short length);
//};