#include "Ball.h"


// returns whether/how this entity collides with the boundaries of the arena
arenaCollision Ball::checkForArenaBoundaryCollision(Entity arena)
{
	arenaCollision result = NO_COLLISION;

	if (position_y + halfSize_y > arena.halfSize_y)
	{ result = TOP_COLLISION; }

	else if (position_y - halfSize_y < -arena.halfSize_y)
	{ result = BOTTOM_COLLISION; }

	else if (position_x - halfSize_x < -arena.halfSize_x)
	{ result = LEFT_COLLISION; }

	else if (position_x + halfSize_x > arena.halfSize_x)
	{ result = RIGHT_COLLISION; }

	return result;
}


// moves the ball entity within the arena
void Ball::move(float deltaTime, Entity arena)
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