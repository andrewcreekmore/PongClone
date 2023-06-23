#include "Entity.h"


// returns whether this entity collides with another; for use between non-static entities
bool Entity::aabbVSaabb(Entity other)
{
	return position_x + halfSize_x > other.position_x - other.halfSize_x && position_x - halfSize_x < other.position_x + other.halfSize_x &&
		position_y + halfSize_y > other.position_y - other.halfSize_y && position_y + halfSize_y < other.position_y + other.halfSize_y;
}


arenaCollision Entity::checkForArenaBoundaryCollision(Entity arena)
{
	return NO_COLLISION;
}
