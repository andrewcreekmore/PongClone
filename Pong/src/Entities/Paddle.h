#pragma once

#include "Entity.h"


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
	void move(float deltaTime, Entity arena);

	// resets paddle back to round starting position
	void reset();

	// returns whether/how this entity collides with the boundaries of the arena
	virtual arenaCollision checkForArenaBoundaryCollision(Entity arena) override;
};