#include "Paddle.h"
#include "../Framework/Utilities.h"

// returns whether/how this entity collides with the boundaries of the arena
arenaCollision Paddle::checkForArenaBoundaryCollision(Entity arena)
{
	arenaCollision result = NO_COLLISION;

	if (position_y + halfSize_y > arena.halfSize_y)
	{ result = TOP_COLLISION; }

	else if (position_y - halfSize_y < -arena.halfSize_y)
	{ result = BOTTOM_COLLISION; }

	return result;
}


// moves the paddle entity within the arena
void Paddle::move(float deltaTime, Entity arena)
{
	// calculate paddle acceleration, position and velocity
	acceleration -= velocity_y * 10.f; // add friction
	position_y = position_y + (velocity_y * deltaTime) + ((acceleration * deltaTime * deltaTime) * 0.5f);
	velocity_y = velocity_y + acceleration * deltaTime;
}


// resets paddle back to round starting position
void Paddle::reset()
{
	position_x = initialPosition_x;
	position_y = 0.f;
}