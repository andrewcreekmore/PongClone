#pragma once

/*
===========================================================================
Entity: abstract game-object base class; a thing existing in the game-world
- has position, velocity, acceleration, size, and color member variables
- provides collision-checking functions (for both dynamic/static others)
===========================================================================
*/

enum arenaCollision
{
	NO_COLLISION,
	TOP_COLLISION,
	BOTTOM_COLLISION,
	LEFT_COLLISION,
	RIGHT_COLLISION
};

class Entity
{
public:

	float position_x, position_y;
	float velocity_x, velocity_y;
	float acceleration;
	float halfSize_x, halfSize_y;
	unsigned int color;

	inline Entity()
		: position_x(0.f), position_y(0.f), velocity_x(0.f), velocity_y(0.f),
		acceleration(0.f), color(0xffffff), halfSize_x(0.f), halfSize_y(0.f)
	{}

	// returns whether this entity collides with another class instance
	bool aabbVSaabb(Entity other);

	// returns whether/how this entity collides with the boundaries of the arena
	inline virtual arenaCollision checkForArenaBoundaryCollision(float arenaHalfSize_y) { return NO_COLLISION; }
	inline virtual arenaCollision checkForArenaBoundaryCollision(float arenaHalfSize_x, float arenaHalfSize_y) { return NO_COLLISION; }

	// returns an adjusted positional value to account for gain loss due to spatialization
	inline float getAudioPosition_x() { return position_x / 8.f; }
	inline float getAudioPosition_y() { return position_y / 8.f; }
};