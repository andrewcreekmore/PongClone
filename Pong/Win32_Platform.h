#pragma once

struct RenderState
{
	int height, width;
	void* memory;
	BITMAPINFO bitmapInfo;
};

RenderState renderState;

bool bGameRunning = true;