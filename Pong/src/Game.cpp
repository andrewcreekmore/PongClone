#include "Game.h"


// construct game entities
Arena arena;
Paddle player(80.f); // positioned to right
Paddle enemy(-80.f); // positioned to left
Ball ball;

// retrieve default sound device
SoundDevice* currentSoundDevice = LISTENER->get();

// create sound players + load SFX
SoundPlayer paddleSoundPlayer;
SoundPlayer arenaBoundarySoundPlayer(2.5f);
SoundPlayer scoredPointSoundPlayer(0.5f);
int paddleImpactSound = SFX_LOAD(".\\res\\Sounds\\Pop.ogg");
int arenaBoundaryImpactSound = SFX_LOAD(".\\res\\Sounds\\pingpongboard.ogg");
int scoredPointSound = SFX_LOAD(".\\res\\Sounds\\applause-mono-24bit-48khz.wav");


// handles overall game process
void simulateGame(Input* input, float deltaTime, HWND parentWindow)
{
	clearScreen(0x000000);

	// render arena
	drawRect(0, 0, arena.halfSize_x, arena.halfSize_y, arena.color);
	drawArenaBoundaries(arena.halfSize_x, arena.halfSize_y, 0xffffff);

	if (currentGameMode == GM_GAMEPLAY)
	{
		if (pressed(KEY_ESC)) // show exit prompt
		{
			pauseGame();
			activePauseMenuButton = 0; // always start at top option (Resume)
			currentGameMode = GM_PAUSEMENU;
		}

		// handle player movement input
		handleMovementInput(input, deltaTime);

		// simulate round entities
		simulateRound(deltaTime);

		// draw round entities to screen
		renderRound();
	}

	else if (currentGameMode == GM_MAINMENU)
	{
		handleMainMenuInput(input);
		drawMainMenu(activeMainMenuButton);
	}

	else if (currentGameMode == GM_PAUSEMENU)
	{
		handlePauseMenuInput(input);
		drawPauseMenu(activePauseMenuButton);
	}
}


// handles ball/paddle movement + AI
void simulateRound(float deltaTime)
{
	if (bRoundActive)
	{
		handleBallCollision();
		ball.move(deltaTime, arena);
	}

	handlePaddleCollision(player);
	handlePaddleCollision(enemy);

	player.move(deltaTime, arena);
	if (bActiveEnemyAI) { decideNextMoveAI(); }
	enemy.move(deltaTime, arena);
}


// handle player (right paddle) movement input
void handleMovementInput(Input* input, float deltaTime)
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
void startNewGame(bool activeAI)
{
	stopRoundSFX();
	currentGameMode = GM_GAMEPLAY;
	bActiveEnemyAI = activeAI;

	ball.reset();
	player.reset();
	enemy.reset();
	player.score = 0;
	enemy.score = 0;
	
	bGamePaused = false;
	bRoundActive = true;
}


// flags round as inactive then resumes after brief delay; for use after points scored
void setupNextRound()
{
	bRoundActive = false;
	delay(2);
	ball.bHidden = false;
	delay(1);

	if (!bGamePaused) // don't begin if game was paused during delay
	{ bRoundActive = true; }
}


// hides/resets ball + calls setupNextRound() in own thread
void resetRound()
{
	ball.bHidden = true;
	ball.reset();

	std::thread setupNextRoundThread(setupNextRound);
	setupNextRoundThread.detach();
}


// flags round as inactive while paused
void pauseCurrentRound()
{
	bRoundActive = false;

	while (bGamePaused)
	{ delay(1); }

	// resume
	currentGameMode = GM_GAMEPLAY;
	bRoundActive = true;
}


// pauses the current round in own thread
void pauseGame()
{
	pauseRoundSFX();
	bGamePaused = true;
	std::thread pauseThread(pauseCurrentRound);
	pauseThread.detach();
}


// resumes any paused in-round SFX + unflags bGamePaused
void unpauseGame()
{
	resumeRoundSFX();
	bGamePaused = false;
}


// draws elements of current round to screen
void renderRound()
{
	// render player / enemy AI scores
	drawRectNumber(enemy.score, -15.f, 40.f, 1.f, 0xbbffbb);
	drawRectNumber(player.score, 15.f, 40.f, 1.f, 0xbbffbb);

	// render ball and player + enemy AI paddles
	if (!ball.bHidden)
	{ drawRect(ball.position_x, ball.position_y, ball.halfSize_x, ball.halfSize_y, ball.color); }
	
	drawRect(enemy.position_x, enemy.position_y, enemy.halfSize_x, enemy.halfSize_y, enemy.color);
	drawRect(player.position_x, player.position_y, player.halfSize_x, player.halfSize_y, player.color);
}


// renders main menu
void drawMainMenu(int activeButton)
{
	float position_x = -30.f;
	float mainMenu_SinglePlayerPosition_y = -15.f;
	float mainMenu_MultiPlayerPosition_y = -23.f;
	float mainMenu_ExitPosition_y = -31.f;
	float highlightedSize = .85f;
	float defaultSize = .75f;

	drawRectText("C++ PONG", -68, 35, 3, 0xffffff);
	drawRectText("BY ANDREW CREEKMORE", -53, 12, 1, 0xcccccc);
	
	// draw menu options
	drawRectText("SINGLE PLAYER", position_x, mainMenu_SinglePlayerPosition_y, (activeButton == 0 ? highlightedSize : defaultSize), (activeButton == 0 ? 0xff0000 : 0xffffff));
	drawRectText("MULTIPLAYER", position_x, mainMenu_MultiPlayerPosition_y, (activeButton == 1 ? highlightedSize : defaultSize), (activeButton == 1 ? 0xff0000 : 0xffffff));
	drawRectText("EXIT", position_x, mainMenu_ExitPosition_y, (activeButton == 2 ? highlightedSize : defaultSize), (activeButton == 2 ? 0xff0000 : 0xffffff));
}


// processes user input (main menu)
void handleMainMenuInput(Input* input)
{
	if (pressed(KEY_W) || pressed(KEY_UP)) // choose game mode
	{
		if (activeMainMenuButton > 0) { activeMainMenuButton--; }
		else { activeMainMenuButton = 2; } // wrap around to last menu option
	}

	if (pressed(KEY_S) || pressed(KEY_DOWN))
	{
		if (activeMainMenuButton < 2) { activeMainMenuButton++; }
		else { activeMainMenuButton = 0; } // wrap around to first menu option
	}

	if (pressed(KEY_ENTER) || pressed(KEY_SPACE))
	{
		switch (activeMainMenuButton)
		{
		case 0: // SINGLE-PLAYER
			startNewGame(true);
			break;

		case 1: // MULTI-PLAYER
			startNewGame(false);
			break;

		case 2: // EXIT
			PostMessage(parentWindow, WM_CLOSE, 0, 0); // send "Really quit?" message box
			break;
		}
	}
}


// renders pause menu overlay
void drawPauseMenu(int activeButton)
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
	drawRectText("RESUME", position_x, pauseMenu_ResumePosition_y, (activeButton == 0 ? highlightedSize : defaultSize), (activeButton == 0 ? highlightedColor : defaultColor));
	drawRectText("NEW GAME", position_x, pauseMenu_NewGamePosition_y, (activeButton == 1 ? highlightedSize : defaultSize), (activeButton == 1 ? highlightedColor : defaultColor));
	drawRectText("MAIN MENU", position_x, pauseMenu_MainMenuPosition_y, (activeButton == 2 ? highlightedSize : defaultSize), (activeButton == 2 ? highlightedColor : defaultColor));
	drawRectText("EXIT", position_x, pauseMenu_ExitPosition_y, (activeButton == 3 ? highlightedSize : defaultSize), (activeButton == 3 ? highlightedColor : defaultColor));
}


// processes user input (pause menu)
void handlePauseMenuInput(Input* input)
{
	if (pressed(KEY_ESC)) // resume
	{ bGamePaused = false; }

	if (pressed(KEY_W) || pressed(KEY_UP))
	{ 
		if (activePauseMenuButton > 0) { activePauseMenuButton--; }
		else { activePauseMenuButton = 3; } // wrap around to last menu option
	}

	if (pressed(KEY_S) || pressed(KEY_DOWN))
	{ 
		if (activePauseMenuButton < 3) { activePauseMenuButton++; }
		else { activePauseMenuButton = 0; } // wrap around to first menu option
	}

	if (pressed(KEY_ENTER) || pressed(KEY_SPACE))
	{
		switch (activePauseMenuButton)
		{
		case 0: // RESUME
			unpauseGame();
			break;

		case 1: // NEW GAME (same mode as current)
			startNewGame(bActiveEnemyAI);
			
			break;

		case 2: // MAIN MENU
			activeMainMenuButton = 0; // always start at top option (single-player)
			currentGameMode = GM_MAINMENU;
			break;

		case 3: // EXIT
			PostMessage(parentWindow, WM_CLOSE, 0, 0); // send "Really quit?" message box
			break;
		}
	}
}


// checks if the ball has hit another entity and handles the collision response
void handleBallCollision()
{
	// collision check: with player
	if (ball.aabbVSaabb(player))
	{
		ball.position_x = player.position_x - player.halfSize_x - ball.halfSize_x; // reset position
		ball.velocity_x *= -1.f; // bounce away horizontally
		ball.velocity_y = (ball.position_y - player.position_y) * 2.f + (player.velocity_y * 0.75f); // bounce away vertically

		paddleSoundPlayer.setPosition(player.getAudioPosition_x(), player.getAudioPosition_y(), 0.f);
		paddleSoundPlayer.play(paddleImpactSound);
	}

	// collision check: with enemy AI
	else if (ball.aabbVSaabb(enemy))
	{
		ball.position_x = enemy.position_x + enemy.halfSize_x + ball.halfSize_x; // reset position
		ball.velocity_x *= -1.f; // bounce away horizontally
		ball.velocity_y = (ball.position_y - enemy.position_y) * 2.f + (enemy.velocity_y * 0.75f); // bounce away vertically

		paddleSoundPlayer.setPosition(enemy.getAudioPosition_x(), enemy.getAudioPosition_y(), 0.f);
		paddleSoundPlayer.play(paddleImpactSound);
	}

	// collision check: arena boundaries
	arenaCollision result = ball.checkForArenaBoundaryCollision(arena);

	if (result != NO_COLLISION)
	{ 
		switch (result)
		{
		case NO_COLLISION:
			break;

		case TOP_COLLISION:
			ball.position_y = arena.halfSize_y - ball.halfSize_y; // reset position to limit
			ball.velocity_y *= -1.f; // invert velocity to bounce away

			arenaBoundarySoundPlayer.setPosition(ball.getAudioPosition_x(), ball.getAudioPosition_y(), 0.f);
			arenaBoundarySoundPlayer.play(arenaBoundaryImpactSound);

			break;

		case BOTTOM_COLLISION:
			ball.position_y = -arena.halfSize_y + ball.halfSize_y; // reset position to limit
			ball.velocity_y *= -1.f; // invert velocity to bounce away

			arenaBoundarySoundPlayer.setPosition(ball.getAudioPosition_x(), ball.getAudioPosition_y(), 0.f);
			arenaBoundarySoundPlayer.play(arenaBoundaryImpactSound);
			break;

		case LEFT_COLLISION:
			player.score++; // PLAYER SCORES POINT
			scoredPointSoundPlayer.play(scoredPointSound);
			break;

		case RIGHT_COLLISION:
			enemy.score++; // ENEMY AI (or "player2" in multi-player mode) SCORES POINT
			scoredPointSoundPlayer.play(scoredPointSound);
			break;
		}

		// if either paddle scored a point
		if (result == LEFT_COLLISION || result == RIGHT_COLLISION)
		{
			// reset positions, setup next round, brief pause, resume
			resetRound();
		}
	}
}


// checks if a paddle has hit an arena boundary and handles the collision response
void handlePaddleCollision(Paddle &paddle)
{
	// collision check: arena boundaries
	arenaCollision result = paddle.checkForArenaBoundaryCollision(arena);

	switch (result)
	{
	case NO_COLLISION:
		break;

	case TOP_COLLISION:
		paddle.position_y = arena.halfSize_y - paddle.halfSize_y; // reset position
		paddle.velocity_y *= -0.5f; // bounce away from hit boundary
		break;

	case BOTTOM_COLLISION:
		paddle.position_y = -arena.halfSize_y + paddle.halfSize_y; // reset position
		paddle.velocity_y *= -0.5f; // bounce away from hit boundary
		break;
	}
}


// for enemy AI paddle in single-player: determine next movement
void decideNextMoveAI()
{
	if (!bActiveEnemyAI) { return; }

	// move much slower when ball is moving away vs coming at paddle
	float speedMultiplier = ball.velocity_x > 0.f ? 30.f : 85.f;

	// determine acceleration needed (direction, magnitude)
	float calculatedAcceleration = (ball.position_y - enemy.position_y) * speedMultiplier;

	// if don't have to move to keep ball in play 
	if (calculatedAcceleration <= 100.f && calculatedAcceleration >= -100.f)
	{
		if (ball.position_x < -25.f) // if close enough
		{
			// introduce additional movement to try and break stalemate bounce
			if (enemy.bLastMoveWasUp)
			{
				enemy.acceleration += 800.f;
				enemy.bLastMoveWasUp = true;
			}
			else
			{
				enemy.acceleration -= 800.f;
				enemy.bLastMoveWasUp = false;
			}
		}
	}

	else
	{
		// track last move direction to reduce jitter/rapid direction-swapping
		enemy.bLastMoveWasUp = calculatedAcceleration > 0.f ? true : false;

		// process the acceleration for next move() call
		enemy.acceleration = clampFloat(-1200.f, calculatedAcceleration, 1200.f);
	}
}


// pauses all in-round SFX
void pauseRoundSFX()
{
	paddleSoundPlayer.pause();
	arenaBoundarySoundPlayer.pause();
	scoredPointSoundPlayer.pause();
}


// resumes any currently-paused in-round SFX
void resumeRoundSFX()
{
	paddleSoundPlayer.resume();
	arenaBoundarySoundPlayer.resume();
	scoredPointSoundPlayer.resume();
}


// stops playback of all in-round SFX
void stopRoundSFX()
{
	paddleSoundPlayer.stop();
	arenaBoundarySoundPlayer.stop();
	scoredPointSoundPlayer.stop();
}
