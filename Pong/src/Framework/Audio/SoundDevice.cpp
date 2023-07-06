#include "SoundDevice.h"
#include <AL\al.h>
#include <stdio.h>
#include "OpenAL_ErrorCheck.h"

/*
===========================================================================
SoundDevice: OpenAL device implementation
- gets the default system sound device and sets up AL sound context
- provides methods for getting/setting device gain
- singleton
===========================================================================
*/

static SoundDevice* _instance = nullptr;

SoundDevice* SoundDevice::get()
{
	init();
	return _instance;
}

void SoundDevice::init()
{
	if (_instance == nullptr)
		_instance = new SoundDevice();
}

//---------------------------// 

float SoundDevice::getGain()
{
	float currentGain;
	alGetListenerf(AL_GAIN, &currentGain);
	AL_CheckAndThrow();
	return currentGain;
}

//---------------------------
// sets the master volume of our listeners, clamped
void SoundDevice::setGain(const float& val)
{
	float newVolume = val;
	if (newVolume < 0.f) { newVolume = 0.f; }
	else if (newVolume > 5.f) { newVolume = 5.f; }

	alListenerf(AL_GAIN, newVolume);
	AL_CheckAndThrow();
}

//---------------------------// 

SoundDevice::SoundDevice()
{
	// by passing nullptr, get default device
	p_ALCDevice = alcOpenDevice(nullptr);
	if (!p_ALCDevice)
		throw("failed to get sound device");

	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);  // create context
	if (!p_ALCContext)
		throw("failed to set sound context");

	if (!alcMakeContextCurrent(p_ALCContext))   // make context current
		throw("failed to make context current");

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
		name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
	printf("opened \"%s\"\n", name);
}

SoundDevice::~SoundDevice()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(p_ALCContext);
	alcCloseDevice(p_ALCDevice);
}