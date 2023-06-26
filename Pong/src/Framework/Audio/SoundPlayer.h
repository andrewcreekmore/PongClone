#pragma once
#include <AL\al.h>
#include <iostream>
#include "OpenAL_ErrorCheck.h"

class SoundPlayer
{
public:
		
	SoundPlayer();
	SoundPlayer(float gain);
	~SoundPlayer();

	// playback
	void play(const ALuint bufferToPlay);
	void stop();
	void pause();
	void resume();
	bool isPlaying();
	bool isPaused();

	// setters
	void setBufferToPlay(const ALuint& bufferToPlay);
	void setGain(const float& val);
	void setLooping(const bool& bShouldLoop);
	void setPosition(const float& x, const float& y, const float& z);

private:

	ALuint p_Source;
	ALuint p_Buffer = 0;

	float p_Gain = 1.f;
};

