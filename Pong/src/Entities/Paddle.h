#pragma once

#include "Entity.h"
#include "Framework/Utilities.h"

/*
===========================================================================
Paddle: entity representing a ping-pong paddle, player or AI
- has additional score variable and AI-related flags
- provides move, reset, and arena collision-check functions
===========================================================================
*/

class Paddle : public Entity
{
public:

	int score;
	bool bLastMoveWasUp;
	bool bBreakingStalemate;
	float initialPosition_x;

	inline Paddle(float initial_x) // sets defaults
		: score(0), bLastMoveWasUp(false), bBreakingStalemate(false)
	{
		initialPosition_x = initial_x;
		position_x = initialPosition_x;
		halfSize_x = 2.5f;
		halfSize_y = 12.f;
	}

	// moves the paddle's position within the arena
	void move(float deltaTime);

	// resets paddle back to round starting position
	void reset();

	// returns whether/how this entity collides with the boundaries of the arena
	virtual arenaCollision checkForArenaBoundaryCollision(float arenaHalfSize_y) override;
};