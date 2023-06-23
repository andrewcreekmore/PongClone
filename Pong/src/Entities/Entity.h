#pragma once


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

	// returns whether this entity collides with another; for use between non-static entities
	bool aabbVSaabb(Entity other);

	// returns whether/how this entity collides with the boundaries of the arena
	virtual arenaCollision checkForArenaBoundaryCollision(Entity arena);
};