#pragma once

/*
===========================================================================
Platform_Common.h: platform-agnostic data
- contains input-related defines, enums, and structs
===========================================================================
*/

#define isDown(key) input->keyState[key].bIsDown
#define pressed(key) (input->keyState[key].bIsDown && input->keyState[key].bHasChanged)
#define released(key) (!input->keyState[key].bIsDown && input->keyState[key].bHasChanged)

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

struct KeyState
{
	bool bIsDown;
	bool bHasChanged;
};

struct Input
{
	KeyState keyState[KEY_COUNT];
};