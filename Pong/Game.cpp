#include "Game.h"

// construct game entities
Arena arena;
Paddle player(80.f); // positioned to right
Paddle enemy(-80.f); // positioned to left
Ball ball;


// returns whether this entity collides with another; for use between non-static entities
bool Entity::aabbVSaabb(Entity other)
{
	return position_x + halfSize_x > other.position_x - other.halfSize_x && position_x - halfSize_x < other.position_x + other.halfSize_x &&
		   position_y + halfSize_y > other.position_y - other.halfSize_y && position_y + halfSize_y < other.position_y + other.halfSize_y;
}


// returns whether/how this entity collides with the boundaries of the arena
arenaCollision Paddle::checkForArenaBoundaryCollision()
{
	arenaCollision result = NO_COLLISION;

	if (position_y + halfSize_y > arena.halfSize_y)
	{ result = TOP_COLLISION; }
	else if (position_y - halfSize_y < -arena.halfSize_y)
	{ result = BOTTOM_COLLISION; }

	return result;
}


// returns whether/how this entity collides with the boundaries of the arena
arenaCollision Ball::checkForArenaBoundaryCollision()
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


// moves the paddle entity within the arena
void Paddle::move(float deltaTime)
{
	// collision check: arena boundaries
	arenaCollision result = checkForArenaBoundaryCollision();

	switch (result)
	{
	case NO_COLLISION:
		break;

	case TOP_COLLISION:
		position_y = arena.halfSize_y - halfSize_y; // reset position
		velocity_y *= -0.5f; // bounce away from hit boundary
		break;

	case BOTTOM_COLLISION:
		position_y = -arena.halfSize_y + halfSize_y; // reset position
		velocity_y *= -0.5f; // bounce away from hit boundary
		break;
	}

	// calculate paddle acceleration, position and velocity
	acceleration -= velocity_y * 10.f; // add friction
	position_y = position_y + (velocity_y * deltaTime) + ((acceleration * deltaTime * deltaTime) * 0.5f);
	velocity_y = velocity_y + acceleration * deltaTime;
}


// moves the ball entity within the arena
void Ball::move(float deltaTime)
{
	// collision check: with player
	if (aabbVSaabb(player))
	{
		position_x = player.position_x - player.halfSize_x - halfSize_x; // reset position
		velocity_x *= -1.f; // bounce away horizontally
		velocity_y = (position_y - player.position_y) * 2.f + (player.velocity_y * 0.75f); // bounce away vertically
	}

	// collision check: with enemy AI
	else if (aabbVSaabb(enemy))
	{
		position_x = enemy.position_x + enemy.halfSize_x + halfSize_x; // reset position
		velocity_x *= -1.f; // bounce away horizontally
		velocity_y = (position_y - enemy.position_y) * 2.f + (enemy.velocity_y * 0.75f); // bounce away vertically
	}

	// collision check: arena boundaries
	arenaCollision result = checkForArenaBoundaryCollision();

	switch (result)
	{
	case NO_COLLISION:
		break;

	case TOP_COLLISION:
		position_y = arena.halfSize_y - halfSize_y; // reset position to limit
		velocity_y *= -1.f; // invert velocity to bounce away
		break;

	case BOTTOM_COLLISION:
		position_y = -arena.halfSize_y + halfSize_y; // reset position to limit
		velocity_y *= -1.f; // invert velocity to bounce away
		break;

	case LEFT_COLLISION:
		velocity_x *= -1.f; // invert horizontal velocity
		velocity_y = 0.f; // reset vertical velocity
		position_x = 0.f;
		position_y = 0.f; // reset position to center of arena

		// PLAYER SCORES POINT
		player.score++;
		break;

	case RIGHT_COLLISION:
		velocity_x *= -1.f; // invert horizontal velocity
		velocity_y = 0.f; // reset vertical velocity
		position_x = 0.f;
		position_y = 0.f; // reset position to center of arena

		// ENEMY AI SCORES POINT
		enemy.score++;
		break;
	}

	// movement
	position_x += velocity_x * deltaTime;
	position_y += velocity_y * deltaTime;
}


// handle player (right paddle) movement input
void handleInput(Input* input, float deltaTime)
{
	player.acceleration = 0.f; // reset
	if (isDown(KEY_UP)) { player.acceleration += 1500; }
	if (isDown(KEY_DOWN)) { player.acceleration -= 1500; }
	player.move(deltaTime);

	// TEMPORARY, PLAYER CONTROLLING BOTH SIDES
	// handle opponent enemy AI (left paddle) movement
	enemy.acceleration = 0.f; // reset
	if (isDown(KEY_W)) { enemy.acceleration += 1500; }
	if (isDown(KEY_S)) { enemy.acceleration -= 1500; }
	enemy.move(deltaTime);
}


static void simulateGame(Input* input, float deltaTime)
{
	clearScreen(0x000000);
	
	// render arena boundaries
	drawRect(0, 0, arena.halfSize_x, arena.halfSize_y, arena.color);

	// handle player (right paddle) movement input
	handleInput(input, deltaTime);

	// handle ball movement
	ball.move(deltaTime);

	// render player / enemy AI scores
	drawNumber(player.score, -15.f, 40.f, 1.f, 0xbbffbb);
	drawNumber(enemy.score, 15.f, 40.f, 1.f, 0xbbffbb);

	// render ball and player + enemy AI paddles
	drawRect(ball.position_x, ball.position_y, ball.halfSize_x, ball.halfSize_y, ball.color);
	drawRect(enemy.position_x, enemy.position_y, enemy.halfSize_x, enemy.halfSize_y, enemy.color);
	drawRect(player.position_x, player.position_y, player.halfSize_x, player.halfSize_y, player.color);
}