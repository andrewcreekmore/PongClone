#pragma once

struct RenderState
{
	int height, width;
	void* memory;
	BITMAPINFO bitmapInfo;
};

RenderState renderState;
HWND parentWindow;