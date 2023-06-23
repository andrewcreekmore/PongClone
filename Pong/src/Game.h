#pragma once

#include <windows.h>
#include "Framework/Audio/SoundSource.h"
#include "Framework/Audio/SoundBuffer.h"
#include "Entities/Entity.h"
#include "Entities/Paddle.h"
#include "Entities/Ball.h"
#include "Platforms/Platform_Common.h"

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

void handleMovementInput(Input* input, float deltaTime);

void simulateRound(float deltaTime);

void startNewGame(bool activeAI);

void resetRound();

void renderRound();

void drawPauseMenu(int activeButton);

void pauseCurrentRound();

void pauseGame();

void drawMainMenu(int activeButton);

void handleBallCollision();

void handlePaddleCollision(Paddle paddle);

void decideNextMoveAI();

void simulateGame(Input* input, float deltaTime, HWND parentWindow);

inline void unpauseGame() { bGamePaused = false; }