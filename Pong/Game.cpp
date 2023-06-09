#include "Game.h"


static void simulateGame(Input* input, float deltaTime)
{
	clearScreen(0xff5500);

	if (isDown(KEY_UP)) { playerPosition_y += playerSpeed * deltaTime; }
	if (isDown(KEY_DOWN)) { playerPosition_y -= playerSpeed * deltaTime; }
	if (isDown(KEY_LEFT)) { playerPosition_x -= playerSpeed * deltaTime; }
	if (isDown(KEY_RIGHT)) { playerPosition_x += playerSpeed * deltaTime; }

	if (isDown(KEY_W)) { playerPosition_y += playerSpeed * deltaTime; }
	if (isDown(KEY_S)) { playerPosition_y -= playerSpeed * deltaTime; }
	if (isDown(KEY_A)) { playerPosition_x -= playerSpeed * deltaTime; }
	if (isDown(KEY_D)) { playerPosition_x += playerSpeed * deltaTime; }

	drawRect(playerPosition_x, playerPosition_y, 1, 1, 0x00ff22);
	drawRect(30, 30, 5, 5, 0xffff22);
	drawRect(-20, 20, 8, 3, 0xffff22);
}