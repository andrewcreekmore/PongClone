#pragma once

#include <windows.h>
#include "Framework/Audio/SoundDevice.h"
#include "Framework/Audio/SoundLibrary.h"
#include "Framework/Audio/SoundPlayer.h"
#include "Framework/Renderer.h"
#include "Framework/Utilities.h"
#include "Entities/Paddle.h"
#include "Entities/Ball.h"
#include "Platforms/Platform_Common.h"
#include <chrono>
#include <thread>

/*
===========================================================================
Game: overall game-loop logic
- manages game state (in-menu, in-round, round active status)
- handles simulation of current round
- handles UI input (main menu, pause menu)
===========================================================================
*/

enum gameMode
{
	GM_MAINMENU,
	GM_DIFFICULTYSELECT,
	GM_GAMEPLAY,
	GM_PAUSEMENU,
};

struct Arena
{
	unsigned int color = 0x091238;
	float halfSize_x = 85.f;
	float halfSize_y = 45.f;
};

//---------------------------// 
// game audio initialization

// retrieve default sound device
SoundDevice* currentSoundDevice = LISTENER->get();

// create sound players + load SFX
SoundPlayer paddleSoundPlayer;
SoundPlayer arenaBoundarySoundPlayer(2.5f);
SoundPlayer scoredPointSoundPlayer(0.5f);
int paddleImpactSound = SFX_LOAD(".\\res\\Sounds\\Pop.ogg");
int arenaBoundaryImpactSound = SFX_LOAD(".\\res\\Sounds\\pingpongboard.ogg");
int scoredPointSound = SFX_LOAD(".\\res\\Sounds\\applause-mono-24bit-48khz.wav");

//---------------------------// 

class Game
{
public:

	Game();

private:

	HWND parentWindow;

	Arena arena;
	Paddle player; // positioned to right 
	Paddle enemy; // positioned to left
	Ball ball;

	gameMode currentGameMode;
	bool bGamePaused;
	bool bRoundActive;
	bool bActiveEnemyAI;
	int activeMainMenuButton;
	int activePauseMenuButton;

public:

	void setParentWindow(HWND window) { parentWindow = window; }
	void simulateGame(Input* input, float deltaTime);
	void unpauseGame();

private:

	void simulateRound(float deltaTime);

	void handleMovementInput(Input* input, float deltaTime);

	void startNewGame(bool activeAI);

	static void setupNextRound(Game* currentGame);

	void resetRound();

	void renderRound();

	void drawPauseMenu(int activeButton);

	void handlePauseMenuInput(Input* input);

	static void pauseCurrentRound(Game* currentGame);

	void pauseGame();

	void drawMainMenu(int activeButton);

	void handleMainMenuInput(Input* input);

	void handleBallCollision();

	void handlePaddleCollision(Paddle& paddle);

	void decideNextMoveAI();

	void pauseRoundSFX();

	void resumeRoundSFX();

	void stopRoundSFX();
};