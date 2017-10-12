#include "mapKeyComparators.h"

bool mapExitKeyCompare(MapExit* first, MapExit* second)
{
	if (first->mapId > second->mapId)
		return false;

	if (first->mapId < second->mapId)
		return true;

	if (first->edge > second->edge)
		return false;

	if (first->edge < second->edge)
		return true;

	return (first->unit < second->unit);
}