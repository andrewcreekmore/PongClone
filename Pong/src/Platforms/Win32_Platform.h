#pragma once

/*
===========================================================================
Win32_Platform: Windows platform-specific entry point
- contains GameWindowCallback and WinMain functions
===========================================================================
*/

struct RenderState
{
	int height, width;
	void* memory;
	BITMAPINFO bitmapInfo;
};

RenderState renderState;
HWND parentWindow;
