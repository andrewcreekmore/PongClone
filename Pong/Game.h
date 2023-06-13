#pragma once

#define isDown(key) input->keyState[key].bIsDown
#define pressed(key) (input->keyState[key].bIsDown && input->keyState[key].bHasChanged)
#define released(key) (!input->keyState[key].bIsDown && input->keyState[key].bHasChanged)


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

	// returns whether this entity penetrates another
	bool aabbVSaabb(Entity other);
};


class Arena : public Entity
{
public:

	inline Arena() // sets defaults
	{
		color = 0xffaa33;
		halfSize_x = 85.f;
		halfSize_y = 45.f;
	}
};


class Paddle : public Entity
{
public:
	
	int score;
	 
	inline Paddle(float initialPosition_x) // sets defaults
		: score(0)
	{
		position_x = initialPosition_x;
		halfSize_x = 2.5f;
		halfSize_y = 12.f;
	}

	// moves the paddle's position within the arena
	void move(float deltaTime);
};


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
	void move(float deltaTime);
};

