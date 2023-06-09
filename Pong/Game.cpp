#include "Game.h"


static void simulateGame(Input* input, float deltaTime)
{
	clearScreen(0x000000);

	// handle player (right paddle) movement input
	if (isDown(KEY_UP) || isDown(KEY_W)) { playerPosition_y += playerSpeed * deltaTime; }
	if (isDown(KEY_DOWN) || isDown(KEY_S)) { playerPosition_y -= playerSpeed * deltaTime; }

	// ball
	drawRect(0, 0, 1, 1, 0x00ff22);
	// left paddle
	drawRect(-60, 30, 2, 15, 0xffff22);
	// right paddle
	drawRect(playerPosition_x, playerPosition_y, 2, 15, 0xffff22);

}