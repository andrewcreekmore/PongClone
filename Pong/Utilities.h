#pragma once

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
