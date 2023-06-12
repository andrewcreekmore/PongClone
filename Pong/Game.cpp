#include "Game.h"

// handle player (right paddle) movement input
void handleInput(Input* input, float deltaTime)
{
	player.acceleration = 0.f; // reset
	if (isDown(KEY_UP)) { player.acceleration += 1500; }
	if (isDown(KEY_DOWN)) { player.acceleration -= 1500; }
	player.Move(deltaTime);

	// TEMPORARY, PLAYER CONTROLLING BOTH SIDES
	// handle opponent enemy AI (left paddle) movement
	enemy.acceleration = 0.f; // reset
	if (isDown(KEY_W)) { enemy.acceleration += 1500; }
	if (isDown(KEY_S)) { enemy.acceleration -= 1500; }
	enemy.Move(deltaTime);
}

static void simulateGame(Input* input, float deltaTime)
{
	clearScreen(0x000000);
	
	// draw arena boundaries
	drawRect(0, 0, arena.halfSize_x, arena.halfSize_y, arena.color);

	// handle player (right paddle) movement input
	handleInput(input, deltaTime);

	// handle ball movement
	ball.Move(deltaTime);

	// render ball and player + enemy AI paddles
	drawRect(ball.position_x, ball.position_y, ball.halfSize_x, ball.halfSize_y, ball.color);
	drawRect(enemy.position_x, enemy.position_y, enemy.halfSize_x, enemy.halfSize_y, enemy.color);
	drawRect(player.position_x, player.position_y, player.halfSize_x, player.halfSize_y, player.color);
}