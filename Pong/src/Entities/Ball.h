#pragma once

#include "Entity.h"
#include "../Framework/Utilities.h"


class Ball : public Entity
{
public:

	inline Ball() // sets defaults
	{
		halfSize_x = 1.f;
		halfSize_y = 1.f;
		velocity_x = 100.f;
	}

	// moves the ball's position within the arena
	void move(float deltaTime, Entity arena);

	// returns whether/how this entity collides with the boundaries of the arena
	virtual arenaCollision checkForArenaBoundaryCollision(Entity arena) override;

	// resets position and velocity of ball
	void reset();
};

