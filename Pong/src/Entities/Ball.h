#pragma once

#include "Entity.h"
#include "Framework/Utilities.h"

/*
===========================================================================
Ball: entity representing a ping-pong ball
- has additional bHidden flag
- provides move, reset, and arena collision-check functions
===========================================================================
*/

class Ball : public Entity
{
public:

	bool bHidden;

	inline Ball() // sets defaults
	{
		halfSize_x = 1.f;
		halfSize_y = 1.f;
		velocity_x = 100.f;
		bHidden = false;
	}

	// moves the ball's position within the arena
	void move(float deltaTime);

	// returns whether/how this entity collides with the boundaries of the arena
	virtual arenaCollision checkForArenaBoundaryCollision(float arenaHalfSize_x, float arenaHalfSize_y) override;

	// resets position and velocity of ball
	void reset();
};

