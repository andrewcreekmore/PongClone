#include "SoundPlayer.h"


SoundPlayer::SoundPlayer()
{
	alGenSources(1, &p_Source);
	alSourcei(p_Source, AL_BUFFER, p_Buffer);
	alSourcei(p_Source, AL_SOURCE_RELATIVE, true);
	alSourcef(p_Source, AL_GAIN, p_Gain);

	AL_CheckAndThrow();
}


SoundPlayer::SoundPlayer(float gain)
{
	p_Gain = gain;
	alGenSources(1, &p_Source);
	alSourcei(p_Source, AL_BUFFER, p_Buffer);
	alSourcei(p_Source, AL_SOURCE_RELATIVE, true);
	alSourcef(p_Source, AL_GAIN, p_Gain);

	AL_CheckAndThrow();
}


SoundPlayer::~SoundPlayer()
{
	alDeleteSources(1, &p_Source);
}


void SoundPlayer::play(const ALuint bufferToPlay)
{
	if (bufferToPlay != p_Buffer) // only upload to buffer if changed
	{
		p_Buffer = bufferToPlay;
		alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
	}

	alSourcePlay(p_Source);
	AL_CheckAndThrow();
}


void SoundPlayer::stop()
{
	alSourceStop(p_Source);
	AL_CheckAndThrow();
}


void SoundPlayer::pause()
{
	alSourcePause(p_Source);
	AL_CheckAndThrow();
}


void SoundPlayer::resume()
{
	if (isPaused())
	{
		alSourcePlay(p_Source);
		AL_CheckAndThrow();
	}
}


bool SoundPlayer::isPlaying()
{
	ALint playState;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &playState);
	return (playState == AL_PLAYING);
}


bool SoundPlayer::isPaused()
{
	ALint playState;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &playState);
	return (playState == AL_PAUSED);
}

void SoundPlayer::setBufferToPlay(const ALuint& bufferToPlay)
{
	if (bufferToPlay != p_Buffer)
	{
		p_Buffer = bufferToPlay;
		alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
		AL_CheckAndThrow();
	}
}


void SoundPlayer::setGain(const float& val)
{
	float newVolume = val;
	if (newVolume < 0.f) { newVolume = 0.f; }
	else if (newVolume > 5.f) { newVolume = 5.f; }
	alSourcef(p_Source, AL_GAIN, newVolume);
}


void SoundPlayer::setLooping(const bool& bShouldLoop)
{
	alSourcei(p_Source, AL_LOOPING, (ALint)bShouldLoop);
	AL_CheckAndThrow();
}


void SoundPlayer::setPosition(const float& x, const float& y, const float& z)
{
	alSource3f(p_Source, AL_POSITION, x, y, z);
	AL_CheckAndThrow();
}

