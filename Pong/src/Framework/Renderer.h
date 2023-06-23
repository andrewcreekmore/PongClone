#pragma once

#include "Utilities.h"

static float renderScale = 0.01f;

void clearScreen(unsigned int color);
void drawRectInPixels(int x0, int y0, int x1, int y1, unsigned int color);
void drawRect(float x, float y, float halfSize_x, float halfSize_y, unsigned int color);
void drawArenaBoundaries(float arena_x, float arena_y, unsigned int color);
void drawRectText(const char* textContent, float position_x, float position_y, float textSize, unsigned int textColor);
void drawRectNumber(int numValue, float numPosition_x, float numPosition_y, float numSize, unsigned int numColor);