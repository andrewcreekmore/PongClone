#include "Entity.h"

/*
===========================================================================
Entity: abstract game-object base class; a thing existing in the game-world
- has position, velocity, acceleration, size, and color member variables
- provides collision-checking functions (for both dynamic/static others)
===========================================================================
*/

// returns whether this entity collides with another; for use between non-static entities
bool Entity::aabbVSaabb(Entity other)
{
	return position_x + halfSize_x > other.position_x - other.halfSize_x && position_x - halfSize_x < other.position_x + other.halfSize_x &&
		position_y + halfSize_y > other.position_y - other.halfSize_y && position_y + halfSize_y < other.position_y + other.halfSize_y;
}