#pragma once

//struct MapExit
//{
//	int destMapId;
//	MapExit(int id) { destMapId = id; }
//};
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

struct MapExit
{
	int sourceId;
	int edge;
	int destId;
	int destY;
	int destX;
};

struct MapOpening
{
	int mapId;
	int location;
};