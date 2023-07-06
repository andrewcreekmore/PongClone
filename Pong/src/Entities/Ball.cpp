#include "Ball.h"

/*
===========================================================================
Ball: entity representing a ping-pong ball
- has additional bHidden flag
- provides move, reset, and arena collision-check functions
===========================================================================
*/

// returns whether/how this entity collides with the boundaries of the arena
arenaCollision Ball::checkForArenaBoundaryCollision(float arenaHalfSize_x, float arenaHalfSize_y)
{
	arenaCollision result = NO_COLLISION;

	if (position_y + halfSize_y > arenaHalfSize_y)
	{ result = TOP_COLLISION; }

	else if (position_y - halfSize_y < -arenaHalfSize_y)
	{ result = BOTTOM_COLLISION; }

	else if (position_x - halfSize_x < -arenaHalfSize_x)
	{ result = LEFT_COLLISION; }

	else if (position_x + halfSize_x > arenaHalfSize_x)
	{ result = RIGHT_COLLISION; }

	return result;
}

// moves the ball entity within the arena
void Ball::move(float deltaTime)
{
	// movement
	position_x += velocity_x * deltaTime;
	position_y += velocity_y * deltaTime;
}

// resets position and velocity of ball
void Ball::reset()
{
	position_x = 0.f;
	position_y = 0.f;
	velocity_y = 0.f;
	velocity_x *= -1.f; // invert horizontal velocity
	velocity_x = clampFloat(-100.f, velocity_x, 100.f); // reset to starting speed
}