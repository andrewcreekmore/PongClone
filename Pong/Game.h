#pragma once

#define isDown(key) input->keyState[key].bIsDown
#define pressed(key) (input->keyState[key].bIsDown && input->keyState[key].bHasChanged)
#define released(key) (!input->keyState[key].bIsDown && input->keyState[key].bHasChanged)

class Entity
{
public:

	float position_x;
	float position_y;
	float velocity_x;
	float velocity_y;
	float acceleration;
	unsigned int color;
	float halfSize_x;
	float halfSize_y;

	Entity()
	{
		position_x = 0.0f;
		position_y = 0.0f;
		velocity_x = 0.0f;
		velocity_y = 0.0f;
		acceleration = 0.0f;
		color = 0xffffff;
		halfSize_x = 0.f;
		halfSize_y = 0.f;
	}
};


class Arena : public Entity
{
public:
	Arena()
	{
		color = 0xffaa33;
		halfSize_x = 85.f;
		halfSize_y = 45.f;
	}
};

// arena
Arena arena;

class Paddle : public Entity
{
public:
	
	int score;

	Paddle()
	{
		color = 0xffffff;
		halfSize_x = 2.5f;
		halfSize_y = 12.f;
		score = 0;
	}


	void Move(float deltaTime)
	{
		// collision check: arena boundaries
		if (position_y + halfSize_y > arena.halfSize_y) // top of arena
		{
			position_y = arena.halfSize_y - halfSize_y; // reset position
			velocity_y *= -0.5f; // bounce away from hit boundary
		}
		else if (position_y - halfSize_y < -arena.halfSize_y) // bottom of arena
		{
			position_y = -arena.halfSize_y + halfSize_y; // reset position
			velocity_y *= -0.5f; // bounce away from hit boundary
		}

		// calculate paddle acceleration, position and velocity
		acceleration -= velocity_y * 10.f; // add friction
		position_y = position_y + (velocity_y * deltaTime) + ((acceleration * deltaTime * deltaTime) * 0.5f);
		velocity_y = velocity_y + acceleration * deltaTime;
	}
};


class Player : public Paddle
{
public:

	Player()
	{
		position_x = 80.f;
	}
};


class Enemy : public Paddle
{
public:

	Enemy()
	{
		position_x = -80.f;
	}
};


// player + AI opponent
Player player;
Enemy enemy;

class Ball : public Entity
{
public:

	Ball()
	{
		color = 0xffffff;
		halfSize_x = 1.f;
		halfSize_y = 1.f;
		velocity_x = 100.f;
	}

	// helper function for collision checks
	bool aabbVSaabb(float position1_x, float position1_y, float halfSize1_x, float halfSize1_y, float position2_x, float position2_y, float halfSize2_x, float halfSize2_y)
	{
		return position1_x + halfSize1_x > position2_x - halfSize2_x && position1_x - halfSize1_x < position2_x + halfSize2_x &&
			   position1_y + halfSize1_y > position2_y - halfSize2_y && position1_y + halfSize1_y < position2_y + halfSize2_y;
	}

	void Move(float deltaTime)
	{
		// collision check: with player
		if (aabbVSaabb(position_x, position_y, halfSize_x, halfSize_y, player.position_x, player.position_y, player.halfSize_x, player.halfSize_y))
		{
			position_x = player.position_x - player.halfSize_x - halfSize_x; // reset position
			velocity_x *= -1.f; // bounce away horizontally
			velocity_y = (position_y - player.position_y) * 2.f + (player.velocity_y * 0.75f); // bounce away vertically
		}

		// collision check: with enemy AI
		else if (aabbVSaabb(position_x, position_y, halfSize_x, halfSize_y, enemy.position_x, enemy.position_y, enemy.halfSize_x, enemy.halfSize_y))
		{
			position_x = enemy.position_x + enemy.halfSize_x + halfSize_x; // reset position
			velocity_x *= -1.f; // bounce away horizontally
			velocity_y = (position_y - enemy.position_y) * 2.f + (enemy.velocity_y * 0.75f); // bounce away vertically
		}

		// collision check: with arena top/bottom boundaries
		if (position_y + halfSize_y > arenaHalfSize_y) // arena top
		{
			position_y = arenaHalfSize_y - halfSize_y; // reset position to limit
			velocity_y *= -1.f; // invert velocity to bounce away
		}
		else if (position_y - halfSize_y < -arenaHalfSize_y) // arena bottom
		{
			position_y = -arenaHalfSize_y + halfSize_y; // reset position to limit
			velocity_y *= -1.f; // invert velocity to bounce away
		}

		// collision check: arena right/left
		if (position_x + halfSize_x > arena.halfSize_x) // right (behind player; enemy AI scores point)
		{
			velocity_x *= -1.f;
			velocity_y = 0.f;
			position_x = 0.f;
			position_y = 0.f;
			enemy.score++;
		}
		else if (position_x - halfSize_x < -arena.halfSize_x) // left (behind enemy AI; player scores point)
		{
			velocity_x *= -1.f;
			velocity_y = 0.f;
			position_x = 0.f;
			position_y = 0.f;
			player.score++;
		}

		position_x += velocity_x * deltaTime;
		position_y += velocity_y * deltaTime;
	}
};

// ball
Ball ball;
