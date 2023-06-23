#pragma once


struct KeyState
{
	bool bIsDown;
	bool bHasChanged;
};


enum
{
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_W,
	KEY_S,
	KEY_A,
	KEY_D,
	KEY_ENTER,
	KEY_SPACE,
	KEY_ESC,

	KEY_COUNT, // count of num of keys; should be last element
};


struct Input
{
	KeyState keyState[KEY_COUNT];
};


