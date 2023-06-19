#include "Game.h"
#include <windows.h>
#include "Utilities.h"
#include <chrono>
#include <thread>


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
	// move much slower when ball is moving away vs coming at paddle
	float speedMultiplier = ball.velocity_x > 0.f ? 30.f : 85.f;

	// determine acceleration needed (direction, magnitude)
	float calculatedAcceleration = (ball.position_y - position_y) * speedMultiplier;

	// if don't have to move to keep ball in play 
	if (calculatedAcceleration <= 100.f && calculatedAcceleration >= -100.f)
	{ 
		if (ball.position_x < -25.f) // if close enough
		{
			// introduce additional movement to try and break stalemate bounce
			if (bLastMoveWasUp)
			{
				acceleration += 800.f;
				bLastMoveWasUp = true;
			}
			else
			{
				acceleration -= 800.f;
				bLastMoveWasUp = false;
			}
		}
	}

	else
	{
		// track last move direction to reduce jitter/rapid direction-swapping
		bLastMoveWasUp = calculatedAcceleration > 0.f ? true : false;

		// process the acceleration for next move() call
		acceleration = clampFloat(-1200.f, calculatedAcceleration, 1200.f);
	}
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


// resets paddle back to round starting position
void Paddle::reset()
{
	position_x = initialPosition_x;
	position_y = 0.f;
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
		player.score++; // PLAYER SCORES POINT
		break;

	case RIGHT_COLLISION:
		enemy.score++; // ENEMY AI (or "player2" in multi-player mode) SCORES POINT
		break;
	}

	// if either paddle scored a point
	if (result == LEFT_COLLISION || result == RIGHT_COLLISION)
	{
		// reset positions, brief pause, resume
		std::thread setupNextRoundThread(setupNextRound);
		setupNextRoundThread.detach();
	}

	// movement
	position_x += velocity_x * deltaTime;
	position_y += velocity_y * deltaTime;
}


// resets position and velocity of ball
void Ball::reset()
{
	position_x = 0.f;
	position_y = 0.f;
	velocity_y = 0.f;
	velocity_x *= -1.f; // invert horizontal velocity
	velocity_x = clampFloat(-100.f, velocity_x, 100.f); // reset to starting speed
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

	// handle opponent / player2 (left paddle) movement input
	enemy.acceleration = 0.f; // reset
	if (!bActiveEnemyAI)
	{
		if (isDown(KEY_W)) { enemy.acceleration += 1500; }
		if (isDown(KEY_S)) { enemy.acceleration -= 1500; }
	}
}


// for use from main or pause menus; starts fresh game
void startNewGame()
{
	currentGameMode = GM_GAMEPLAY;
	bActiveEnemyAI = activeMainMenuButton ? false : true;

	ball.reset();
	player.reset();
	enemy.reset();
	player.score = 0;
	enemy.score = 0;
	
	bGamePaused = false;
}


// resets paddle/ball and resumes after brief delay; for use after points scored
void setupNextRound()
{
	bRoundActive = false;
	ball.reset();
	player.reset();
	enemy.reset();

	delay(1.5);
	if (!bGamePaused) // don't begin if game was paused during delay
	{ bRoundActive = true; }
}


// flags round as inactive while paused
void pauseRound()
{
	bRoundActive = false;

	while (bGamePaused)
	{ delay(1); }

	// resume
	bHideBall = false;
	bHideScore = false;
	currentGameMode = GM_GAMEPLAY;
	bRoundActive = true;
}


// handles ball/paddle movement + AI
static void simulateRound(float deltaTime)
{
	if (bRoundActive)
	{ 
		ball.move(deltaTime); 
		player.move(deltaTime);

		if (bActiveEnemyAI)
		{ enemy.decideNextMove(); }

		enemy.move(deltaTime);
	}
}


// draws elements of current round to screen
static void renderRound()
{
	// render player / enemy AI scores
	if (!bHideScore)
	{
		drawRectNumber(enemy.score, -15.f, 40.f, 1.f, 0xbbffbb);
		drawRectNumber(player.score, 15.f, 40.f, 1.f, 0xbbffbb);
	}

	// render ball and player + enemy AI paddles
	if (!bHideBall)
	{ drawRect(ball.position_x, ball.position_y, ball.halfSize_x, ball.halfSize_y, ball.color); }

	drawRect(enemy.position_x, enemy.position_y, enemy.halfSize_x, enemy.halfSize_y, enemy.color);
	drawRect(player.position_x, player.position_y, player.halfSize_x, player.halfSize_y, player.color);
}


// renders pause menu overlay
void drawPauseMenu(int highlightedButton)
{
	float position_x = -20.f;
	float pauseMenu_ResumePosition_y = 18.f;
	float pauseMenu_NewGamePosition_y = 10.f;
	float pauseMenu_MainMenuPosition_y = 2.f;
	float pauseMenu_ExitPosition_y = -6.f;
	unsigned int highlightedColor = 0xff0000;
	unsigned int defaultColor = 0xffffff;
	float highlightedSize = .85f;
	float defaultSize = .75f;

	// draw round entities in background
	renderRound();

	// draw overlay + menu options
	drawRect(0.f, 3.f, 28.f, 22.f, 0xffffff);
	drawRect(0.f, 3.f, 27.f, 21.f, arena.color);
	drawRectText("RESUME", position_x, pauseMenu_ResumePosition_y, (highlightedButton == 0 ? highlightedSize : defaultSize), (highlightedButton == 0 ? highlightedColor : defaultColor));
	drawRectText("NEW GAME", position_x, pauseMenu_NewGamePosition_y, (highlightedButton == 1 ? highlightedSize : defaultSize), (highlightedButton == 1 ? highlightedColor : defaultColor));
	drawRectText("MAIN MENU", position_x, pauseMenu_MainMenuPosition_y, (highlightedButton == 2 ? highlightedSize : defaultSize), (highlightedButton == 2 ? highlightedColor : defaultColor));
	drawRectText("EXIT", position_x, pauseMenu_ExitPosition_y, (highlightedButton == 3 ? highlightedSize : defaultSize), (highlightedButton == 3 ? highlightedColor : defaultColor));
}


// handles overall game process
static void simulateGame(Input* input, float deltaTime)
{
	clearScreen(0x000000);
	
	// render arena
	drawRect(0, 0, arena.halfSize_x, arena.halfSize_y, arena.color);
	drawArenaBoundaries(arena.halfSize_x, arena.halfSize_y, 0xffffff);

	if (currentGameMode == GM_GAMEPLAY)
	{
		if (pressed(KEY_ESC)) // show exit prompt
		{
			bRoundActive = false;
			bGamePaused = true;
			// pause round
			std::thread pauseThread(pauseRound);
			pauseThread.detach();

			activePauseMenuButton = 0; // always start at top option (Resume)
			currentGameMode = GM_PAUSEMENU;
		}

		// handle player movement input
		handleInput(input, deltaTime);

		// simulate round entities
		simulateRound(deltaTime);

		// draw round entities to screen
		renderRound();
	}

	else if (currentGameMode == GM_MAINMENU)
	{
		if (pressed(KEY_A) || pressed (KEY_D)) // choose game mode
		{ activeMainMenuButton = !activeMainMenuButton; }

		if (pressed(KEY_ENTER) || pressed(KEY_SPACE))
		{ startNewGame(); }

		drawRectText("PONG", -30, 35, 3, 0xcccccc);
		drawRectText("BY ANDREW CREEKMORE", -53, 12, 1, 0xcccccc);

		if (activeMainMenuButton == 0) // single-player (play against enemy AI)
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

	else if (currentGameMode == GM_PAUSEMENU)
	{
		if (pressed(KEY_ESC)) // resume
		{ bGamePaused = false; }

		if ((pressed(KEY_W) || pressed(KEY_UP)) && activePauseMenuButton > 0)
		{ activePauseMenuButton--; }

		if ((pressed(KEY_S) || pressed(KEY_DOWN)) && activePauseMenuButton < 3)
		{ activePauseMenuButton++; }

		if (pressed(KEY_ENTER) || pressed(KEY_SPACE))
		{
			switch (activePauseMenuButton)
			{
			case 0: // RESUME
				bGamePaused = false;
				break;

			case 1: // NEW GAME
				startNewGame();
				break;

			case 2: // MAIN MENU
				currentGameMode = GM_MAINMENU;
				break;

			case 3: // EXIT
				PostMessage(parentWindow, WM_CLOSE, 0, 0); // send "Really quit?" message box
				break;
			}
		}

		drawPauseMenu(activePauseMenuButton);
	}
}