#include "Game.h"
#include <windows.h>
#include "Utilities.h"


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


// for enemy AI paddle: determine next movement
void Paddle::decideNextMove()
{
	float calculatedAcceleration = (ball.position_y - position_y) * 100;

	// when calculatedAcceleration is low (i.e., what's necessary to keep ball in play)... 
	if (calculatedAcceleration > -100.f && calculatedAcceleration < 100.f) 
	{ calculatedAcceleration += 1000; } // ...introduce additional movement to try and 'hit ball past player'

	acceleration = clampFloat(-1200, calculatedAcceleration, 1200);
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
	if (!bRoundActive) { return; }

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
		if (bRoundActive)
		{
			player.score++;
			bRoundActive = false;
		}

		break;

	case RIGHT_COLLISION:
		velocity_x *= -1.f; // invert horizontal velocity
		velocity_y = 0.f; // reset vertical velocity
		position_x = 0.f;
		position_y = 0.f; // reset position to center of arena

		// ENEMY AI SCORES POINT
		if (bRoundActive)
		{
			enemy.score++;
			bRoundActive = false;
		}

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
	if (bActiveEnemyAI) // allow use of W/S keys when only controlling player
	{
		if (isDown(KEY_W)) { player.acceleration += 1500; }
		if (isDown(KEY_S)) { player.acceleration -= 1500; }
	}
	player.move(deltaTime);

	// handle opponent enemy AI (left paddle) movement
	enemy.acceleration = 0.f; // reset
	if (!bActiveEnemyAI)
	{
		if (isDown(KEY_W)) { enemy.acceleration += 1500; }
		if (isDown(KEY_S)) { enemy.acceleration -= 1500; }
	}

	else // use enemy AI to control movement
	{ enemy.decideNextMove(); }

	enemy.move(deltaTime);
}


static void simulateGame(Input* input, float deltaTime)
{
	clearScreen(0x000000);
	
	// render arena
	drawRect(0, 0, arena.halfSize_x, arena.halfSize_y, arena.color);
	drawArenaBoundaries(arena.halfSize_x, arena.halfSize_y, 0xffffff);

	if (pressed(KEY_ESC)) // show exit prompt
	{ PostMessage(parentWindow, WM_CLOSE, 0, 0); }

	if (currentGameMode == GAMEPLAY)
	{
		// handle player (right paddle) movement input
		handleInput(input, deltaTime);

		// handle ball movement
		if (bRoundActive)
		{ ball.move(deltaTime); }
		
		else // reset after scored points
		{
			enemy.position_y = 0.f;
			player.position_y = 0.f;
			bRoundActive = true;
		}

		// render player / enemy AI scores
		drawRectNumber(enemy.score, -15.f, 40.f, 1.f, 0xbbffbb);
		drawRectNumber(player.score, 15.f, 40.f, 1.f, 0xbbffbb);

		// render ball and player + enemy AI paddles
		drawRect(ball.position_x, ball.position_y, ball.halfSize_x, ball.halfSize_y, ball.color);
		drawRect(enemy.position_x, enemy.position_y, enemy.halfSize_x, enemy.halfSize_y, enemy.color);
		drawRect(player.position_x, player.position_y, player.halfSize_x, player.halfSize_y, player.color);
	}

	else // menu
	{
		if (pressed(KEY_A) || pressed (KEY_D)) // choose game mode
		{ activeMenuButton = !activeMenuButton; }

		if (pressed(KEY_ENTER) || pressed(KEY_SPACE)) // select chosen button
		{
			currentGameMode = GAMEPLAY;
			bActiveEnemyAI = activeMenuButton ? false : true;
		}

		drawRectText("PONG", -30, 35, 3, 0xcccccc);
		drawRectText("BY ANDREW CREEKMORE", -53, 12, 1, 0xcccccc);

		if (activeMenuButton == 0) // single-player (play against enemy AI)
		{
			drawRectText("SINGLE PLAYER", -80, -15, 1, 0xff0000);
			drawRect(-65, -25, 15, 1, 0xff0000);
			drawRectText("MULTIPLAYER", 15, -15, 1, 0xcccccc);
		}
		else // multi-player (control both paddles)
		{
			drawRectText("SINGLE PLAYER", -80, -15, 1, 0xcccccc);
			drawRectText("MULTIPLAYER", 15, -15, 1, 0xff0000);
			drawRect(30, -25, 15, 1, 0xff0000);
		}
	}
}