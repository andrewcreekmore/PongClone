#pragma once

#include <time.h>

inline int clampInt(int min, int val, int max)
{
	if (val < min) return min;
	if (val > max) return max;

	return val;
}

inline float clampFloat(float min, float val, float max)
{
	if (val < min) return min;
	if (val > max) return max;

	return val;
}

inline void delay(int numSeconds)
{
	// convert into milliseconds
	int numMilliseconds = 1000 * numSeconds;

	// storing start time
	clock_t startTime = clock();

	// looping until required time
	while (clock() < startTime + numMilliseconds)
		;
}