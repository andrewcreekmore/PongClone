#include "SoundDevice.h"
#include <AL\al.h>
#include <stdio.h>

SoundDevice* SoundDevice::get()
{
	static SoundDevice* sndDevice = new SoundDevice;
	return sndDevice;
}


SoundDevice::SoundDevice()
{
	// by passing nullptr, get default device
	p_ALCDevice = alcOpenDevice(nullptr);
	if (!p_ALCDevice)
	{ throw("Failed to get sound device!"); }

	// set context
	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);
	if (!p_ALCContext)
	{ throw("Failed to set sound context!");}

	// make context current
	if (!alcMakeContextCurrent(p_ALCContext))
	{ throw("Failed to make context current!"); }

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
	{ name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER); }

	if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
	{ name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER); }

	printf("Opened \" % s\"\n", name);
}


SoundDevice::~SoundDevice()
{
	if (!alcMakeContextCurrent(nullptr))
	{ throw("Failed to set context to nullptr!"); }

	alcDestroyContext(p_ALCContext);
	if (p_ALCContext)
	{ throw("Failed to unset context during close!"); }

	if (!alcCloseDevice(p_ALCDevice))
	{ throw("Failed to close sound device!"); }
}
