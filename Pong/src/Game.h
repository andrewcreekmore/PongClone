#pragma once

#include <windows.h>
#include "Framework/Audio/SoundDevice.h"
#include "Framework/Audio/SoundLibrary.h"
#include "Framework/Audio/SoundPlayer.h"
#include "Framework/Renderer.h"
#include "Framework/Utilities.h"
#include "Entities/Entity.h"
#include "Entities/Paddle.h"
#include "Entities/Ball.h"
#include "Platforms/Platform_Common.h"
#include <chrono>
#include <thread>

#define isDown(key) input->keyState[key].bIsDown
#define pressed(key) (input->keyState[key].bIsDown && input->keyState[key].bHasChanged)
#define released(key) (!input->keyState[key].bIsDown && input->keyState[key].bHasChanged)

enum gameMode
{
	GM_MAINMENU,
	GM_DIFFICULTYSELECT,
	GM_GAMEPLAY,
	GM_PAUSEMENU,
};

bool bAppRunning = true;
bool bGamePaused = false;
bool bRoundActive = false;
gameMode currentGameMode = GM_MAINMENU;
int activeMainMenuButton = 0;
int activePauseMenuButton = 0;
bool bActiveEnemyAI;
SoundPlayer gameSoundPlayers[];

class Arena : public Entity
{
public:

	inline Arena() // sets defaults
	{
		color = 0x091238;
		halfSize_x = 85.f;
		halfSize_y = 45.f;
	}
};

void simulateGame(Input* input, float deltaTime, HWND parentWindow);

void simulateRound(float deltaTime);

void handleMovementInput(Input* input, float deltaTime);

void startNewGame(bool activeAI);

void resetRound();

void renderRound();

void drawPauseMenu(int activeButton);

void handlePauseMenuInput(Input* input);

void pauseCurrentRound();

void pauseGame();

void unpauseGame();

void drawMainMenu(int activeButton);

void handleMainMenuInput(Input* input);

void handleBallCollision();

void handlePaddleCollision(Paddle &paddle);

void decideNextMoveAI();

void pauseRoundSFX();

void resumeRoundSFX();

void stopRoundSFX();